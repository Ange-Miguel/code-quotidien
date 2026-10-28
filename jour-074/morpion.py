"""Jour 074 — Jeu du morpion à deux joueurs

Problème : programmer un jeu complet, même simple, oblige à bien représenter
l'état (la grille), vérifier les règles (coup valide, victoire, match nul) et
séparer la logique de l'affichage.
Solution : la grille est une liste de 9 cases. Les 8 lignes gagnantes sont
listées une fois pour toutes. Deux joueurs humains s'affrontent avec --jouer ;
en démo, deux joueurs automatiques jouent : l'un au hasard, l'autre avec une
stratégie simple (gagner si possible, sinon bloquer, sinon le centre, puis
les coins).
On apprend : la représentation d'un plateau, la détection de victoire, une
petite stratégie de jeu et la boucle principale d'un jeu au tour par tour.
"""
import random
import sys

LIGNES = [(0, 1, 2), (3, 4, 5), (6, 7, 8), (0, 3, 6), (1, 4, 7), (2, 5, 8), (0, 4, 8), (2, 4, 6)]


def afficher(g: list[str]) -> None:
    for l in range(3):
        print("   " + " | ".join(g[3 * l + c] or str(3 * l + c + 1) for c in range(3)))
        if l < 2:
            print("   --+---+--")


def gagnant(g: list[str]) -> str | None:
    for a, b, c in LIGNES:
        if g[a] and g[a] == g[b] == g[c]:
            return g[a]
    return None


def libres(g: list[str]) -> list[int]:
    return [i for i, v in enumerate(g) if not v]


def strategie(g: list[str], moi: str, rng: random.Random) -> int:
    adversaire = "O" if moi == "X" else "X"
    for symbole in (moi, adversaire):            # gagner, sinon bloquer
        for i in libres(g):
            g[i] = symbole
            fini = gagnant(g) == symbole
            g[i] = ""
            if fini:
                return i
    for i in [4, 0, 2, 6, 8]:
        if not g[i]:
            return i
    return rng.choice(libres(g))


def partie(joueurs: dict, rng: random.Random, verbeux: bool = True) -> str:
    g = [""] * 9
    tour = "X"
    while True:
        i = joueurs[tour](g, tour, rng)
        g[i] = tour
        if verbeux:
            print(f"{tour} joue en {i + 1}")
        if (v := gagnant(g)) or not libres(g):
            if verbeux:
                afficher(g)
            return v or "nul"
        tour = "O" if tour == "X" else "X"


def humain(g, tour, rng) -> int:
    afficher(g)
    while True:
        r = input(f"Joueur {tour}, case (1-9) : ").strip()
        if r.isdigit() and 1 <= int(r) <= 9 and not g[int(r) - 1]:
            return int(r) - 1
        print("Case invalide ou déjà prise.")


def main() -> int:
    rng = random.Random(74)
    if sys.argv[1:] == ["--jouer"]:
        r = partie({"X": humain, "O": humain}, rng)
        print("Match nul !" if r == "nul" else f"Victoire de {r} !")
        return 0
    if len(sys.argv) > 1:
        print("Usage : python3 morpion.py [--jouer]", file=sys.stderr)
        return 1
    hasard = lambda g, t, r: r.choice(libres(g))
    print("Partie de démonstration : X (stratégie) contre O (hasard)")
    print("Résultat :", partie({"X": strategie, "O": hasard}, rng))
    scores = {"X": 0, "O": 0, "nul": 0}
    for _ in range(500):
        scores[partie({"X": hasard, "O": strategie}, rng, verbeux=False)] += 1
    print(f"\n500 parties, X au hasard contre O stratège : {scores}")
    scores = {"X": 0, "O": 0, "nul": 0}
    for _ in range(100):
        scores[partie({"X": strategie, "O": strategie}, rng, verbeux=False)] += 1
    print(f"100 parties stratège contre stratège : {scores}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
