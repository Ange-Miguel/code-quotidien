# Jour 008 — Calculateur de facture d'électricité selon la tranche de consommation

**Langue :** Python

## Le problème
Les factures d'électricité domestiques sont souvent calculées par tranches de consommation : plus on consomme, plus le kWh coûte cher. Difficile de prévoir sa facture à la main.

## La solution
On découpe la consommation mensuelle en tranches et on applique à chaque morceau son propre prix, puis on ajoute la redevance fixe et la TVA. Les tarifs utilisés sont des valeurs d'exemple modifiables en haut du fichier.

## Ce qu'on apprend
Les listes de tuples, le calcul par tranches (comme l'impôt progressif), l'arrondi et la mise en forme de montants.

## Lancer
```bash
python3 facture_electricite.py
```

## Exemple de sortie
```
Consommation : 15 kWh
      0 →    15 kWh :     15 × 79 =     1 185 FCFA
  Redevance fixe : 500 FCFA
  Hors taxe : 1 685 | TVA 18 % : 303
  TOTAL : 1 988 FCFA

Consommation : 120 kWh
      0 →    20 kWh :     20 × 79 =     1 580 FCFA
     20 →   120 kWh :    100 × 113 =    11 300 FCFA
  Redevance fixe : 500 FCFA
  Hors taxe : 13 380 | TVA 18 % : 2 408
  TOTAL : 15 788 FCFA

Consommation : 400 kWh
      0 →    20 kWh :     20 × 79 =     1 580 FCFA
```
