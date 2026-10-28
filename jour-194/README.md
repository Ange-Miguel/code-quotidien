# Jour 194 — Jeu du serpent (snake) en console

**Langue :** Python

## Le problème
Programmer un jeu d'action complet : un serpent se déplace sur une grille, mange des fruits, grandit, et perd s'il touche un mur ou son propre corps. Il faut gérer l'état du jeu, les règles, l'affichage et les entrées.

## La solution
Le corps du serpent est une file (deque) : à chaque pas, on ajoute la nouvelle tête et on retire la queue, sauf s'il a mangé. Un ensemble (set) des cases occupées rend le test de collision instantané. Le fruit apparaît sur une case libre au hasard. En démo, une petite intelligence artificielle joue seule : elle cherche le plus court chemin vers le fruit par un parcours en largeur (BFS) et, si aucun chemin n'existe, choisit le coup qui laisse le plus d'espace libre. Avec --jouer, on joue au clavier (z q s d puis Entrée).

## Ce qu'on apprend
La modélisation d'un jeu, deque et set, le parcours en largeur pour trouver un chemin, et la boucle de jeu.

## Lancer
```bash
python3 serpent.py
```

## Exemple de sortie
```
Tour 15 :
+----------------+
|                |
|                |
|                |
|                |
|                |
|                |
|              oo|
| *            @o|
|                |
|                |
+----------------+  score 1
Tour 120 :
+----------------+
```
