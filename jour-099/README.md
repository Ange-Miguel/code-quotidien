# Jour 099 — Mini-tableur : évaluer des formules simples en cellules

**Langue :** Java

## Le problème
Comment un tableur comme Excel recalcule-t-il automatiquement les cellules (=A1*B1, =SOMME(C1:C3)) quand une valeur change, et comment détecte-t-il une référence circulaire ?

## La solution
Chaque cellule contient un nombre ou une formule. L'évaluation est récursive : pour calculer =A1*B1 on évalue A1 et B1 d'abord. On garde en cache les valeurs calculées et un ensemble des cellules « en cours de calcul » : retomber sur l'une d'elles signale une référence circulaire. Formules gérées : + − * /, parenthèses, références (A1) et SOMME/MOYENNE sur une plage (A1:A3).

## Ce qu'on apprend
L'évaluation récursive avec mémoïsation, la détection de cycle, les expressions régulières et un analyseur d'expressions (repris du jour 042).

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Feuille de vente :
  A1   4500                       → 4 500,00
  A2   2500                       → 2 500,00
  A3   12000                      → 12 000,00
  B1   3                          → 3,00
  B2   10                         → 10,00
  B3   1                          → 1,00
  C1   =A1*B1                     → 13 500,00
  C2   =A2*B2                     → 25 000,00
  C3   =A3*B3                     → 12 000,00
  C4   =SOMME(C1:C3)              → 50 500,00
  C5   =C4*0.18                   → 9 090,00
  C6   =C4+C5                     → 59 590,00
  D1   =MOYENNE(A1:A3)            → 6 333,33

```
