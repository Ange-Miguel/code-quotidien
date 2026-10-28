"""Jour 092 — Suivi de remboursement de dettes entre amis

Problème : entre amis ou colocataires, on avance souvent de l'argent : l'un
paie le taxi, l'autre le repas, un troisième le crédit internet. Au bout d'un
mois, plus personne ne sait qui doit combien à qui, et il faudrait des
dizaines de virements Mobile Money pour tout régler.
Solution : on enregistre chaque dépense (qui a payé, pour qui). On calcule le
solde net de chacun (ce qu'il a avancé moins ce qu'il a consommé). Puis un
algorithme glouton fait payer le plus gros débiteur au plus gros créancier,
répète, et aboutit à au plus n − 1 transferts.
On apprend : les soldes nets, les dictionnaires, le tri, la simplification de
transactions et les arrondis en FCFA.
"""
import sys
from collections import defaultdict


def soldes(depenses: list[tuple[str, int, list[str]]]) -> dict[str, int]:
    solde = defaultdict(int)
    for payeur, montant, beneficiaires in depenses:
        if montant <= 0 or not beneficiaires:
            raise ValueError("dépense invalide")
        part, reste = divmod(montant, len(beneficiaires))
        solde[payeur] += montant
        for i, b in enumerate(beneficiaires):
            solde[b] -= part + (1 if i < reste else 0)    # répartit les FCFA restants
    return dict(solde)


def simplifier(solde: dict[str, int]) -> list[tuple[str, str, int]]:
    debiteurs = sorted([(-v, p) for p, v in solde.items() if v < 0], reverse=True)
    creanciers = sorted([(v, p) for p, v in solde.items() if v > 0], reverse=True)
    transferts = []
    i = j = 0
    debiteurs = [list(x) for x in debiteurs]
    creanciers = [list(x) for x in creanciers]
    while i < len(debiteurs) and j < len(creanciers):
        montant = min(debiteurs[i][0], creanciers[j][0])
        transferts.append((debiteurs[i][1], creanciers[j][1], montant))
        debiteurs[i][0] -= montant
        creanciers[j][0] -= montant
        if debiteurs[i][0] == 0:
            i += 1
        if creanciers[j][0] == 0:
            j += 1
    return transferts


def main() -> int:
    tous = ["Ange", "Kofi", "Awa", "Serge"]
    depenses = [
        ("Ange", 12000, tous),                    # courses de la semaine
        ("Kofi", 3000, ["Kofi", "Awa"]),          # zem
        ("Awa", 8000, tous),                      # crédit internet partagé
        ("Serge", 15000, ["Serge", "Ange", "Kofi"]),  # repas au maquis
        ("Ange", 2500, ["Awa"]),                  # avance à Awa
        ("Kofi", 1000, tous),                     # eau
    ]
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    print("Dépenses :")
    for payeur, montant, pour in depenses:
        print(f"   {payeur:6} a payé {montant:>6} FCFA pour {', '.join(pour)}")
    s = soldes(depenses)
    assert sum(s.values()) == 0
    print("\nSoldes nets (positif = on lui doit) :")
    for p, v in sorted(s.items(), key=lambda x: -x[1]):
        print(f"   {p:6} {v:+7d} FCFA")
    nb_dettes_brutes = sum(len([b for b in pour if b != payeur]) for payeur, _, pour in depenses)
    t = simplifier(s)
    print(f"\nRemboursements simplifiés ({len(t)} transferts au lieu de {nb_dettes_brutes}) :")
    for de, a, m in t:
        print(f"   {de:6} → {a:6} : {m} FCFA")
    return 0


if __name__ == "__main__":
    sys.exit(main())
