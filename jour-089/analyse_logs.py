"""Jour 089 — Analyse d'un fichier journal (log) : erreurs par heure

Problème : un serveur, une passerelle IoT ou une application écrit des
milliers de lignes de journal. Quand quelque chose ne va pas, il faut
rapidement savoir quand les erreurs ont eu lieu, lesquelles reviennent le plus
et si un pic est apparu.
Solution : chaque ligne est analysée avec une expression régulière
(date, heure, niveau, module, message). On compte les niveaux par heure, on
regroupe les messages d'erreur en remplaçant les nombres variables par « N »
pour reconnaître les erreurs identiques, et on signale les heures dont le
nombre d'erreurs dépasse la moyenne + 2 écarts-types.
On apprend : les expressions régulières avec groupes nommés, Counter,
la normalisation de messages et la détection simple d'anomalies.
"""
import random
import re
import statistics
import sys
from collections import Counter, defaultdict

LIGNE = re.compile(r"^(?P<date>\d{4}-\d{2}-\d{2}) (?P<heure>\d{2}):\d{2}:\d{2} (?P<niveau>INFO|WARN|ERROR) "
                   r"\[(?P<module>[\w-]+)\] (?P<message>.*)$")


def generer_journal(rng: random.Random) -> list[str]:
    lignes = []
    for h in range(24):
        for _ in range(rng.randint(30, 60)):
            m, s = rng.randrange(60), rng.randrange(60)
            r = rng.random()
            erreur_pic = h == 14 and r < 0.35       # incident simulé à 14 h
            if r < 0.04 or erreur_pic:
                msg = rng.choice([f"capteur {rng.randint(1, 9)} ne répond pas après {rng.randint(1000, 5000)} ms",
                                  f"échec d'envoi MQTT (code {rng.choice([3, 5])})", "base de données verrouillée"])
                lignes.append(f"2026-10-27 {h:02d}:{m:02d}:{s:02d} ERROR [passerelle] {msg}")
            elif r < 0.12:
                lignes.append(f"2026-10-27 {h:02d}:{m:02d}:{s:02d} WARN [batterie] tension basse {rng.uniform(3.2, 3.4):.2f} V")
            else:
                lignes.append(f"2026-10-27 {h:02d}:{m:02d}:{s:02d} INFO [capteurs] mesure envoyée")
    lignes.insert(100, "ligne corrompue ###")
    return lignes


def analyser(lignes: list[str]) -> None:
    par_heure = defaultdict(Counter)
    erreurs = Counter()
    illisibles = 0
    for l in lignes:
        m = LIGNE.match(l.strip())
        if not m:
            illisibles += 1
            continue
        par_heure[m["heure"]][m["niveau"]] += 1
        if m["niveau"] == "ERROR":
            erreurs[re.sub(r"\d+", "N", m["message"])] += 1
    nb = [par_heure[h]["ERROR"] for h in sorted(par_heure)]
    seuil = statistics.mean(nb) + 2 * statistics.pstdev(nb)
    print(f"{len(lignes)} lignes, {illisibles} illisible(s)\n\nHeure  INFO  WARN  ERROR")
    for h in sorted(par_heure):
        c = par_heure[h]
        alerte = "  ⚠ pic anormal" if c["ERROR"] > seuil else ""
        print(f"  {h}h {c['INFO']:5d} {c['WARN']:5d} {c['ERROR']:6d} {'█' * c['ERROR']}{alerte}")
    print("\nErreurs les plus fréquentes :")
    for msg, n in erreurs.most_common(5):
        print(f"  {n:4d} × {msg}")


def main() -> int:
    if len(sys.argv) == 2:
        try:
            with open(sys.argv[1], encoding="utf-8", errors="replace") as f:
                analyser(f.readlines())
        except OSError as e:
            print(f"Impossible de lire le journal : {e}", file=sys.stderr)
            return 1
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 analyse_logs.py [journal.log]", file=sys.stderr)
        return 1
    analyser(generer_journal(random.Random(89)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
