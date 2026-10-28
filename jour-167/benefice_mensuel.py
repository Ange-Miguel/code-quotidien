"""Jour 167 — Calcul des charges et du bénéfice mensuel d'une petite entreprise

Problème : beaucoup de petits commerçants regardent seulement l'argent en
caisse. Ils ne distinguent pas chiffre d'affaires, marge, charges fixes et
bénéfice, et ne savent pas à partir de quel volume de ventes ils gagnent
vraiment de l'argent (seuil de rentabilité).
Solution : à partir des ventes du mois (avec le coût d'achat des produits
vendus), on calcule la marge brute, on retire les charges fixes (loyer,
salaire, internet, électricité) et variables (commissions Mobile Money,
livraisons), puis on obtient le résultat. Le seuil de rentabilité = charges
fixes / taux de marge sur coûts variables. On montre l'évolution sur 3 mois
et l'effet d'une hausse de 10 % des ventes. Chiffres d'exemple.
On apprend : les notions de base de gestion (CA, marge, charges, résultat,
seuil de rentabilité), les dataclasses et la présentation d'un compte de
résultat simplifié.
"""
import sys
from dataclasses import dataclass


@dataclass
class Mois:
    nom: str
    chiffre_affaires: int
    cout_achats: int
    nb_livraisons: int

    def charges_variables(self) -> int:
        return round(self.chiffre_affaires * 0.01) + self.nb_livraisons * 1000   # 1 % Mobile Money + livreurs


CHARGES_FIXES = {"loyer boutique": 60_000, "salaire vendeur": 50_000, "internet + téléphone": 15_000,
                 "électricité": 12_000, "publicité réseaux sociaux": 20_000}


def fcfa(x: float) -> str:
    return f"{x:>12,.0f}".replace(",", " ")


def resultat(m: Mois, afficher: bool = True) -> float:
    marge_brute = m.chiffre_affaires - m.cout_achats
    variables = m.charges_variables()
    fixes = sum(CHARGES_FIXES.values())
    res = marge_brute - variables - fixes
    taux_mcv = (marge_brute - variables) / m.chiffre_affaires
    seuil = fixes / taux_mcv if taux_mcv > 0 else float("inf")
    if afficher:
        print(f"{m.nom}")
        print(f"   Chiffre d'affaires         {fcfa(m.chiffre_affaires)}")
        print(f"   − coût des produits vendus {fcfa(m.cout_achats)}")
        print(f"   = marge brute              {fcfa(marge_brute)}  ({marge_brute / m.chiffre_affaires:.0%} du CA)")
        print(f"   − charges variables        {fcfa(variables)}")
        print(f"   − charges fixes            {fcfa(fixes)}")
        print(f"   = RÉSULTAT                 {fcfa(res)}  {'bénéfice' if res >= 0 else 'PERTE'}")
        print(f"   Seuil de rentabilité : {fcfa(seuil).strip()} FCFA de ventes ({'atteint' if m.chiffre_affaires >= seuil else 'non atteint'})\n")
    return res


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    print("Charges fixes mensuelles : " + ", ".join(f"{k} {v:,}".replace(",", " ") for k, v in CHARGES_FIXES.items()) + "\n")
    mois = [Mois("Août 2026", 520_000, 330_000, 40), Mois("Septembre 2026", 610_000, 380_000, 52),
            Mois("Octobre 2026", 740_000, 455_000, 61)]
    total = sum(resultat(m) for m in mois)
    print(f"Résultat cumulé sur 3 mois : {fcfa(total).strip()} FCFA")
    m = mois[-1]
    plus = Mois("+10 % de ventes", round(m.chiffre_affaires * 1.1), round(m.cout_achats * 1.1), round(m.nb_livraisons * 1.1))
    gain = resultat(plus, afficher=False) - resultat(m, afficher=False)
    print(f"Si les ventes d'octobre augmentaient de 10 %, le résultat progresserait de {fcfa(gain).strip()} FCFA "
          f"(+{gain / resultat(m, afficher=False):.0%}) : c'est l'effet de levier des charges fixes.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
