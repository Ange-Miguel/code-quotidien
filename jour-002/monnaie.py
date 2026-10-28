"""Jour 002 — Calculateur de monnaie à rendre (pièces et billets FCFA)

Problème : un commerçant doit rendre la monnaie vite et sans erreur, avec le
moins de billets et de pièces possible.
Solution : algorithme glouton : on prend toujours la plus grande coupure FCFA
possible (10 000, 5 000, 2 000, 1 000, 500, 250, 200, 100, 50, 25, 10, 5) jusqu'à
épuiser le montant. Avec ces coupures, le glouton donne la solution optimale.
On apprend : les listes, la division entière (//) et le modulo (%), les
algorithmes gloutons et la validation des entrées.
"""
import sys

COUPURES = [10000, 5000, 2000, 1000, 500, 250, 200, 100, 50, 25, 10, 5]


def rendre(prix: int, paye: int) -> dict[int, int]:
    """Retourne {coupure: quantité} pour la monnaie à rendre."""
    if paye < prix:
        raise ValueError(f"il manque {prix - paye} FCFA")
    reste = paye - prix
    if reste % 5:
        raise ValueError("la plus petite pièce est 5 FCFA : montant non rendable")
    resultat = {}
    for c in COUPURES:
        if reste >= c:
            resultat[c] = reste // c
            reste %= c
    return resultat


def afficher(prix: int, paye: int) -> None:
    print(f"Prix : {prix} FCFA | Payé : {paye} FCFA | À rendre : {paye - prix} FCFA")
    detail = rendre(prix, paye)
    if not detail:
        print("  Compte juste, rien à rendre.")
    for c, n in detail.items():
        nature = "billet" if c > 500 else "pièce"
        print(f"  {n} × {nature}{'s' if n > 1 else ''} de {c} FCFA")
    print(f"  Total : {sum(detail.values())} coupure(s)\n")


def main() -> int:
    if len(sys.argv) == 3:
        try:
            prix, paye = int(sys.argv[1]), int(sys.argv[2])
            if prix < 0 or paye < 0:
                raise ValueError("montants négatifs interdits")
            afficher(prix, paye)
        except ValueError as e:
            print(f"Erreur : {e}", file=sys.stderr)
            return 1
        return 0
    if len(sys.argv) != 1:
        print("Usage : python3 monnaie.py <prix> <montant_payé>", file=sys.stderr)
        return 1
    for prix, paye in [(3750, 10000), (1225, 2000), (500, 500)]:
        afficher(prix, paye)
    return 0


if __name__ == "__main__":
    sys.exit(main())
