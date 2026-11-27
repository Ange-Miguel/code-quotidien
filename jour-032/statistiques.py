"""Jour 032 — Statistiques d'une série : moyenne, médiane, écart-type

Problème : un relevé de ventes, de températures ou de notes ne parle pas tout
seul. On veut savoir quelle est la valeur typique et à quel point les valeurs
sont dispersées.
Solution : on recode nous-mêmes moyenne, médiane, mode, variance, écart-type
et quartiles (sans bibliothèque), puis on vérifie nos résultats avec le module
statistics. Un histogramme texte montre la forme de la distribution.
On apprend : les formules statistiques de base, la différence entre moyenne
et médiane face aux valeurs extrêmes, et la vérification croisée d'un calcul.
"""
import math
import statistics
import sys


def moyenne(x: list[float]) -> float:
    return sum(x) / len(x)

