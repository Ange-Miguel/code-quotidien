# Jour 098 — Gestion de parking : places libres et durée de stationnement

**Langue :** Python

## Le problème
Un parking de marché ou d'hôpital veut savoir en temps réel combien de places sont libres, combien de temps chaque véhicule est resté, et combien facturer (tarif dégressif, première demi-heure gratuite, forfait journalier plafonné).

## La solution
Une classe Parking garde les places dans un dictionnaire {numéro de place: (immatriculation, heure d'entrée)}. L'entrée attribue la plus petite place libre (tas heapq), la sortie calcule la durée et le tarif. On rejoue une journée d'entrées et de sorties et on produit les statistiques (taux d'occupation, recette, durée moyenne).

## Ce qu'on apprend
Heapq pour trouver vite la plus petite place libre, datetime et timedelta, les règles de tarification et la validation d'une plaque.

## Lancer
```bash
python3 parking.py
```

## Exemple de sortie
```
07:50 entrée AB 1234 RB → place 1
        places libres : 3/4
08:05 entrée CD 5678 RB → place 2
        places libres : 2/4
08:30 entrée EF 9012 RB → place 3
        places libres : 1/4
09:00 entrée GH 3456 RB → place 4
        places libres : 0/4
09:10 entrée IJ 7890 RB → COMPLET, refusé
        places libres : 0/4
09:20 sortie CD 5678 RB (place 2) après 1:15:00 → 200 FCFA
        places libres : 1/4
09:25 entrée IJ 7890 RB → place 2
        places libres : 0/4
09:25 sortie GH 3456 RB (place 4) après 0:25:00 → 0 FCFA
```
