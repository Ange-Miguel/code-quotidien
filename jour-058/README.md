# Jour 058 — File d'attente (queue) : simulation d'un guichet de banque

**Langue :** C

## Le problème
Une agence de banque ou de mobile money se demande combien de guichets ouvrir pour que les clients n'attendent pas trop, sans payer des agents inutiles.

## La solution
On simule minute par minute l'arrivée aléatoire des clients (probabilité par minute) dans une file FIFO implémentée par un tableau circulaire. Chaque guichet libre prend le client en tête de file. On mesure l'attente moyenne et maximale et la longueur de file, pour 1, 2 et 3 guichets.

## Ce qu'on apprend
La file circulaire (enfiler/défiler en O(1)), la simulation à événements discrets et l'interprétation de résultats de simulation.

## Lancer
```bash
gcc -Wall -Wextra -o guichet guichet.c -lm
./guichet
```

## Exemple de sortie
```
Journée de 8 h, 27 arrivées/heure en moyenne, service de 2 à 6 min

1 guichet(s) :  117 clients servis | attente moyenne 128.7 min | max 217 min | file max 112 | restants 112
2 guichet(s) :  219 clients servis | attente moyenne   4.5 min | max  16 min | file max   6 | restants 4
3 guichet(s) :  221 clients servis | attente moyenne   0.2 min | max   3 min | file max   2 | restants 1
4 guichet(s) :  220 clients servis | attente moyenne   0.0 min | max   2 min | file max   2 | restants 0
```
