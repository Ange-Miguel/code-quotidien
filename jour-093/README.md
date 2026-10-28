# Jour 093 — Calendrier mensuel affiché en console

**Langue :** Java

## Le problème
Afficher le calendrier d'un mois (comme la commande cal sous Linux), avec les semaines commençant le lundi, en français, et en signalant les jours fériés.

## La solution
On détermine le jour de la semaine du 1er du mois. Pour savoir le calculer sans bibliothèque, on implémente la congruence de Zeller, que l'on vérifie avec java.time. On aligne ensuite les jours dans une grille de 7 colonnes et on marque d'un astérisque les jours fériés fixes du Bénin.

## Ce qu'on apprend
La congruence de Zeller, YearMonth, l'alignement en grille et la vérification d'un calcul maison par la bibliothèque standard.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
        octobre 2026
 Lu  Ma  Me  Je  Ve  Sa  Di
              1   2   3   4
  5   6   7   8   9  10  11
 12  13  14  15  16  17  18
 19  20  21  22  23  24  25
 26  27  28  29  30  31

       novembre 2026
 Lu  Ma  Me  Je  Ve  Sa  Di
                          1*
  2   3   4   5   6   7   8
  9  10  11  12  13  14  15
 16  17  18  19  20  21  22
 23  24  25  26  27  28  29
```
