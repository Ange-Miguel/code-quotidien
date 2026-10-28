# Jour 157 — Détection de prix anormaux dans une liste de médicaments

**Langue :** C

## Le problème
Pour un même médicament, les prix relevés dans différentes pharmacies ou facturés par différents services peuvent varier. Certains écarts sont normaux, d'autres révèlent une erreur de saisie (un zéro de trop) ou une surfacturation. Comment les repérer automatiquement sans fixer à la main un seuil pour chaque produit ?

## La solution
La méthode des quartiles (règle de Tukey) : pour chaque médicament, on calcule Q1, la médiane et Q3 des prix observés ; l'écart interquartile IQR = Q3 − Q1. Un prix est suspect s'il sort de [Q1 − 1,5 IQR ; Q3 + 1,5 IQR]. Cette méthode est robuste : un prix aberrant ne déforme pas les seuils, contrairement à la moyenne. Données fictives.

## Ce qu'on apprend
Les quartiles et l'interpolation, la règle de Tukey, le regroupement de données par clé en C et le tri de nombres réels.

## Lancer
```bash
gcc -Wall -Wextra -o prix_anormaux prix_anormaux.c -lm
./prix_anormaux
```

## Exemple de sortie
```
Paracétamol 500 mg x16 : médiane 638 FCFA, plage normale [484 ; 809]
   Pharmacie A      600 FCFA     -6 %  ok
   Pharmacie B      650 FCFA     +2 %  ok
   Pharmacie C      600 FCFA     -6 %  ok
   Pharmacie D     6000 FCFA   +841 %  ⚠ TROP CHER (erreur ou surfacturation ?)
   Clinique E       700 FCFA    +10 %  ok
   Pharmacie F      625 FCFA     -2 %  ok

Amoxicilline 500 mg x12 : médiane 1825 FCFA, plage normale [1575 ; 2075]
   Pharmacie A     1800 FCFA     -1 %  ok
   Pharmacie B     1750 FCFA     -4 %  ok
   Pharmacie C     1900 FCFA     +4 %  ok
   Clinique E      3200 FCFA    +75 %  ⚠ TROP CHER (erreur ou surfacturation ?)
   Pharmacie F     1850 FCFA     +1 %  ok
   Pharmacie D      180 FCFA    -90 %  ⚠ TROP BAS (erreur de saisie ?)
```
