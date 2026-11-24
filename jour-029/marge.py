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

