"""Jour 044 — Calcul de surface et de peinture nécessaire pour une pièce

Problème : avant d'aller chez le quincaillier, il faut savoir combien de pots
de peinture acheter. Trop peu oblige à revenir, trop coûte cher.
Solution : surface des murs = périmètre × hauteur, moins les ouvertures
(portes, fenêtres), plus éventuellement le plafond. On multiplie par le nombre
de couches, on divise par le rendement (m²/litre), on ajoute 10 % de marge,
puis on choisit la combinaison de pots (15 L, 5 L, 1 L) la moins chère qui
couvre le besoin.
On apprend : la géométrie appliquée, les dataclasses, l'arrondi au supérieur
et une recherche exhaustive de la meilleure combinaison d'achats.
"""
import math
import sys
from dataclasses import dataclass, field
from itertools import product

POTS = {15: 32_000, 5: 12_500, 1: 3_000}   # litres → prix FCFA (exemple)


@dataclass
class Piece:
    longueur: float
    largeur: float
    hauteur: float
    ouvertures: list[tuple[float, float]] = field(default_factory=list)
    plafond: bool = True

    def surface(self) -> float:
        murs = 2 * (self.longueur + self.largeur) * self.hauteur
        trous = sum(l * h for l, h in self.ouvertures)
        return murs - trous + (self.longueur * self.largeur if self.plafond else 0)


def meilleur_achat(litres: float) -> tuple[int, dict[int, int]]:
    meilleur = (math.inf, {})
    for n15, n5, n1 in product(range(4), range(6), range(6)):
        volume = 15 * n15 + 5 * n5 + n1
        if volume >= litres:
            prix = n15 * POTS[15] + n5 * POTS[5] + n1 * POTS[1]
            if prix < meilleur[0]:
                meilleur = (prix, {15: n15, 5: n5, 1: n1})
    return meilleur


def devis(nom: str, piece: Piece, couches: int = 2, rendement: float = 10.0) -> None:
    s = piece.surface()
    if s <= 0:
        raise ValueError("surface nulle ou négative")
    litres = s * couches / rendement * 1.10
    prix, pots = meilleur_achat(litres)
    if prix == math.inf:
        raise ValueError("pièce trop grande pour cet outil")
    detail = ", ".join(f"{n} × {v} L" for v, n in pots.items() if n)
    print(f"{nom} : {s:.1f} m² à peindre, {couches} couches → {litres:.1f} L (marge 10 % incluse)")
    print(f"   Achat conseillé : {detail} = {prix:,} FCFA\n".replace(",", " "))


def main() -> int:
    if len(sys.argv) == 1:
        devis("Chambre 3 × 4 m", Piece(4, 3, 2.8, [(0.9, 2.1), (1.2, 1.2)]))
        devis("Salon 5 × 6 m", Piece(6, 5, 3.0, [(0.9, 2.1), (1.5, 1.4), (1.5, 1.4)]))
        devis("Boutique (murs seuls)", Piece(8, 4, 3.2, [(2.5, 2.5)], plafond=False), couches=1)
        return 0
    try:
        longueur, largeur, hauteur = (float(v) for v in sys.argv[1:4])
        if min(longueur, largeur, hauteur) <= 0 or len(sys.argv) != 4:
            raise ValueError("dimensions positives attendues")
        devis("Votre pièce", Piece(longueur, largeur, hauteur))
    except ValueError as e:
        print(f"Erreur : {e}. Usage : python3 peinture.py <longueur> <largeur> <hauteur>", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
