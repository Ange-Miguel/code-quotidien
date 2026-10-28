"""Jour 194 — Jeu du serpent (snake) en console

Problème : programmer un jeu d'action complet : un serpent se déplace sur une
grille, mange des fruits, grandit, et perd s'il touche un mur ou son propre
corps. Il faut gérer l'état du jeu, les règles, l'affichage et les entrées.
Solution : le corps du serpent est une file (deque) : à chaque pas, on ajoute
la nouvelle tête et on retire la queue, sauf s'il a mangé. Un ensemble
(set) des cases occupées rend le test de collision instantané. Le fruit
apparaît sur une case libre au hasard. En démo, une petite intelligence
artificielle joue seule : elle cherche le plus court chemin vers le fruit
par un parcours en largeur (BFS) et, si aucun chemin n'existe, choisit le
coup qui laisse le plus d'espace libre. Avec --jouer, on joue au clavier
(z q s d puis Entrée).
On apprend : la modélisation d'un jeu, deque et set, le parcours en largeur
pour trouver un chemin, et la boucle de jeu.
"""
import random
import sys
from collections import deque

LARGEUR, HAUTEUR = 16, 10
DIRECTIONS = {"z": (0, -1), "s": (0, 1), "q": (-1, 0), "d": (1, 0)}


class Jeu:
    def __init__(self, rng: random.Random):
        self.rng = rng
        self.corps = deque([(4, 5), (3, 5), (2, 5)])     # la tête est à gauche de la deque
        self.occupees = set(self.corps)
        self.direction = (1, 0)
        self.score, self.vivant = 0, True
        self.fruit = self.placer_fruit()

    def placer_fruit(self):
        libres = [(x, y) for x in range(LARGEUR) for y in range(HAUTEUR) if (x, y) not in self.occupees]
        return self.rng.choice(libres) if libres else None

    def pas(self) -> None:
        tx, ty = self.corps[0]
        tete = (tx + self.direction[0], ty + self.direction[1])
        mange = tete == self.fruit
        if not mange:
            self.occupees.discard(self.corps.pop())        # la queue avance
        if not (0 <= tete[0] < LARGEUR and 0 <= tete[1] < HAUTEUR) or tete in self.occupees:
            self.vivant = False
            return
        self.corps.appendleft(tete)
        self.occupees.add(tete)
        if mange:
            self.score += 1
            self.fruit = self.placer_fruit()

    def afficher(self) -> None:
        print("+" + "-" * LARGEUR + "+")
        for y in range(HAUTEUR):
            ligne = "".join("@" if (x, y) == self.corps[0] else "o" if (x, y) in self.occupees else "*" if (x, y) == self.fruit else " "
                            for x in range(LARGEUR))
            print("|" + ligne + "|")
        print("+" + "-" * LARGEUR + f"+  score {self.score}")


def voisins_libres(jeu: Jeu, case, occupees) -> list:
    return [(case[0] + dx, case[1] + dy) for dx, dy in DIRECTIONS.values()
            if 0 <= case[0] + dx < LARGEUR and 0 <= case[1] + dy < HAUTEUR and (case[0] + dx, case[1] + dy) not in occupees]


def ia(jeu: Jeu) -> None:
    tete, obstacles = jeu.corps[0], jeu.occupees - {jeu.corps[-1]}
    precedent, file = {tete: None}, deque([tete])
    while file:                                            # BFS vers le fruit
        c = file.popleft()
        if c == jeu.fruit:
            while precedent[c] != tete:
                c = precedent[c]
            jeu.direction = (c[0] - tete[0], c[1] - tete[1])
            return
        for v in voisins_libres(jeu, c, obstacles):
            if v not in precedent:
                precedent[v] = c
                file.append(v)
    options = voisins_libres(jeu, tete, obstacles)         # pas de chemin : survivre
    if options:
        meilleur = max(options, key=lambda v: len(voisins_libres(jeu, v, obstacles)))
        jeu.direction = (meilleur[0] - tete[0], meilleur[1] - tete[1])


def main() -> int:
    if sys.argv[1:] == ["--jouer"]:
        jeu = Jeu(random.Random())
        while jeu.vivant:
            jeu.afficher()
            touche = input("z/q/s/d puis Entrée (rien = tout droit) : ").strip().lower()[:1]
            if touche in DIRECTIONS and DIRECTIONS[touche] != (-jeu.direction[0], -jeu.direction[1]):
                jeu.direction = DIRECTIONS[touche]
            jeu.pas()
        print(f"Perdu ! Score : {jeu.score}")
        return 0
    if len(sys.argv) > 1:
        print("Usage : python3 serpent.py [--jouer]", file=sys.stderr)
        return 1
    jeu = Jeu(random.Random(194))
    for tour in range(2000):
        ia(jeu)
        jeu.pas()
        if not jeu.vivant or jeu.fruit is None:
            break
        if tour in (15, 120):
            print(f"Tour {tour} :")
            jeu.afficher()
    print(f"\nL'IA a mangé {jeu.score} fruits en {tour + 1} déplacements ({'grille remplie !' if jeu.vivant else 'puis s’est bloquée'}).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
