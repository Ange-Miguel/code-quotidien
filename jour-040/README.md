# Jour 040 — Simulateur de lancer de dés et statistiques

**Langue :** C

## Le problème
Au jeu de société, on se demande si « 7 sort vraiment plus souvent » avec deux dés. Simuler des milliers de lancers permet de vérifier la théorie des probabilités par l'expérience.

## La solution
Un générateur pseudo-aléatoire xorshift32 (rapide, reproductible avec une graine) lance deux dés N fois. On compare la fréquence observée de chaque somme (2 à 12) à la probabilité théorique (6 − |s − 7|)/36, avec un histogramme et le test du khi-deux.

## Ce qu'on apprend
Les générateurs pseudo-aléatoires, la loi des grands nombres, les tableaux de compteurs et le test du khi-deux.

## Lancer
```bash
gcc -Wall -Wextra -o des des.c -lm
./des
```

## Exemple de sortie
```
100 lancers de deux dés
Somme  Observé   Théorie  Histogramme
    2    0.00 %    2.78 %  
    3    7.00 %    5.56 %  #####################
    4   12.00 %    8.33 %  ####################################
    5   12.00 %   11.11 %  ####################################
    6   15.00 %   13.89 %  #############################################
    7   17.00 %   16.67 %  ###################################################
    8   10.00 %   13.89 %  ##############################
    9   13.00 %   11.11 %  #######################################
   10   10.00 %    8.33 %  ##############################
   11    2.00 %    5.56 %  ######
   12    2.00 %    2.78 %  ######
Khi-deux = 9.17 → conforme à la théorie (dés équilibrés)

```
