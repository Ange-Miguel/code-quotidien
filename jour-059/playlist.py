"""Jour 059 — Liste chaînée : gestion d'une playlist musicale

Problème : une playlist doit permettre d'ajouter un titre à la fin ou juste
après le morceau en cours, de supprimer, de passer au suivant ou au précédent,
et de boucler. Insérer au milieu d'un tableau oblige à tout décaler.
Solution : une liste doublement chaînée circulaire : chaque nœud connaît son
précédent et son suivant. Insérer ou supprimer devient une opération en temps
constant une fois le nœud trouvé. On implémente aussi l'itération (pour …
in) et le mélange.
On apprend : les listes chaînées, les pointeurs (références) prev/next, les
méthodes spéciales __iter__ et __len__, et le coût des opérations.
"""
import random
import sys


class Noeud:
    def __init__(self, titre: str, artiste: str, duree: int):
        self.titre, self.artiste, self.duree = titre, artiste, duree
        self.prec = self.suiv = self

    def __str__(self) -> str:
        return f"{self.titre} — {self.artiste} ({self.duree // 60}:{self.duree % 60:02d})"


class Playlist:
    def __init__(self):
        self.courant = None
        self.taille = 0

    def __len__(self):
        return self.taille

    def __iter__(self):
        if not self.courant:
            return
        n = self.courant
        for _ in range(self.taille):
            yield n
            n = n.suiv

    def _inserer_apres(self, ref: Noeud, n: Noeud) -> None:
        n.prec, n.suiv = ref, ref.suiv
        ref.suiv.prec = n
        ref.suiv = n
        self.taille += 1

    def ajouter_fin(self, *args) -> None:
        n = Noeud(*args)
        if not self.courant:
            self.courant, self.taille = n, 1
        else:
            self._inserer_apres(self.courant.prec, n)

    def jouer_ensuite(self, *args) -> None:
        if not self.courant:
            self.ajouter_fin(*args)
        else:
            self._inserer_apres(self.courant, Noeud(*args))

    def supprimer(self, titre: str) -> bool:
        for n in self:
            if n.titre.lower() == titre.lower():
                if self.taille == 1:
                    self.courant = None
                else:
                    n.prec.suiv, n.suiv.prec = n.suiv, n.prec
                    if n is self.courant:
                        self.courant = n.suiv
                self.taille -= 1
                return True
        return False

    def suivant(self) -> Noeud:
        self.courant = self.courant.suiv
        return self.courant

    def precedent(self) -> Noeud:
        self.courant = self.courant.prec
        return self.courant

    def melanger(self, rng: random.Random) -> None:
        morceaux = [(n.titre, n.artiste, n.duree) for n in self]
        rng.shuffle(morceaux)
        self.courant, self.taille = None, 0
        for m in morceaux:
            self.ajouter_fin(*m)

    def afficher(self, titre: str) -> None:
        total = sum(n.duree for n in self)
        print(f"{titre} ({len(self)} titres, {total // 60} min {total % 60:02d} s)")
        for i, n in enumerate(self):
            print(f"   {'▶' if i == 0 else ' '} {n}")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    p = Playlist()
    for m in [("Morceau A", "Artiste 1", 214), ("Morceau B", "Artiste 2", 187),
              ("Morceau C", "Artiste 3", 245), ("Morceau D", "Artiste 1", 201)]:
        p.ajouter_fin(*m)
    p.afficher("Playlist")
    print("\nSuivant :", p.suivant())
    p.jouer_ensuite("Morceau E", "Artiste 4", 230)
    print("Ajout de « Morceau E » juste après le titre en cours")
    print("Suivant :", p.suivant())
    print("Précédent :", p.precedent())
    print("Suppression de « Morceau C » :", p.supprimer("morceau c"))
    p.afficher("\nPlaylist actuelle")
    p.melanger(random.Random(59))
    p.afficher("\nAprès mélange")
    return 0


if __name__ == "__main__":
    sys.exit(main())
