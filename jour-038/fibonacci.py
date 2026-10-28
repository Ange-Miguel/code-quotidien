"""Jour 038 — Suite de Fibonacci et croissance d'une population

Problème : comment évolue une population qui se reproduit (le célèbre problème
des lapins de Fibonacci), et pourquoi certaines méthodes de calcul deviennent
inutilisables quand n grandit ?
Solution : on compare trois façons de calculer F(n) : récursive naïve
(exponentielle), récursive avec mémoïsation, et itérative. On mesure le nombre
d'appels et le temps. On montre que F(n+1)/F(n) tend vers le nombre d'or, puis
on simule un élevage de poulets avec un taux de mortalité.
On apprend : la récursivité, la mémoïsation (functools.lru_cache), la
complexité exponentielle contre linéaire et la convergence d'une suite.
"""
import sys
import time
from functools import lru_cache

appels = 0


def fib_naif(n: int) -> int:
    global appels
    appels += 1
    return n if n < 2 else fib_naif(n - 1) + fib_naif(n - 2)


@lru_cache(maxsize=None)
def fib_memo(n: int) -> int:
    return n if n < 2 else fib_memo(n - 1) + fib_memo(n - 2)


def fib_iter(n: int) -> int:
    a, b = 0, 1
    for _ in range(n):
        a, b = b, a + b
    return a


def comparer(n: int) -> None:
    global appels
    appels = 0
    t = time.perf_counter()
    r = fib_naif(n)
    t_naif = time.perf_counter() - t
    t = time.perf_counter()
    assert fib_memo(n) == fib_iter(n) == r
    t_rapide = time.perf_counter() - t
    print(f"F({n}) = {r} | naïf : {appels:,} appels, {t_naif * 1000:.1f} ms | "
          f"rapide : {t_rapide * 1000:.3f} ms".replace(",", " "))


def elevage(couples: int, mois: int, mortalite: float) -> None:
    """Chaque couple adulte donne un nouveau couple par mois ; une part meurt chaque mois."""
    jeunes, adultes = couples, 0
    print(f"\nÉlevage : {couples} couple(s) au départ, mortalité {mortalite:.0%}/mois")
    for m in range(1, mois + 1):
        jeunes, adultes = adultes, adultes + jeunes
        adultes = round(adultes * (1 - mortalite))
        if m % 3 == 0:
            print(f"   mois {m:2d} : {jeunes + adultes:6d} couples")


def main() -> int:
    if len(sys.argv) == 2:
        try:
            n = int(sys.argv[1])
            if not 0 <= n <= 10000:
                raise ValueError
        except ValueError:
            print("Usage : python3 fibonacci.py <n entre 0 et 10000>", file=sys.stderr)
            return 1
        print(f"F({n}) = {fib_iter(n)}")
        return 0
    for n in (10, 20, 25):
        comparer(n)
    print("\nRapport F(n+1)/F(n) → nombre d'or φ ≈ 1,6180339887")
    for n in (5, 10, 20, 40):
        print(f"   n = {n:2d} : {fib_iter(n + 1) / fib_iter(n):.10f}")
    elevage(1, 24, 0.0)
    elevage(1, 24, 0.05)
    return 0


if __name__ == "__main__":
    sys.exit(main())
