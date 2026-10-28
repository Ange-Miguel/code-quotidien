"""Jour 062 — Recherche dichotomique dans un catalogue trié

Problème : dans un catalogue de 100 000 références triées, chercher un
produit en lisant tout un par un est lent. Comment trouver une référence, ou
tous les produits dans une fourchette de prix, en quelques étapes ?
Solution : la recherche dichotomique coupe l'intervalle en deux à chaque
comparaison : 17 étapes suffisent pour 100 000 éléments. On écrit la version
de base, puis les variantes « première position ≥ x » et « dernière position
≤ x », qui permettent de trouver une plage de prix. On compare avec le module
bisect et avec une recherche linéaire.
On apprend : la recherche dichotomique et ses pièges (bornes, boucle infinie),
les variantes lower/upper bound et le module bisect.
"""
import bisect
import random
import sys
import time


def chercher(t: list, x) -> int:
    """Indice de x dans t trié, ou -1. Compte les étapes."""
    bas, haut, etapes = 0, len(t) - 1, 0
    while bas <= haut:
        etapes += 1
        milieu = (bas + haut) // 2
        if t[milieu] == x:
            print(f"   trouvé en {etapes} étape(s)")
            return milieu
        if t[milieu] < x:
            bas = milieu + 1
        else:
            haut = milieu - 1
    print(f"   absent (vérifié en {etapes} étape(s))")
    return -1


def premier_superieur_ou_egal(t: list, x) -> int:
    bas, haut = 0, len(t)
    while bas < haut:
        milieu = (bas + haut) // 2
        if t[milieu] < x:
            bas = milieu + 1
        else:
            haut = milieu
    return bas


def dernier_inferieur_ou_egal(t: list, x) -> int:
    bas, haut = 0, len(t)
    while bas < haut:
        milieu = (bas + haut) // 2
        if t[milieu] <= x:
            bas = milieu + 1
        else:
            haut = milieu
    return bas - 1


def main() -> int:
    rng = random.Random(62)
    prix = sorted(rng.randrange(500, 500_000, 50) for _ in range(100_000))
    if len(sys.argv) == 3:
        try:
            mini, maxi = int(sys.argv[1]), int(sys.argv[2])
            if mini > maxi:
                raise ValueError
        except ValueError:
            print("Usage : python3 recherche_dichotomique.py <prix min> <prix max>", file=sys.stderr)
            return 1
    elif len(sys.argv) == 1:
        mini, maxi = 15_000, 15_500
    else:
        print("Usage : python3 recherche_dichotomique.py [<prix min> <prix max>]", file=sys.stderr)
        return 1

    print(f"Catalogue de {len(prix):,} prix triés".replace(",", " "))
    for cible in (prix[31_415], 123_457, prix[-1]):
        print(f"Recherche de {cible} FCFA :")
        i = chercher(prix, cible)
        assert (i >= 0) == (cible in prix)

    a, b = premier_superieur_ou_egal(prix, mini), dernier_inferieur_ou_egal(prix, maxi)
    assert a == bisect.bisect_left(prix, mini) and b == bisect.bisect_right(prix, maxi) - 1
    print(f"\nProduits entre {mini} et {maxi} FCFA : {max(0, b - a + 1)} (indices {a} à {b})")

    t0 = time.perf_counter()
    for _ in range(200):
        premier_superieur_ou_egal(prix, rng.randrange(500, 500_000))
    t1 = time.perf_counter()
    for _ in range(200):
        next((i for i, p in enumerate(prix) if p >= rng.randrange(500, 500_000)), None)
    t2 = time.perf_counter()
    print(f"200 recherches : dichotomique {1000 * (t1 - t0):.2f} ms | linéaire {1000 * (t2 - t1):.0f} ms")
    return 0


if __name__ == "__main__":
    sys.exit(main())
