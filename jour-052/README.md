# Jour 052 — Gestion de stock d'une boutique (ajout, vente, alerte rupture)

**Langue :** C

## Le problème
Une boutique perd des ventes quand un produit populaire est en rupture, et immobilise de l'argent dans des produits qui ne partent pas.

## La solution
Chaque produit a un stock, un seuil d'alerte et un prix. Les opérations (réception, vente) sont des fonctions qui vérifient les règles (pas de vente au-delà du stock). On calcule la valeur du stock et la liste des produits à recommander, avec une quantité suggérée.

## Ce qu'on apprend
Les tableaux de structures, les fonctions qui renvoient un code d'erreur, la recherche par référence et les rapports de synthèse.

## Lancer
```bash
gcc -Wall -Wextra -o stock stock.c -lm
./stock
```

## Exemple de sortie
```
Journal des opérations :
  vente     SAV01  x25  → ok
  vente     HUI05  x8   → ok
  vente     RIZ25  x7   → stock insuffisant
  vente     RIZ25  x3   → ok
  vente     LAIT1  x1   → produit introuvable
  réception SAV01  x20  → ok
  vente     SUC01  x14  → ok

Réf   Produit                   Stock  Seuil     Valeur
SAV01  Savon de Marseille           35     10       8750
HUI05  Huile 5 L                     4      5      26000  ⚠ à recommander
RIZ25  Riz 25 kg                     3      4      43500  ⚠ à recommander
SUC01  Sucre 1 kg                    6      8       3600  ⚠ à recommander
Valeur du stock : 81850 FCFA | marge réalisée : 22950 FCFA
```
