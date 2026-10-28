# Jour 169 — Objectifs d'épargne avec projection

**Langue :** C

## Le problème
On veut acheter un ordinateur, payer une formation, constituer l'apport d'un projet ou un fonds d'urgence. Combien mettre de côté chaque mois pour y arriver à temps ? Et si on épargne un montant fixe, quand atteindra-t-on chaque objectif ?

## La solution
Pour chaque objectif (montant cible, déjà épargné, échéance), on calcule la mensualité nécessaire avec la formule de la valeur future d'une suite de versements (si l'épargne rapporte des intérêts) : versement = (cible − déjà × (1+t)^n) × t / ((1+t)^n − 1). Puis on simule mois par mois le partage d'un budget d'épargne entre les objectifs par ordre de priorité, et on affiche une barre de progression et la date d'atteinte. Montants d'exemple.

## Ce qu'on apprend
La valeur future, les versements périodiques, la simulation mois par mois avec priorités et l'affichage de progression.

## Lancer
```bash
gcc -Wall -Wextra -o objectif_epargne objectif_epargne.c -lm
./objectif_epargne
```

## Exemple de sortie
```
Mensualités nécessaires pour respecter chaque échéance :
   Fonds d'urgence             18083 FCFA/mois pendant  6 mois
   Ordinateur portable         28446 FCFA/mois pendant 10 mois
   Formation en ligne          17895 FCFA/mois pendant  5 mois
   Apport projet startup       35966 FCFA/mois pendant 24 mois
   Total nécessaire : 100391 FCFA/mois ; disponible : 40000 FCFA/mois → toutes les échéances ne pourront pas être tenues

Simulation avec 40000 FCFA/mois, par ordre de priorité :
 après  6 mois :
   Fonds d'urgence          [####################] 100 %
   Ordinateur portable      [###########.........]  55 %
   Formation en ligne       [....................]   0 %
   Apport projet startup    [##..................]  10 %
 après 12 mois :
   Fonds d'urgence          [####################] 100 %
```
