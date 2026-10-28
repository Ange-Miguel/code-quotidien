"""Jour 119 — Alerte de niveau d'eau : pompe automatique

Problème : la citerne du toit (jour 118) se remplit grâce à une pompe qui
tire l'eau d'un puits ou d'un réservoir au sol. Il faut démarrer la pompe
automatiquement quand le niveau est bas, l'arrêter quand la citerne est
pleine, et surtout protéger la pompe : jamais de fonctionnement à sec, et
pas de démarrages trop rapprochés.
Solution : un automate avec deux seuils (hystérésis : marche à 30 %, arrêt à
95 %), une sécurité « manque d'eau » (le niveau ne monte pas alors que la
pompe tourne depuis 3 minutes → arrêt et alarme), un délai minimal entre deux
démarrages et une alerte par message. On simule une journée de consommation
domestique (pics le matin et le soir) et on affiche le journal.
On apprend : la logique de commande avec sécurités, la gestion d'états
temporisés, la simulation d'un système physique et la journalisation.
"""
import random
import sys

CAPACITE_L = 1000
SEUIL_MARCHE, SEUIL_ARRET = 30, 95
DEBIT_POMPE = 20          # L/min
DELAI_REDEMARRAGE = 15    # minutes
DELAI_SECURITE = 3        # minutes sans montée → marche à sec


class Pompe:
    def __init__(self):
        self.marche = False
        self.arret_depuis = 999
        self.alarme = False
        self.niveau_au_demarrage = 0.0
        self.minutes_marche = 0
        self.demarrages = 0
        self.journal: list[str] = []

    def log(self, t: int, msg: str) -> None:
        self.journal.append(f"{t // 60:02d}h{t % 60:02d}  {msg}")

    def decider(self, t: int, pct: float, eau_au_puits: bool) -> None:
        if self.alarme:
            return
        if self.marche:
            self.minutes_marche += 1
            if self.minutes_marche >= DELAI_SECURITE and pct <= self.niveau_au_demarrage + 0.5:
                self.marche, self.alarme = False, True
                self.log(t, "⚠ ALARME : le niveau ne monte pas (puits vide ?) — pompe arrêtée, SMS envoyé")
            elif pct >= SEUIL_ARRET:
                self.marche, self.arret_depuis = False, 0
                self.log(t, f"arrêt pompe (citerne à {pct:.0f} %)")
        else:
            self.arret_depuis += 1
            if pct <= SEUIL_MARCHE and self.arret_depuis >= DELAI_REDEMARRAGE:
                self.marche, self.minutes_marche, self.niveau_au_demarrage = True, 0, pct
                self.demarrages += 1
                self.log(t, f"démarrage pompe (citerne à {pct:.0f} %)")


def consommation(t: int, rng: random.Random) -> float:
    heure = t / 60
    base = 6 if 6 <= heure < 8 or 18 <= heure < 21 else 1.5 if 8 <= heure < 23 else 0.1
    return max(0.0, rng.gauss(base, base / 2))


def main() -> int:
    panne_puits = (17 * 60, 19 * 60)
    if len(sys.argv) == 2 and sys.argv[1] == "--sans-panne":
        panne_puits = (-1, -1)
    elif len(sys.argv) > 1:
        print("Usage : python3 pompe_auto.py [--sans-panne]", file=sys.stderr)
        return 1
    rng = random.Random(119)
    pompe, litres = Pompe(), 600.0
    minimum = 100.0
    for t in range(24 * 60):
        eau_au_puits = not (panne_puits[0] <= t < panne_puits[1])
        if pompe.marche and eau_au_puits:
            litres = min(CAPACITE_L, litres + DEBIT_POMPE)
        litres = max(0.0, litres - consommation(t, rng))
        pct = 100 * litres / CAPACITE_L
        minimum = min(minimum, pct)
        pompe.decider(t, pct, eau_au_puits)
    print("Journal de la journée (le puits est à sec entre 17 h et 19 h) :")
    print("\n".join("   " + l for l in pompe.journal))
    print(f"\n{pompe.demarrages} démarrage(s), niveau minimal {minimum:.0f} %, alarme {'active' if pompe.alarme else 'non'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
