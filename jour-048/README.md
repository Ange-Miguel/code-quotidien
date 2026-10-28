# Jour 048 — Inversion des mots d'une phrase

**Langue :** Java

## Le problème
Un classique des entretiens techniques : inverser l'ordre des mots d'une phrase (« le chat dort » → « dort chat le »), en gérant les espaces multiples, et le faire en place, sans tableau supplémentaire.

## La solution
Trois versions. (1) Simple avec split et une liste inversée. (2) Avec une pile (Deque). (3) En place sur un tableau de caractères : on inverse toute la chaîne, puis chaque mot individuellement. On vérifie que les trois donnent le même résultat. Bonus : inverser les lettres de chaque mot en gardant l'ordre des mots.

## Ce qu'on apprend
Les piles, l'inversion en place avec deux indices, la complexité mémoire O(1) et la comparaison de plusieurs implémentations.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
« le zem attend devant le campus »
   mots inversés   : « campus le devant attend zem le »
   lettres inversées : « el mez dnetta tnaved el supmac »
«   Calavi   est   ma   ville   »
   mots inversés   : « ville ma est Calavi »
   lettres inversées : « ivalaC tse am elliv »
« un »
   mots inversés   : « un »
   lettres inversées : « nu »
```
