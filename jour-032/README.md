# Jour 032 — Statistiques d'une série : moyenne, médiane, écart-type

**Langue :** Python

## Le problème
Un relevé de ventes, de températures ou de notes ne parle pas tout seul. On veut savoir quelle est la valeur typique et à quel point les valeurs sont dispersées.

## La solution
On recode nous-mêmes moyenne, médiane, mode, variance, écart-type et quartiles (sans bibliothèque), puis on vérifie nos résultats avec le module statistics. Un histogramme texte montre la forme de la distribution.

## Ce qu'on apprend
Les formules statistiques de base, la différence entre moyenne et médiane face aux valeurs extrêmes, et la vérification croisée d'un calcul.

## Lancer
```bash
python3 statistiques.py
```

## Exemple de sortie
```
Ventes journalières (unités) (14 valeurs)
   moyenne 21.57 | médiane 15.00 | mode 15
   min 9 | max 95 | écart-type 20.96 | Q1 13 | Q3 18
   [     9.0 ;     26.2[ ████████████ 12
   [    26.2 ;     43.4[ █ 1
   [    43.4 ;     60.6[  0
   [    60.6 ;     77.8[  0
   [    77.8 ;     95.0[ █ 1

Remarque : la journée à 95 tire la moyenne vers le haut, pas la médiane.
```
