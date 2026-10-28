# Jour 061 — Tri rapide (quicksort) sur des prix de produits

**Langue :** C

## Le problème
Trier des milliers de prix (catalogue, comparateur) avec un tri en O(n²) devient vite trop lent. Le tri rapide fait en moyenne O(n log n).

## La solution
On choisit un pivot (médiane de trois pour éviter le pire cas sur des données déjà triées), on partitionne le tableau (schéma de Hoare) en plus petits / plus grands que le pivot, puis on trie récursivement chaque partie, en récursant d'abord sur la plus petite pour limiter la pile. On compare le temps avec le qsort de la bibliothèque standard.

## Ce qu'on apprend
La stratégie « diviser pour régner », la partition de Hoare, le choix du pivot, la profondeur de récursion et la mesure de temps.

## Lancer
```bash
gcc -Wall -Wextra -o quicksort quicksort.c -lm
./quicksort
```

## Exemple de sortie
```
1000000 prix triés
  Notre tri rapide : 0.069 s
  qsort standard   : 0.113 s
  Moins cher : 100 FCFA | plus cher : 1000095 FCFA | médian : 499450 FCFA
  Tri d'un tableau déjà trié : 0.015 s (grâce à la médiane de trois)
```
