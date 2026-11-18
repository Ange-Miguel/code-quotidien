"""Jour 023 — Calculateur d'intérêts simples et composés

Problème : quand on épargne (compte, tontine améliorée, placement), on veut
savoir combien on aura dans quelques années et comprendre la différence entre
intérêts simples et intérêts composés.
Solution : intérêts simples : capital × (1 + taux × années). Intérêts composés :
capital × (1 + taux/n)^(n × années), avec n capitalisations par an. On ajoute un
versement mensuel régulier simulé mois par mois, et la « règle de 72 » pour
estimer le temps de doublement.
On apprend : les puissances, les simulations mois par mois, la comparaison de
formules et la mise en forme de tableaux.
"""
import sys


def simples(capital: float, taux: float, annees: int) -> float:
    return capital * (1 + taux * annees)


def composes(capital: float, taux: float, annees: int, n: int = 12) -> float:
    return capital * (1 + taux / n) ** (n * annees)


def avec_versements(capital: float, versement: float, taux: float, annees: int) -> float:
    solde = capital
    for _ in range(annees * 12):
        solde = solde * (1 + taux / 12) + versement
    return solde


def fcfa(x: float) -> str:
    return f"{x:,.0f}".replace(",", " ") + " FCFA"


def tableau(capital: float, taux: float, annees: int, versement: float) -> None:
    print(f"Capital {fcfa(capital)}, taux {taux * 100:.1f} %/an, versement {fcfa(versement)}/mois")
    print(f"{'Année':>5} {'Simples':>18} {'Composés':>18} {'+ versements':>20}")
    for a in range(1, annees + 1):
        print(f"{a:>5} {fcfa(simples(capital, taux, a)):>18} {fcfa(composes(capital, taux, a)):>18}"
              f" {fcfa(avec_versements(capital, versement, taux, a)):>20}")
    if taux > 0:
        print(f"Règle de 72 : le capital double en ~{72 / (taux * 100):.1f} ans à intérêts composés")
    total_verse = capital + versement * 12 * annees
    gain = avec_versements(capital, versement, taux, annees) - total_verse
    print(f"Total versé : {fcfa(total_verse)} → intérêts gagnés : {fcfa(gain)}\n")


def main() -> int:
    if len(sys.argv) == 1:
        tableau(500_000, 0.06, 5, 25_000)
        return 0
    try:
        capital, taux, annees = float(sys.argv[1]), float(sys.argv[2]) / 100, int(sys.argv[3])
        versement = float(sys.argv[4]) if len(sys.argv) > 4 else 0
        if capital < 0 or not 0 <= taux <= 1 or not 1 <= annees <= 50 or versement < 0:
            raise ValueError("valeurs hors limites")
        tableau(capital, taux, annees, versement)
    except (ValueError, IndexError) as e:
        print(f"Erreur : {e}. Usage : python3 epargne.py <capital> <taux %> <années> [versement mensuel]",
              file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
