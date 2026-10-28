"""Jour 041 — Pierre-feuille-ciseaux contre l'ordinateur

Problème : un jeu simple, mais un bon terrain pour une première « IA » : un
humain ne joue jamais vraiment au hasard, il a des habitudes qu'on peut
exploiter.
Solution : l'ordinateur mémorise ce que le joueur a joué après chacun de ses
coups précédents (chaîne de Markov d'ordre 1). Il prédit le coup le plus
probable et joue ce qui le bat. En démo, il affronte un joueur simulé qui a
une habitude (il répète souvent son coup) puis un joueur vraiment aléatoire.
On apprend : les dictionnaires imbriqués, Counter, une prédiction statistique
simple et l'importance du vrai hasard face à un adversaire qui apprend.
"""
import random
import sys
from collections import Counter, defaultdict

COUPS = ["pierre", "feuille", "ciseaux"]
BAT = {"pierre": "feuille", "feuille": "ciseaux", "ciseaux": "pierre"}  # BAT[x] bat x


class Ordinateur:
    def __init__(self, rng: random.Random):
        self.rng = rng
        self.transitions = defaultdict(Counter)
        self.precedent = None

    def jouer(self) -> str:
        historique = self.transitions.get(self.precedent)
        if not historique:
            return self.rng.choice(COUPS)
        prevu = historique.most_common(1)[0][0]
        return BAT[prevu]

    def apprendre(self, coup_joueur: str) -> None:
        if self.precedent is not None:
            self.transitions[self.precedent][coup_joueur] += 1
        self.precedent = coup_joueur


def resultat(a: str, b: str) -> int:
    """1 si a gagne, -1 si b gagne, 0 si égalité."""
    if a == b:
        return 0
    return 1 if BAT[b] == a else -1


def partie(nom: str, strategie, manches: int, rng: random.Random) -> None:
    ordi = Ordinateur(rng)
    score = Counter()
    dernier = rng.choice(COUPS)
    for _ in range(manches):
        coup = strategie(dernier)
        machine = ordi.jouer()
        score[resultat(machine, coup)] += 1
        ordi.apprendre(coup)
        dernier = coup
    print(f"{nom:28} → ordinateur {score[1]} | joueur {score[-1]} | égalités {score[0]}")


def main() -> int:
    if sys.argv[1:] == ["--jouer"]:
        ordi, rng = Ordinateur(random.Random()), None
        print("Tapez pierre, feuille ou ciseaux (q pour quitter).")
        while (coup := input("> ").strip().lower()) != "q":
            if coup not in COUPS:
                print("Coup inconnu.")
                continue
            machine = ordi.jouer()
            r = resultat(coup, machine)
            print(f"Ordinateur : {machine} → {['Égalité', 'Gagné !', 'Perdu'][r]}")
            ordi.apprendre(coup)
        return 0
    if len(sys.argv) > 1:
        print("Usage : python3 pierre_feuille_ciseaux.py [--jouer]", file=sys.stderr)
        return 1
    rng = random.Random(41)
    habitude = lambda dernier: dernier if rng.random() < 0.7 else rng.choice(COUPS)
    cycle = lambda dernier: BAT[dernier]
    hasard = lambda dernier: rng.choice(COUPS)
    print("200 manches contre trois types de joueurs :")
    partie("Joueur qui répète son coup", habitude, 200, rng)
    partie("Joueur qui tourne en cycle", cycle, 200, rng)
    partie("Joueur vraiment aléatoire", hasard, 200, rng)
    return 0


if __name__ == "__main__":
    sys.exit(main())
