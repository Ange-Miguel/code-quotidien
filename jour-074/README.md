# Jour 074 — Jeu du morpion à deux joueurs

**Langue :** Python

## Le problème
Programmer un jeu complet, même simple, oblige à bien représenter l'état (la grille), vérifier les règles (coup valide, victoire, match nul) et séparer la logique de l'affichage.

## La solution
La grille est une liste de 9 cases. Les 8 lignes gagnantes sont listées une fois pour toutes. Deux joueurs humains s'affrontent avec --jouer ; en démo, deux joueurs automatiques jouent : l'un au hasard, l'autre avec une stratégie simple (gagner si possible, sinon bloquer, sinon le centre, puis les coins).

## Ce qu'on apprend
La représentation d'un plateau, la détection de victoire, une petite stratégie de jeu et la boucle principale d'un jeu au tour par tour.

## Lancer
```bash
python3 morpion.py
```

## Exemple de sortie
```
Partie de démonstration : X (stratégie) contre O (hasard)
X joue en 5
O joue en 2
X joue en 1
O joue en 6
X joue en 9
   X | O | 3
   --+---+--
   4 | X | O
   --+---+--
   7 | 8 | X
Résultat : X

500 parties, X au hasard contre O stratège : {'X': 6, 'O': 425, 'nul': 69}
100 parties stratège contre stratège : {'X': 0, 'O': 0, 'nul': 100}
```
