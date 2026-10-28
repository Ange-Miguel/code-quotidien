"""Jour 185 — Cache LRU pour accélérer des requêtes

Problème : une application interroge souvent les mêmes données lentes à
obtenir : prix d'un produit en base, météo d'une ville par une API payante,
résultat d'un calcul lourd. Garder les derniers résultats en mémoire évite de
refaire le travail, mais la mémoire est limitée : que garder ?
Solution : le cache LRU (Least Recently Used) garde les N derniers éléments
utilisés et évince le plus anciennement utilisé quand il est plein. On
l'implémente avec un dictionnaire ordonné (OrderedDict) : move_to_end à
chaque accès, popitem(last=False) pour évincer, soit O(1) partout. On ajoute
une durée de vie (TTL) pour ne pas servir des données périmées, et on
mesure le taux de succès selon la taille du cache sur un trafic réaliste où
quelques produits sont très demandés. On compare avec functools.lru_cache.
On apprend : le principe du cache, la politique LRU, OrderedDict, la durée
de vie des données, et l'effet de la taille du cache sur les performances.
"""
import functools
import random
import sys
import time
from collections import OrderedDict


class CacheLRU:
    def __init__(self, capacite: int, ttl: float | None = None, horloge=time.monotonic):
        if capacite < 1:
            raise ValueError("capacité ≥ 1")
        self.capacite, self.ttl, self.horloge = capacite, ttl, horloge
        self.donnees: OrderedDict = OrderedDict()
        self.succes = self.echecs = 0

    def obtenir(self, cle, calculer):
        maintenant = self.horloge()
        if cle in self.donnees:
            valeur, date = self.donnees[cle]
            if self.ttl is None or maintenant - date < self.ttl:
                self.donnees.move_to_end(cle)
                self.succes += 1
                return valeur
            del self.donnees[cle]                       # périmé
        self.echecs += 1
        valeur = calculer(cle)
        self.donnees[cle] = (valeur, maintenant)
        if len(self.donnees) > self.capacite:
            self.donnees.popitem(last=False)           # évince le moins récemment utilisé
        return valeur

    def taux(self) -> float:
        return self.succes / max(1, self.succes + self.echecs)


appels_lents = 0


def prix_en_base(ref: int) -> int:
    global appels_lents
    appels_lents += 1
    return 1000 + (ref * 7919) % 50_000


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    rng = random.Random(185)
    poids = [1 / (k + 1) ** 1.1 for k in range(2000)]           # loi de Zipf : quelques produits très demandés
    requetes = rng.choices(range(2000), weights=poids, k=20_000)
    print("20 000 requêtes sur 2 000 produits (les produits populaires reviennent souvent)\n")
    print(f"{'Taille du cache':>15} {'Succès':>8} {'Appels à la base':>17}")
    for taille in (10, 50, 200, 1000):
        global appels_lents
        appels_lents = 0
        c = CacheLRU(taille)
        for r in requetes:
            c.obtenir(r, prix_en_base)
        print(f"{taille:15d} {c.taux():8.1%} {appels_lents:17,d}".replace(",", " "))
    t = [0.0]
    c = CacheLRU(3, ttl=60, horloge=lambda: t[0])
    for cle, instant in [("A", 0), ("B", 1), ("A", 2), ("C", 3), ("D", 4), ("A", 5), ("B", 6), ("A", 70)]:
        t[0] = instant
        avant = c.succes
        c.obtenir(cle, lambda k: k.lower())
        print(f"t={instant:3d} s  demande {cle} → {'succès' if c.succes > avant else 'échec'}   cache : {list(c.donnees)}")
    lru = functools.lru_cache(maxsize=200)(prix_en_base)
    for r in requetes:
        lru(r)
    print(f"\nfunctools.lru_cache(200) : {lru.cache_info().hits / len(requetes):.1%} de succès (même principe, intégré à Python)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
