# Jour 079 — Rendu de monnaie optimal (programmation dynamique)

**Langue :** C

## Le problème
Au jour 002, l'algorithme glouton rendait la monnaie de façon optimale avec les coupures FCFA. Mais avec d'autres systèmes de pièces (jetons de jeu, tickets, anciennes monnaies), le glouton peut se tromper : avec des pièces de 1, 3 et 4, rendre 6 donne 4+1+1 au lieu de 3+3.

## La solution
Programmation dynamique. min[m] = nombre minimal de pièces pour le montant m = 1 + min(min[m − p]) sur toutes les pièces p ≤ m. On garde la dernière pièce utilisée pour reconstruire la solution, et on compte aussi le nombre de façons différentes de rendre la somme.

## Ce qu'on apprend
La programmation dynamique ascendante, la reconstruction de solution, la comparaison avec le glouton et le comptage de combinaisons.

## Lancer
```bash
gcc -Wall -Wextra -o rendu_optimal rendu_optimal.c -lm
./rendu_optimal
```

## Exemple de sortie
```
Pièces FCFA, montant 775 :
   optimal : 3 pièce(s) = 500 250 25
   glouton : 3 pièce(s)
   nombre de façons de rendre : 34456

Jetons 1, 3, 4, montant 6 :
   optimal : 2 pièce(s) = 3 3
   glouton : 3 pièce(s)  ← pas optimal !
   nombre de façons de rendre : 4

Tickets 1, 10, 25, montant 30 :
   optimal : 3 pièce(s) = 10 10 10
   glouton : 6 pièce(s)  ← pas optimal !
   nombre de façons de rendre : 5

```
