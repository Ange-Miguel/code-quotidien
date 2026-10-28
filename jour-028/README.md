# Jour 028 — Minuteur Pomodoro en console

**Langue :** C

## Le problème
Réviser ou coder pendant des heures sans pause fait chuter la concentration. La méthode Pomodoro alterne 25 min de travail et 5 min de pause, avec une longue pause de 15 min tous les 4 cycles.

## La solution
Une machine à états (TRAVAIL, PAUSE_COURTE, PAUSE_LONGUE) et une barre de progression mise à jour chaque seconde. Un facteur d'accélération permet de tester le programme en quelques secondes (mode démo).

## Ce qu'on apprend
Les machines à états, nanosleep pour attendre sans bloquer le processeur, \r pour réécrire une ligne et fflush pour forcer l'affichage.

## Lancer
```bash
gcc -Wall -Wextra -o pomodoro pomodoro.c -lm
./pomodoro
```

## Exemple de sortie
```
Pomodoro : 4 cycle(s) (mode démo accéléré)

[1] Travail       |..............................| 25:00 restantes 
[1] Travail       |..............................| 24:45 restantes 
[1] Travail       |..............................| 24:30 restantes 
[1] Travail       |..............................| 24:15 restantes 
[1] Travail       |#.............................| 24:00 restantes 
[1] Travail       |#.............................| 23:45 restantes 
[1] Travail       |#.............................| 23:30 restantes 
[1] Travail       |##............................| 23:15 restantes 
[1] Travail       |##............................| 23:00 restantes 
[1] Travail       |##............................| 22:45 restantes 
[1] Travail       |###...........................| 22:30 restantes 
[1] Travail       |###...........................| 22:15 restantes 
[1] Travail       |###...........................| 22:00 restantes 
```
