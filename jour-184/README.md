# Jour 184 — Gestion d'une file d'impression partagée

**Langue :** C

## Le problème
Dans un cybercafé, un secrétariat ou une salle informatique, plusieurs personnes envoient des documents à la même imprimante. Un gros document de 200 pages bloque tout le monde pendant que d'autres attendent pour une seule page, et on veut facturer chaque utilisateur.

## La solution
Une file d'attente circulaire des travaux d'impression, avec une politique choisie à l'exécution : FIFO (premier arrivé, premier servi) ou SJF (le plus court d'abord, qui minimise l'attente moyenne). Une protection anti-famine fait passer un travail qui attend depuis trop longtemps. On simule l'imprimante (pages par minute), on mesure l'attente moyenne et maximale, et on édite la facture par utilisateur.

## Ce qu'on apprend
L'ordonnancement de travaux (FIFO, plus court d'abord), la famine et le vieillissement, et la comparaison de politiques.

## Lancer
```bash
gcc -Wall -Wextra -o file_impression file_impression.c -lm
./file_impression
```

## Exemple de sortie
```
Politique FIFO (ordre d'arrivée) :
     0.0 min  #1  Awa        3 p.  attendu  0 min
     1.0 min  #2  Kofi     120 p.  attendu  0 min
     7.0 min  #3  Inès      1 p.  attendu  5 min
     7.0 min  #4  Serge      8 p.  attendu  5 min
     7.5 min  #5  Awa        2 p.  attendu  3 min
   …
   → attente moyenne 1.8 min, maximale 5 min, tout imprimé en 16.1 min

Politique plus court d'abord + anti-famine :
     0.0 min  #1  Awa        3 p.  attendu  0 min
     1.0 min  #2  Kofi     120 p.  attendu  0 min
     7.0 min  #3  Inès      1 p.  attendu  5 min
     7.0 min  #7  Yves       1 p.  attendu  1 min
     7.1 min  #5  Awa        2 p.  attendu  3 min
```
