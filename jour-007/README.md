# Jour 007 — Compte à rebours jusqu'à une date (examens, échéance)

**Langue :** C

## Le problème
Combien de jours reste-t-il avant les examens, une échéance de paiement ou la livraison d'une commande ?

## La solution
On convertit chaque date en « numéro de jour » absolu (nombre de jours depuis une origine fixe) avec la formule des jours juliens ; la différence donne le nombre de jours. On le convertit ensuite en semaines.

## Ce qu'on apprend
Les structures, la validation de dates, l'arithmétique entière et la lecture d'une date au format AAAA-MM-JJ avec sscanf.

## Lancer
```bash
gcc -Wall -Wextra -o compte_a_rebours compte_a_rebours.c -lm
./compte_a_rebours
```

## Exemple de sortie
```
Démo, à partir du 2026-10-28 :
Noël                  2026-12-25 : dans 58 jour(s), soit 8 semaine(s) et 2 jour(s)
Fête du Vodoun        2027-01-10 : dans 74 jour(s), soit 10 semaine(s) et 4 jour(s)
Examens (exemple)      2027-02-15 : dans 110 jour(s), soit 15 semaine(s) et 5 jour(s)
Fête de l'indépendance 2026-08-01 : passé depuis 88 jour(s)
```
