# Jour 077 — Problème des N reines

**Langue :** Python

## Le problème
Placer N reines sur un échiquier N × N sans qu'aucune ne puisse en prendre une autre (ni même ligne, ni même colonne, ni même diagonale). C'est un problème classique de contraintes, proche de l'affectation de fréquences radio ou de créneaux sans conflit.

## La solution
Backtracking ligne par ligne. On place une reine par ligne et on mémorise les colonnes et les deux familles de diagonales déjà occupées (ligne − colonne et ligne + colonne) dans des ensembles, ce qui rend le test de conflit instantané. On compte toutes les solutions et on affiche la première. Les comptes connus (8 reines : 92 solutions) servent de test.

## Ce qu'on apprend
Le backtracking avec ensembles, les invariants des diagonales, les générateurs (yield) et la croissance du nombre de solutions.

## Lancer
```bash
python3 n_reines.py
```

## Exemple de sortie
```
4 reines : 2 solution(s) en 0 ms
   □ ♛ □ ·
   · □ · ♛
   ♛ · □ ·
   · □ ♛ □

6 reines : 4 solution(s) en 0 ms
   □ ♛ □ · □ ·
   · □ · ♛ · □
   □ · □ · □ ♛
   ♛ □ · □ · □
   □ · ♛ · □ ·
   · □ · □ ♛ □

8 reines : 92 solution(s) en 2 ms
```
