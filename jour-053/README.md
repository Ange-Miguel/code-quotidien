# Jour 053 — Journal des dépenses quotidiennes avec total mensuel

**Langue :** Python

## Le problème
À la fin du mois, l'argent est parti sans qu'on sache où : transport, nourriture, crédit téléphone, sorties… Noter ses dépenses ne sert à rien si on ne les analyse pas.

## La solution
Les dépenses sont enregistrées dans un fichier CSV (date, catégorie, montant, note). Le programme calcule le total par mois et par catégorie, compare au budget fixé pour chaque catégorie et signale les dépassements, avec un graphique en barres.

## Ce qu'on apprend
Le module csv, datetime, les regroupements avec defaultdict, les fichiers temporaires et la présentation d'un rapport.

## Lancer
```bash
python3 depenses.py
```

## Exemple de sortie
```
Mois 2026-09 : total 22 800 FCFA
   sorties       12 000 ██████████████████████████████ ⚠ dépasse le budget de 2 000
   études         6 500 ████████████████
   téléphone      2 000 █████
   nourriture     1 500 ███
   transport        800 ██

Mois 2026-10 : total 54 000 FCFA
   nourriture    43 000 ██████████████████████████████ ⚠ dépasse le budget de 3 000
   transport      9 000 ██████
   téléphone      2 000 █
```
