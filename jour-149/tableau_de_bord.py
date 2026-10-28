"""Jour 149 — Tableau de bord console des capteurs en temps réel

Problème : pendant la mise au point d'une installation (serre, chambre
froide, atelier), on veut voir d'un coup d'œil l'état de tous les capteurs,
leurs tendances et les alertes, directement dans le terminal du Raspberry Pi,
sans interface graphique.
Solution : un tableau de bord texte rafraîchi périodiquement : pour chaque
capteur, la dernière valeur, un mini-graphique (sparkline) de l'historique
récent avec les caractères ▁▂▃▄▅▆▇█, une flèche de tendance, l'état
(normal / alerte / hors ligne si aucune donnée récente) et l'heure de la
dernière mesure. Les codes ANSI permettent la couleur et le retour en haut
de l'écran. En démo, 3 images du tableau sont produites sans attendre ;
avec --live, il se rafraîchit chaque seconde.
On apprend : les séquences d'échappement ANSI, les sparklines, deque à
taille fixe, la détection de capteur muet et la mise en page console.
"""
import math
import random
import sys
import time
from collections import deque

BARRES = "▁▂▃▄▅▆▇█"
ANSI = sys.stdout.isatty()
COULEUR = {"normal": "\033[32m", "alerte": "\033[31m", "hors ligne": "\033[90m"} if ANSI else {}
FIN = "\033[0m" if ANSI else ""


class Capteur:
    def __init__(self, nom: str, unite: str, mini: float, maxi: float, base: float, amplitude: float):
        self.nom, self.unite, self.mini, self.maxi = nom, unite, mini, maxi
        self.base, self.amplitude = base, amplitude
        self.historique: deque[float] = deque(maxlen=24)
        self.derniere = None

    def mesurer(self, t: int, rng: random.Random, muet: bool = False) -> None:
        if muet:
            return
        self.historique.append(self.base + self.amplitude * math.sin(t / 6) + rng.gauss(0, self.amplitude / 5))
        self.derniere = t

    def etat(self, t: int) -> str:
        if self.derniere is None or t - self.derniere > 5:
            return "hors ligne"
        return "normal" if self.mini <= self.historique[-1] <= self.maxi else "alerte"


def sparkline(valeurs) -> str:
    if not valeurs:
        return ""
    lo, hi = min(valeurs), max(valeurs)
    return "".join(BARRES[int((v - lo) / (hi - lo or 1) * (len(BARRES) - 1))] for v in valeurs)


def tendance(valeurs) -> str:
    if len(valeurs) < 6:
        return "·"
    ecart = sum(list(valeurs)[-3:]) / 3 - sum(list(valeurs)[-6:-3]) / 3
    return "↗" if ecart > 0.05 else "↘" if ecart < -0.05 else "→"


def afficher(capteurs: list[Capteur], t: int) -> None:
    if ANSI:
        print("\033[H\033[J", end="")
    print(f"=== Tableau de bord — serre de Calavi — t = {t} s ===")
    print(f"{'Capteur':16} {'Valeur':>12}  {'Historique':24} T  État")
    for c in capteurs:
        e = c.etat(t)
        valeur = f"{c.historique[-1]:.1f} {c.unite}" if c.historique else "—"
        print(f"{c.nom:16} {valeur:>12}  {sparkline(c.historique):24} {tendance(c.historique)}  "
              f"{COULEUR.get(e, '')}{e}{FIN}")
    alertes = [c.nom for c in capteurs if c.etat(t) != "normal"]
    print(f"Alertes : {', '.join(alertes) if alertes else 'aucune'}\n")


def main() -> int:
    live = sys.argv[1:] == ["--live"]
    if len(sys.argv) > 1 and not live:
        print("Usage : python3 tableau_de_bord.py [--live]", file=sys.stderr)
        return 1
    rng = random.Random(149)
    capteurs = [Capteur("Température air", "°C", 18, 32, 27, 3.5), Capteur("Humidité air", "%", 50, 85, 70, 9),
                Capteur("Humidité sol", "%", 35, 80, 52, 6), Capteur("Lumière", "klx", 0, 90, 45, 30),
                Capteur("Niveau cuve", "%", 20, 100, 33, 8)]
    for t in range(40 if not live else 10**9):
        for i, c in enumerate(capteurs):
            c.mesurer(t, rng, muet=(i == 2 and t > 30))     # le capteur de sol tombe en panne à t = 30
        if live:
            afficher(capteurs, t)
            time.sleep(1)
        elif t in (12, 25, 39):
            afficher(capteurs, t)
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        sys.exit(0)
