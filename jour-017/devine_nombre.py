"""Jour 017 — Jeu « devine le nombre » avec nombre d'essais

Problème : un petit jeu pour apprendre la logique de recherche : deviner un
nombre entre 1 et 100 avec le moins d'essais possible.
Solution : le jeu répond « plus grand » ou « plus petit ». En mode démo, un
joueur automatique applique la recherche dichotomique (toujours proposer le
milieu de l'intervalle restant) et trouve toujours en 7 essais au plus,
puisque 2^7 = 128 > 100. En mode interactif (--jouer), c'est vous qui jouez.
On apprend : les boucles while, la recherche dichotomique, le logarithme en
base 2 et la séparation entre la logique du jeu et l'interface.
"""
import math
import random
import sys


class Partie:
    def __init__(self, maximum: int = 100, secret: int | None = None):
        self.maximum = maximum
        self.secret = secret if secret is not None else random.randint(1, maximum)
        self.essais = 0

    def proposer(self, n: int) -> str:
        self.essais += 1
        if n < self.secret:
            return "plus grand"
        if n > self.secret:
            return "plus petit"
        return "gagné"


def joueur_automatique(partie: Partie) -> int:
    bas, haut = 1, partie.maximum
    while True:
        milieu = (bas + haut) // 2
        reponse = partie.proposer(milieu)
        print(f"   essai {partie.essais} : {milieu:3d} → {reponse}")
        if reponse == "gagné":
            return partie.essais
        if reponse == "plus grand":
            bas = milieu + 1
        else:
            haut = milieu - 1


def jouer_humain() -> None:
    partie = Partie()
    print("J'ai choisi un nombre entre 1 et 100. À vous !")
    while True:
        texte = input("Votre proposition : ").strip()
        if not texte.isdigit():
            print("Entrez un nombre entier.")
            continue
        reponse = partie.proposer(int(texte))
        print(reponse.capitalize())
        if reponse == "gagné":
            print(f"Trouvé en {partie.essais} essais !")
            return


def main() -> int:
    if sys.argv[1:] == ["--jouer"]:
        jouer_humain()
        return 0
    if len(sys.argv) > 1:
        print("Usage : python3 devine_nombre.py [--jouer]", file=sys.stderr)
        return 1
    print(f"Essais maximum théoriques : {math.ceil(math.log2(101))}")
    random.seed(229)
    for secret in (random.randint(1, 100), 1, 100):
        print(f"Nombre secret : {secret}")
        print(f"   → trouvé en {joueur_automatique(Partie(secret=secret))} essais")
    return 0


if __name__ == "__main__":
    sys.exit(main())
