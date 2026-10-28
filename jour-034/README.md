# Jour 034 — Conversion de chiffres romains ↔ arabes

**Langue :** C

## Le problème
Les chiffres romains apparaissent encore sur les horloges, les chapitres, les noms de rois (Béhanzin régnait au XIXe siècle) et les événements (« IIIe édition »). Il faut savoir les lire et les écrire.

## La solution
Arabe → romain : algorithme glouton avec la table des valeurs incluant les formes soustractives (CM, XC, IV…). Romain → arabe : on lit de gauche à droite ; si un symbole est plus petit que le suivant, on le soustrait. On valide en reconvertissant le résultat (aller-retour).

## Ce qu'on apprend
Les tableaux parallèles, l'algorithme glouton, la validation par aller-retour et la gestion des entrées malformées (« IIII », « VX »).

## Lancer
```bash
gcc -Wall -Wextra -o romains romains.c -lm
./romains
```

## Exemple de sortie
```
1960         → MCMLX
2026         → MMXXVI
1894         → MDCCCXCIV
3999         → MMMCMXCIX
XIX          → 19
MCMLX        → 1960
mmxxvi       → 2026
IIII : nombre romain invalide
VX : nombre romain invalide
```
