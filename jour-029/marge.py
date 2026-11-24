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

