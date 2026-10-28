"""Jour 104 — Filtre moyenne glissante sur des mesures bruitées

Problème : un capteur analogique (température, niveau d'eau, courant) donne
des valeurs qui sautillent à cause du bruit électrique. Afficher ou décider
sur une valeur brute fait clignoter un affichage ou déclencher un relais à
tort.
Solution : deux filtres classiques de l'embarqué. (1) La moyenne glissante
sur N échantillons, calculée en O(1) par mise à jour d'une somme (on ajoute
la nouvelle valeur, on retire la plus ancienne). (2) La moyenne exponentielle
(EMA) : y = y + α(x − y), qui ne demande qu'une seule variable de mémoire. On
mesure la réduction du bruit (écart-type) et le retard introduit par chaque
filtre sur un signal simulé.
On apprend : les filtres numériques simples, deque à taille fixe, le
compromis lissage / réactivité et la mesure de l'efficacité d'un filtre.
"""
import math
import random
import statistics
import sys
from collections import deque


class MoyenneGlissante:
    def __init__(self, n: int):
        self.fenetre, self.n, self.somme = deque(), n, 0.0

    def filtrer(self, x: float) -> float:
        self.fenetre.append(x)
        self.somme += x
        if len(self.fenetre) > self.n:
            self.somme -= self.fenetre.popleft()
        return self.somme / len(self.fenetre)


class MoyenneExponentielle:
    def __init__(self, alpha: float):
        if not 0 < alpha <= 1:
            raise ValueError("alpha dans ]0, 1]")
        self.alpha, self.y = alpha, None

    def filtrer(self, x: float) -> float:
        self.y = x if self.y is None else self.y + self.alpha * (x - self.y)
        return self.y


def signal(rng: random.Random, n: int = 120) -> tuple[list[float], list[float]]:
    vrai = [25 + (5 if t >= 60 else 0) + 0.5 * math.sin(t / 15) for t in range(n)]   # saut à t = 60
    return vrai, [v + rng.gauss(0, 0.8) for v in vrai]


def retard(filtre: list[float], seuil: float = 27.5, depart: int = 60) -> int:
    return next(t for t in range(depart, len(filtre)) if filtre[t] >= seuil) - depart


def main() -> int:
    n = 8
    if len(sys.argv) == 2:
        try:
            n = int(sys.argv[1])
            if not 1 <= n <= 50:
                raise ValueError
        except ValueError:
            print("Usage : python3 moyenne_glissante.py [taille de fenêtre 1..50]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 moyenne_glissante.py [taille]", file=sys.stderr)
        return 1
    vrai, brut = signal(random.Random(104))
    mg, ema = MoyenneGlissante(n), MoyenneExponentielle(2 / (n + 1))
    f1 = [mg.filtrer(x) for x in brut]
    f2 = [ema.filtrer(x) for x in brut]
    erreur = lambda f: statistics.pstdev([a - b for a, b in zip(f[10:60], vrai[10:60])])
    print(f"Signal : 25 °C puis saut à 30 °C à t = 60, bruit ±0,8 °C ; fenêtre N = {n}\n")
    print(f"{'Filtre':24} {'bruit résiduel':>15} {'retard au saut':>15}")
    print(f"{'Brut':24} {erreur(brut):13.2f} °C {retard(brut):12d} éch.")
    print(f"{'Moyenne glissante':24} {erreur(f1):13.2f} °C {retard(f1):12d} éch.")
    print(f"{'Moyenne exponentielle':24} {erreur(f2):13.2f} °C {retard(f2):12d} éch.")
    print("\n  t   brut  glissante  EMA")
    for t in range(54, 72, 2):
        print(f"{t:3d} {brut[t]:6.2f} {f1[t]:8.2f} {f2[t]:7.2f}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
