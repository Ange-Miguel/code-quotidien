# Jour 060 — Tri à bulles, sélection et insertion comparés

**Langue :** Java

## Le problème
Trier est l'une des opérations les plus fréquentes en informatique. Les trois tris « simples » sont faciles à écrire, mais leurs performances diffèrent selon les données (aléatoires, presque triées, inversées).

## La solution
On implémente les trois tris en comptant les comparaisons et les échanges, on les lance sur les mêmes jeux de données et on compare avec Arrays.sort. Le tri par insertion est très rapide sur des données presque triées, le tri par sélection fait toujours le même nombre de comparaisons.

## Ce qu'on apprend
Les algorithmes de tri en O(n²), la mesure expérimentale de la complexité, les cas favorables et défavorables, et la vérification d'un tri.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
3 000 données aléatoires :
   Bulles        4 498 290 comparaisons    2 280 197 échanges    10,12 ms
   Sélection     4 498 500 comparaisons        2 989 échanges    12,57 ms
   Insertion     2 283 183 comparaisons    2 280 197 échanges     7,08 ms
3 000 données presque triées :
   Bulles        4 384 019 comparaisons       64 061 échanges     3,69 ms
   Sélection     4 498 500 comparaisons           31 échanges     2,82 ms
   Insertion        67 060 comparaisons       64 061 échanges     0,13 ms
3 000 données inversées :
   Bulles        4 498 500 comparaisons    4 498 468 échanges     5,96 ms
   Sélection     4 498 500 comparaisons        1 520 échanges    17,86 ms
   Insertion     4 498 500 comparaisons    4 498 468 échanges     5,11 ms
```
