# Jour 016 — Table de multiplication formatée pour un enfant

**Langue :** C

## Le problème
Aider un petit frère ou une petite sœur à réviser ses tables, avec un affichage clair et un petit exercice d'entraînement.

## La solution
On affiche la table demandée (ou la grille complète de 1 à 10), puis on génère des questions dont la réponse est donnée à la fin pour l'auto-correction. Un générateur pseudo-aléatoire à graine fixe rend l'exercice reproductible.

## Ce qu'on apprend
Les boucles imbriquées, le formatage en colonnes avec printf et un générateur pseudo-aléatoire simple (congruence linéaire).

## Lancer
```bash
gcc -Wall -Wextra -o table_multiplication table_multiplication.c -lm
./table_multiplication
```

## Exemple de sortie
```
     |   1   2   3   4   5   6   7   8   9  10
-----+----------------------------------------
   1 |   1   2   3   4   5   6   7   8   9  10
   2 |   2   4   6   8  10  12  14  16  18  20
   3 |   3   6   9  12  15  18  21  24  27  30
   4 |   4   8  12  16  20  24  28  32  36  40
   5 |   5  10  15  20  25  30  35  40  45  50
   6 |   6  12  18  24  30  36  42  48  54  60
   7 |   7  14  21  28  35  42  49  56  63  70
   8 |   8  16  24  32  40  48  56  64  72  80
   9 |   9  18  27  36  45  54  63  72  81  90
  10 |  10  20  30  40  50  60  70  80  90 100

Entraînement (5 questions) :
  1) 7 × 8 = ____
```
