"""Jour 128 — Watchdog simulé : redémarrage si une tâche bloque

Problème : un objet connecté installé sur un toit ou dans un champ ne doit
jamais rester planté : un capteur I2C qui ne répond plus, une boucle infinie,
une connexion réseau bloquée… et l'appareil ne transmet plus rien pendant des
semaines. Personne n'est là pour appuyer sur « reset ».
Solution : le chien de garde (watchdog). C'est un compteur matériel qui
redémarre le microcontrôleur s'il n'est pas « nourri » (remis à zéro)
régulièrement. Le programme ne le nourrit que si toutes les tâches ont donné
signe de vie. On simule un appareil dont une tâche se bloque au hasard ; le
watchdog détecte le blocage, redémarre, et on enregistre la cause du
redémarrage pour le diagnostic.
On apprend : le principe du watchdog, le « nourrissage » conditionnel, la
reprise après redémarrage et la journalisation des causes de reset.
"""
import random
import sys

TIMEOUT_MS = 2000


class Watchdog:
    def __init__(self, timeout: int):
        self.timeout, self.compteur = timeout, 0

    def nourrir(self) -> None:
        self.compteur = 0

    def tic(self, ms: int) -> bool:
        """Retourne True si le délai est écoulé (redémarrage)."""
        self.compteur += ms
        return self.compteur >= self.timeout


class Appareil:
    TACHES = ["capteurs", "radio", "stockage"]

    def __init__(self, rng: random.Random):
        self.rng = rng
        self.redemarrages: list[tuple[int, str]] = []
        self.mesures_envoyees = 0
        self.demarrer()

    def demarrer(self) -> None:
        self.vivantes = set()
        self.bloquee = None

    def cycle(self) -> None:
        """Une itération de la boucle principale (100 ms)."""
        for t in self.TACHES:
            if self.bloquee == t:
                continue
            if self.rng.random() < 0.000002:      # bug rare : la tâche se bloque
                self.bloquee = t
                continue
            self.vivantes.add(t)
        if self.bloquee is None:
            self.mesures_envoyees += 1


def simuler(heures: int, avec_watchdog: bool, rng: random.Random) -> Appareil:
    a, wd = Appareil(rng), Watchdog(TIMEOUT_MS)
    for pas in range(heures * 36_000):            # pas de 100 ms
        a.cycle()
        if a.vivantes == set(Appareil.TACHES):    # toutes les tâches ont donné signe de vie
            wd.nourrir()
            a.vivantes.clear()
        if avec_watchdog and wd.tic(100):
            a.redemarrages.append((pas // 10, a.bloquee or "inconnue"))
            a.demarrer()
            wd.nourrir()
    return a


def main() -> int:
    heures = 24
    if len(sys.argv) == 2:
        try:
            heures = int(sys.argv[1])
            if not 1 <= heures <= 168:
                raise ValueError
        except ValueError:
            print("Usage : python3 watchdog.py [heures simulées 1..168]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 watchdog.py [heures]", file=sys.stderr)
        return 1
    attendu = heures * 36_000
    sans = simuler(heures, False, random.Random(128))
    avec = simuler(heures, True, random.Random(128))
    print(f"{heures} h de fonctionnement simulées (boucle de 100 ms, watchdog de {TIMEOUT_MS} ms)\n")
    print(f"Sans watchdog : {sans.mesures_envoyees / attendu:6.1%} des mesures envoyées (bloqué sur « {sans.bloquee} »)")
    print(f"Avec watchdog : {avec.mesures_envoyees / attendu:6.1%} des mesures envoyées, {len(avec.redemarrages)} redémarrage(s)")
    print("\nJournal des causes de redémarrage (conservé en mémoire non volatile) :")
    for s, cause in avec.redemarrages[:8]:
        print(f"   {s // 3600:02d}h{s % 3600 // 60:02d}m{s % 60:02d}s  tâche bloquée : {cause}")
    if len(avec.redemarrages) > 8:
        print(f"   … et {len(avec.redemarrages) - 8} autre(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
