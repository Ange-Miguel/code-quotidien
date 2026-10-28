# Jour 024 — Formateur de montants avec séparateurs de milliers

**Langue :** Java

## Le problème
« 12500000 » est difficile à lire sur une facture ou une affiche ; « 12 500 000 FCFA » ou « 12,5 M FCFA » l'est beaucoup plus.

## La solution
On écrit nous-mêmes l'insertion d'un espace tous les trois chiffres (en partant de la droite), on gère les nombres négatifs, puis on propose un format court (k, M, Md) pour les tableaux de bord. On compare avec NumberFormat de la bibliothèque standard.

## Ce qu'on apprend
La manipulation de chaînes avec StringBuilder, le parcours à l'envers, les formats compacts et la localisation avec NumberFormat.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
0 → 0 FCFA             | court : 0          | NumberFormat : 0
            950 → 950 FCFA           | court : 950        | NumberFormat : 950
          12500 → 12 500 FCFA        | court : 12,5 k     | NumberFormat : 12 500
         250000 → 250 000 FCFA       | court : 250,0 k    | NumberFormat : 250 000
       12500000 → 12 500 000 FCFA    | court : 12,5 M     | NumberFormat : 12 500 000
       -3750000 → -3 750 000 FCFA    | court : -3,8 M     | NumberFormat : -3 750 000
     2150000000 → 2 150 000 000 FCFA | court : 2,2 Md     | NumberFormat : 2 150 000 000
```
