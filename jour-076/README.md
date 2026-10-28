# Jour 076 — Résolveur de sudoku (backtracking)

**Langue :** C

## Le problème
Résoudre automatiquement n'importe quelle grille de sudoku, même difficile, et comprendre la technique générale du « retour sur trace » (backtracking), utilisée pour les emplois du temps, les labyrinthes…

## La solution
On cherche une case vide ; on essaie chaque chiffre autorisé (absent de la ligne, de la colonne et du bloc 3×3) ; on continue récursivement ; en cas d'impasse, on efface et on essaie le chiffre suivant. Optimisation : on choisit toujours la case vide qui a le moins de possibilités, ce qui réduit énormément l'exploration. Des masques de bits mémorisent les chiffres déjà utilisés.

## Ce qu'on apprend
Le backtracking, la récursivité, les masques de bits et l'heuristique « variable la plus contrainte d'abord ».

## Lancer
```bash
gcc -Wall -Wextra -o sudoku sudoku.c -lm
./sudoku
```

## Exemple de sortie
```
Grille facile :
   5 3 . | . 7 . | . . . 
   6 . . | 1 9 5 | . . . 
   . 9 8 | . . . | . 6 . 
   ------+-------+------
   8 . . | . 6 . | . . 3 
   4 . . | 8 . 3 | . . 1 
   7 . . | . 2 . | . . 6 
   ------+-------+------
   . 6 . | . . . | 2 8 . 
   . . . | 4 1 9 | . . 5 
   . . . | . 8 . | . 7 9 
Solution (51 essais) :
   5 3 4 | 6 7 8 | 9 1 2 
   6 7 2 | 1 9 5 | 3 4 8 
```
