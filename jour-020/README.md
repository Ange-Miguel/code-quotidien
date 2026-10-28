# Jour 020 — Calculateur de consommation de carburant (L/100 km et coût)

**Langue :** Python

## Le problème
Un conducteur de zem ou de voiture veut connaître sa consommation réelle (L/100 km) et ce que lui coûte chaque kilomètre, pour fixer ses prix ou comparer deux motos.

## La solution
À partir de pleins successifs (kilométrage au compteur et litres mis), on calcule la consommation de chaque trajet, la moyenne globale et le coût par kilomètre selon le prix du litre.

## Ce qu'on apprend
Les dataclasses, le calcul sur des paires d'éléments successifs (zip), les moyennes pondérées et la détection d'incohérences.

## Lancer
```bash
python3 carburant.py
```

## Exemple de sortie
```
Trajet      km  litres  L/100km  FCFA/km
       1     180     4.1     2.28     15.5
       2     215     4.8     2.23     15.2
       3     165     3.9     2.36     16.1

Total : 560 km, 12.8 L → moyenne 2.29 L/100 km
Coût moyen : 15.5 FCFA/km (litre à 680 FCFA)
Autonomie estimée avec 10 L : 438 km
```
