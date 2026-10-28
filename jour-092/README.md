# Jour 092 — Suivi de remboursement de dettes entre amis

**Langue :** Python

## Le problème
Entre amis ou colocataires, on avance souvent de l'argent : l'un paie le taxi, l'autre le repas, un troisième le crédit internet. Au bout d'un mois, plus personne ne sait qui doit combien à qui, et il faudrait des dizaines de virements Mobile Money pour tout régler.

## La solution
On enregistre chaque dépense (qui a payé, pour qui). On calcule le solde net de chacun (ce qu'il a avancé moins ce qu'il a consommé). Puis un algorithme glouton fait payer le plus gros débiteur au plus gros créancier, répète, et aboutit à au plus n − 1 transferts.

## Ce qu'on apprend
Les soldes nets, les dictionnaires, le tri, la simplification de transactions et les arrondis en FCFA.

## Lancer
```bash
python3 dettes.py
```

## Exemple de sortie
```
Dépenses :
   Ange   a payé  12000 FCFA pour Ange, Kofi, Awa, Serge
   Kofi   a payé   3000 FCFA pour Kofi, Awa
   Awa    a payé   8000 FCFA pour Ange, Kofi, Awa, Serge
   Serge  a payé  15000 FCFA pour Serge, Ange, Kofi
   Ange   a payé   2500 FCFA pour Awa
   Kofi   a payé   1000 FCFA pour Ange, Kofi, Awa, Serge

Soldes nets (positif = on lui doit) :
   Serge    +4750 FCFA
   Ange     +4250 FCFA
   Awa      -1250 FCFA
   Kofi     -7750 FCFA

Remboursements simplifiés (3 transferts au lieu de 13) :
```
