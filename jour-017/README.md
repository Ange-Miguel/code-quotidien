# Jour 017 — Jeu « devine le nombre » avec nombre d'essais

**Langue :** Python

## Le problème
Un petit jeu pour apprendre la logique de recherche : deviner un nombre entre 1 et 100 avec le moins d'essais possible.

## La solution
Le jeu répond « plus grand » ou « plus petit ». En mode démo, un joueur automatique applique la recherche dichotomique (toujours proposer le milieu de l'intervalle restant) et trouve toujours en 7 essais au plus, puisque 2^7 = 128 > 100. En mode interactif (--jouer), c'est vous qui jouez.

## Ce qu'on apprend
Les boucles while, la recherche dichotomique, le logarithme en base 2 et la séparation entre la logique du jeu et l'interface.

## Lancer
```bash
python3 devine_nombre.py
```

## Exemple de sortie
```
Essais maximum théoriques : 7
Nombre secret : 68
   essai 1 :  50 → plus grand
   essai 2 :  75 → plus petit
   essai 3 :  62 → plus grand
   essai 4 :  68 → gagné
   → trouvé en 4 essais
Nombre secret : 1
   essai 1 :  50 → plus petit
   essai 2 :  25 → plus petit
   essai 3 :  12 → plus petit
   essai 4 :   6 → plus petit
   essai 5 :   3 → plus petit
   essai 6 :   1 → gagné
   → trouvé en 6 essais
```
