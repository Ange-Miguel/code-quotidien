"""Jour 029 — Calcul de marge et prix de vente conseillé

Problème : beaucoup de petits vendeurs confondent marge et coefficient :
ajouter 30 % au prix de revient ne donne pas 30 % de marge sur le prix de vente.
Solution : on calcule les deux notions. Taux de marque (marge / prix de vente)
et taux de marge (marge / prix de revient). À partir d'un taux de marque
visé, prix de vente = revient / (1 − taux). On arrondit ensuite à un « prix
psychologique » se terminant par 900 ou 500 FCFA.
On apprend : la différence entre deux pourcentages qui se ressemblent, les
fonctions pures, l'arrondi personnalisé et les tests par assertions.
"""
import math
import sys


def prix_de_vente(revient: float, taux_marque: float) -> float:
    if not 0 <= taux_marque < 1:
        raise ValueError("taux de marque entre 0 et 99 %")
    return revient / (1 - taux_marque)


def prix_psychologique(prix: float) -> int:
    """Arrondit au-dessus vers un prix en ...500 ou ...900 (ou ...950 sous 1 000)."""
    if prix < 1000:
        return int(math.ceil(prix / 50) * 50)
    millier = int(prix // 1000) * 1000
    for fin in (500, 900, 1500):
        if millier + fin >= prix:
            return millier + fin
    return millier + 1900


def analyse(revient: float, vente: float) -> dict:
    marge = vente - revient
    return {"marge": marge, "taux_marque": marge / vente, "taux_marge": marge / revient,
            "coefficient": vente / revient}


def rapport(nom: str, revient: float, taux_marque: float) -> None:
    brut = prix_de_vente(revient, taux_marque)
    vente = prix_psychologique(brut)
    a = analyse(revient, vente)
    print(f"{nom} — revient {revient:,.0f} FCFA, taux de marque visé {taux_marque:.0%}".replace(",", " "))
    print(f"   prix calculé {brut:,.0f} → prix affiché {vente:,} FCFA".replace(",", " "))
    print(f"   marge {a['marge']:,.0f} FCFA | taux de marque {a['taux_marque']:.1%} | "
          f"taux de marge {a['taux_marge']:.1%} | coefficient ×{a['coefficient']:.2f}\n".replace(",", " "))


def main() -> int:
    assert math.isclose(prix_de_vente(700, 0.30), 1000)
    assert prix_psychologique(4120) == 4500 and prix_psychologique(4600) == 4900
    if len(sys.argv) == 1:
        rapport("Écouteurs Bluetooth", 2850, 0.40)
        rapport("Montre connectée", 8100, 0.35)
        rapport("Lampe solaire", 4600, 0.30)
        return 0
    try:
        rapport("Produit", float(sys.argv[1]), float(sys.argv[2]) / 100)
    except (ValueError, IndexError) as e:
        print(f"Erreur : {e}. Usage : python3 marge.py <prix de revient> <taux de marque %>", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
