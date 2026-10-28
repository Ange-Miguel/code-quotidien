# Jour 185 — Cache LRU pour accélérer des requêtes

**Langue :** Python

## Le problème
Une application interroge souvent les mêmes données lentes à obtenir : prix d'un produit en base, météo d'une ville par une API payante, résultat d'un calcul lourd. Garder les derniers résultats en mémoire évite de refaire le travail, mais la mémoire est limitée : que garder ?

## La solution
Le cache LRU (Least Recently Used) garde les N derniers éléments utilisés et évince le plus anciennement utilisé quand il est plein. On l'implémente avec un dictionnaire ordonné (OrderedDict) : move_to_end à chaque accès, popitem(last=False) pour évincer, soit O(1) partout. On ajoute une durée de vie (TTL) pour ne pas servir des données périmées, et on mesure le taux de succès selon la taille du cache sur un trafic réaliste où quelques produits sont très demandés. On compare avec functools.lru_cache.

## Ce qu'on apprend
Le principe du cache, la politique LRU, OrderedDict, la durée de vie des données, et l'effet de la taille du cache sur les performances.

## Lancer
```bash
python3 cache_lru.py
```

## Exemple de sortie
```
20 000 requêtes sur 2 000 produits (les produits populaires reviennent souvent)

Taille du cache   Succès  Appels à la base
             10    27.5%            14 497
             50    51.8%             9 645
            200    70.8%             5 834
           1000    89.6%             2 089
t=  0 s  demande A → échec   cache : ['A']
t=  1 s  demande B → échec   cache : ['A', 'B']
t=  2 s  demande A → succès   cache : ['B', 'A']
t=  3 s  demande C → échec   cache : ['B', 'A', 'C']
t=  4 s  demande D → échec   cache : ['A', 'C', 'D']
t=  5 s  demande A → succès   cache : ['C', 'D', 'A']
t=  6 s  demande B → échec   cache : ['D', 'A', 'B']
t= 70 s  demande A → échec   cache : ['D', 'B', 'A']
```
