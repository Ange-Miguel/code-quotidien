# Jour 043 — Conversion d'unités de longueur, masse et volume

**Langue :** C

## Le problème
Un fournisseur chinois donne des pouces et des livres, une recette étrangère des onces et des tasses, un chantier des pieds. Il faut convertir sans se tromper de catégorie (on ne convertit pas des kg en litres !).

## La solution
Chaque unité est décrite par sa catégorie et son facteur vers l'unité de base (mètre, kilogramme, litre). Convertir = multiplier par le facteur de départ puis diviser par celui d'arrivée. Une table unique remplace des dizaines de formules.

## Ce qu'on apprend
Les tables de données plutôt que du code répété, la recherche dans un tableau de structures, strcmp et la vérification de cohérence.

## Lancer
```bash
gcc -Wall -Wextra -o unites unites.c -lm
./unites
```

## Exemple de sortie
```
     6.500 in   =      16.5100 cm
     5.500 ft   =       1.6764 m
     2.200 lb   =       0.9979 kg
    25.000 kg   =      55.1156 lb
     1.000 gal  =       3.7854 l
     2.000 cup  =     473.1765 ml
    42.195 km   =      26.2188 mi

Unités disponibles : mm cm m km in ft mi g kg t lb oz ml l gal cup
Impossible : kilogramme (masse) → litre (volume)
```
