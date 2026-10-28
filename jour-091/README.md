# Jour 091 — Gestionnaire de tontine : cotisations et tour de chaque membre

**Langue :** C

## Le problème
Dans une tontine, chaque membre cotise la même somme à chaque séance et, à tour de rôle, un membre reçoit la cagnotte. Il faut savoir qui a payé, qui est en retard, qui « bouffe » à quelle séance, et la position de chacun (a-t-il déjà reçu plus ou moins qu'il n'a versé ?).

## La solution
Un tableau [membres][séances] enregistre les paiements. L'ordre de passage est fixé par tirage au sort reproductible (graine) avec le mélange de Fisher-Yates. Pour chaque séance, la cagnotte versée est la somme réellement collectée ; les retards sont suivis et pénalisés.

## Ce qu'on apprend
Les tableaux 2D, le mélange de Fisher-Yates, le suivi de soldes (crédit / débit) et la présentation d'un état récapitulatif.

## Lancer
```bash
gcc -Wall -Wextra -o tontine tontine.c -lm
./tontine
```

## Exemple de sortie
```
Tontine de 6 membres, cotisation 20000 FCFA par séance

Séance  Bénéficiaire  Collecté   Impayés
   1    Inès           100000   Yves 
   2    Awa             100000   Kofi 
   3    Mireille        120000   —
   4    Yves            120000   —
   5    Serge           120000   —
   6    Kofi            120000   —

Membre      Versé     Reçu     Pénalités  Solde
Awa         120000    100000      1000    -21000
Kofi        100000    120000      1000    +19000  ⚠ doit encore cotiser
Mireille    120000    120000      1000     -1000
Serge       120000    120000         0        +0
```
