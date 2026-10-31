# Jour 005 — Calcul de l'IMC avec interprétation

**Langue :** Python

## Le problème
On veut savoir rapidement si son poids est adapté à sa taille, par exemple avant de commencer un programme de sport.

## La solution
IMC = poids (kg) / taille² (m). On compare le résultat aux seuils de l'OMS et on calcule la plage de poids « normal » pour cette taille.

## Ce qu'on apprend
Les fonctions, les tuples, la recherche dans une table de seuils et la validation de valeurs réalistes.

## Lancer
```bash
python3 imc.py
```

## Exemple de sortie
```
68.0 kg pour 1.75 m → IMC 22.2 : Corpulence normale
   Poids normal pour cette taille : 56.7 à 76.6 kg
52.0 kg pour 1.80 m → IMC 16.0 : Dénutrition
   Poids normal pour cette taille : 59.9 à 81.0 kg
95.0 kg pour 1.70 m → IMC 32.9 : Obésité modérée
   Poids normal pour cette taille : 53.5 à 72.2 kg

Rappel : l'IMC est un indicateur, pas un diagnostic médical.
```
