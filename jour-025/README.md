# Jour 025 — Calcul du prix de revient d'un produit importé de Chine (achat + fret + douane)

**Langue :** C

## Le problème
Un produit acheté 3 € sur une plateforme chinoise ne coûte pas 3 € une fois à Cotonou : il faut ajouter le fret, la douane, le transit et les frais locaux. Sans ce calcul, on vend à perte sans le savoir.

## La solution
On additionne le prix d'achat converti en FCFA, la part du fret répartie au poids, les droits et taxes calculés sur la valeur CAF (coût + assurance + fret) et les frais fixes répartis sur la quantité. Les taux sont des valeurs d'exemple à adapter.

## Ce qu'on apprend
Les structures, les fonctions qui calculent plusieurs résultats, la notion de valeur CAF et la répartition de coûts.

## Lancer
```bash
gcc -Wall -Wextra -o prix_revient prix_revient.c -lm
./prix_revient
```

## Exemple de sortie
```
Écouteurs Bluetooth : 100 unités
  Achat                209906 FCFA
  Fret                  52000 FCFA
  Assurance              2099 FCFA
  Valeur CAF           264005 FCFA
  Douane/taxes         105602 FCFA
  Frais fixes           35000 FCFA
  TOTAL                404607 FCFA
  => prix de revient : 4046 FCFA/unité (x1.9 le prix d'achat)

Montres connectées : 40 unités
  Achat                249264 FCFA
  Fret                  39000 FCFA
  Assurance              2493 FCFA
  Valeur CAF           290756 FCFA
```
