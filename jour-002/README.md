# Jour 002 — Calculateur de monnaie à rendre (pièces et billets FCFA)

**Langue :** Python

## Le problème
Un commerçant doit rendre la monnaie vite et sans erreur, avec le moins de billets et de pièces possible.

## La solution
Algorithme glouton : on prend toujours la plus grande coupure FCFA possible (10 000, 5 000, 2 000, 1 000, 500, 250, 200, 100, 50, 25, 10, 5) jusqu'à épuiser le montant. Avec ces coupures, le glouton donne la solution optimale.

## Ce qu'on apprend
Les listes, la division entière (//) et le modulo (%), les algorithmes gloutons et la validation des entrées.

## Lancer
```bash
python3 monnaie.py
```

## Exemple de sortie
```
Prix : 3750 FCFA | Payé : 10000 FCFA | À rendre : 6250 FCFA
  1 × billet de 5000 FCFA
  1 × billet de 1000 FCFA
  1 × pièce de 250 FCFA
  Total : 3 coupure(s)

Prix : 1225 FCFA | Payé : 2000 FCFA | À rendre : 775 FCFA
  1 × pièce de 500 FCFA
  1 × pièce de 250 FCFA
  1 × pièce de 25 FCFA
  Total : 3 coupure(s)

Prix : 500 FCFA | Payé : 500 FCFA | À rendre : 0 FCFA
  Compte juste, rien à rendre.
  Total : 0 coupure(s)
```
