# Jour 152 — Covoiturage étudiant : appariement passager / conducteur

**Langue :** Python

## Le problème
Chaque matin, des étudiants de Godomey, Calavi ou Cotonou vont au campus. Certains ont une moto et une place libre, d'autres paient un zem. Comment former automatiquement les meilleures paires conducteur / passager, en respectant les horaires et sans faire de grands détours ?

## La solution
Pour chaque paire possible, on calcule un « coût » : détour du conducteur pour aller chercher le passager (distance GPS), écart entre les heures souhaitées ; les paires impossibles (détour > 2 km, écart > 20 min) sont exclues. Puis on cherche l'affectation qui minimise le coût total : un algorithme glouton (meilleure paire d'abord) puis une amélioration par échanges deux à deux, et on compare à la solution exacte par force brute. Positions et horaires sont fictifs.

## Ce qu'on apprend
La modélisation d'un problème d'affectation, une fonction de coût, le glouton, la recherche locale par échanges et itertools.permutations.

## Lancer
```bash
python3 covoiturage.py
```

## Exemple de sortie
```
Glouton : 4 paire(s), coût total 27.0
   Ange   prend Awa     détour    88 m, départ 7h30 (souhait 7h35)
   Kofi   prend Carine  détour    22 m, départ 7h45 (souhait 7h50)
   Serge  prend Inès    détour    81 m, départ 7h15 (souhait 7h20)
   Yves   prend Paul    détour     5 m, départ 7h40 (souhait 7h30)
Glouton + échanges : 4 paire(s), coût total 27.0
   Ange   prend Awa     détour    88 m, départ 7h30 (souhait 7h35)
   Kofi   prend Carine  détour    22 m, départ 7h45 (souhait 7h50)
   Serge  prend Inès    détour    81 m, départ 7h15 (souhait 7h20)
   Yves   prend Paul    détour     5 m, départ 7h40 (souhait 7h30)
Optimal (force brute) : 4 paire(s), coût total 27.0
   Ange   prend Awa     détour    88 m, départ 7h30 (souhait 7h35)
   Kofi   prend Carine  détour    22 m, départ 7h45 (souhait 7h50)
   Serge  prend Inès    détour    81 m, départ 7h15 (souhait 7h20)
   Yves   prend Paul    détour     5 m, départ 7h40 (souhait 7h30)
```
