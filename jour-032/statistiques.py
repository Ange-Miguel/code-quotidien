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


def histogramme(x: list[float], classes: int = 5) -> None:
    mini, maxi = min(x), max(x)
    largeur = (maxi - mini) / classes or 1
    comptes = [0] * classes
    for v in x:
        comptes[min(int((v - mini) / largeur), classes - 1)] += 1
    for i, c in enumerate(comptes):
        a = mini + i * largeur
        print(f"   [{a:8.1f} ; {a + largeur:8.1f}[ {'█' * c} {c}")


def rapport(nom: str, x: list[float]) -> None:
    if len(x) < 2:
        raise ValueError("au moins deux valeurs nécessaires")
    q1, q3 = quartiles(x)
    print(f"{nom} ({len(x)} valeurs)")
    print(f"   moyenne {moyenne(x):.2f} | médiane {mediane(x):.2f} | mode {statistics.mode(x)}")
    print(f"   min {min(x)} | max {max(x)} | écart-type {ecart_type(x):.2f} | Q1 {q1} | Q3 {q3}")
    assert math.isclose(moyenne(x), statistics.mean(x))
    assert math.isclose(mediane(x), statistics.median(x))
    assert math.isclose(ecart_type(x), statistics.pstdev(x))
    histogramme(x)
    print()


def main() -> int:
    if len(sys.argv) == 1:
        ventes = [12, 15, 9, 22, 15, 18, 11, 15, 30, 14, 13, 16, 95, 17]
        rapport("Ventes journalières (unités)", ventes)
        print("Remarque : la journée à 95 tire la moyenne vers le haut, pas la médiane.")
        return 0
    try:
        rapport("Votre série", [float(v.replace(",", ".")) for v in sys.argv[1:]])
    except ValueError as e:
        print(f"Erreur : {e}. Usage : python3 statistiques.py v1 v2 v3 ...", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
