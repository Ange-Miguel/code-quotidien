"""Jour 077 — Problème des N reines

Problème : placer N reines sur un échiquier N × N sans qu'aucune ne puisse en
prendre une autre (ni même ligne, ni même colonne, ni même diagonale). C'est
un problème classique de contraintes, proche de l'affectation de fréquences
radio ou de créneaux sans conflit.
Solution : backtracking ligne par ligne. On place une reine par ligne et on
mémorise les colonnes et les deux familles de diagonales déjà occupées
(ligne − colonne et ligne + colonne) dans des ensembles, ce qui rend le test
de conflit instantané. On compte toutes les solutions et on affiche la
première. Les comptes connus (8 reines : 92 solutions) servent de test.
On apprend : le backtracking avec ensembles, les invariants des diagonales,
les générateurs (yield) et la croissance du nombre de solutions.
"""
import sys
import time


def solutions(n: int):
    colonnes, diag1, diag2 = set(), set(), set()
    placement = []

    def placer(ligne: int):
        if ligne == n:
            yield list(placement)
            return
        for c in range(n):
            if c in colonnes or ligne - c in diag1 or ligne + c in diag2:
                continue
            colonnes.add(c); diag1.add(ligne - c); diag2.add(ligne + c); placement.append(c)
            yield from placer(ligne + 1)
            colonnes.remove(c); diag1.remove(ligne - c); diag2.remove(ligne + c); placement.pop()

    yield from placer(0)


def afficher(placement: list[int]) -> None:
    n = len(placement)
    for ligne, c in enumerate(placement):
        print("   " + " ".join("♛" if j == c else ("·" if (ligne + j) % 2 else "□") for j in range(n)))


def main() -> int:
    connus = {1: 1, 4: 2, 5: 10, 6: 4, 8: 92, 10: 724}
    if len(sys.argv) == 2:
        try:
            n = int(sys.argv[1])
            if not 1 <= n <= 13:
                raise ValueError
        except ValueError:
            print("Usage : python3 n_reines.py <N entre 1 et 13>", file=sys.stderr)
            return 1
        tailles = [n]
    elif len(sys.argv) == 1:
        tailles = [4, 6, 8, 10]
    else:
        print("Usage : python3 n_reines.py [N]", file=sys.stderr)
        return 1
    for n in tailles:
        t0 = time.perf_counter()
        premiere, total = None, 0
        for s in solutions(n):
            premiere = premiere or s
            total += 1
        duree = time.perf_counter() - t0
        if n in connus:
            assert total == connus[n], (n, total)
        print(f"{n} reines : {total} solution(s) en {duree * 1000:.0f} ms")
        if premiere and n <= 8:
            afficher(premiere)
        print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
