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


def mediane(x: list[float]) -> float:
    s, n = sorted(x), len(x)
    return s[n // 2] if n % 2 else (s[n // 2 - 1] + s[n // 2]) / 2


def ecart_type(x: list[float]) -> float:
    m = moyenne(x)
    return math.sqrt(sum((v - m) ** 2 for v in x) / len(x))


def quartiles(x: list[float]) -> tuple[float, float]:
    s, n = sorted(x), len(x)
    bas, haut = s[: n // 2], s[(n + 1) // 2:]
    return mediane(bas), mediane(haut)

