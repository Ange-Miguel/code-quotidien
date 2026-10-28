# Jour 036 — Détection des nombres premiers (crible d'Ératosthène)

**Langue :** Java

## Le problème
Les nombres premiers sont au cœur de la cryptographie (RSA) et des tables de hachage. Il faut savoir les trouver efficacement.

## La solution
Le crible d'Ératosthène marque comme composés tous les multiples de chaque nombre premier, en partant de son carré. Pour un test isolé, on divise seulement jusqu'à la racine carrée. On compare le temps des deux méthodes et on vérifie le nombre de premiers sous 1 000 000 (78 498).

## Ce qu'on apprend
Les tableaux de booléens, la complexité O(n log log n), BitSet, la mesure du temps d'exécution et l'optimisation par la racine.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Crible jusqu'à 1 000 000 : 78 498 nombres premiers en 8,0 ms
Test un par un : 78 498 premiers en 115,3 ms
Les 20 premiers : 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 
97 : premier
1 000 003 : premier
2 147 483 647 : premier
600 851 475 143 : composé
```
