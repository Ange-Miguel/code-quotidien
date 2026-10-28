# Jour 193 — Mini-langage de calcul : analyseur et évaluateur

**Langue :** C

## Le problème
Comment fonctionne un langage de programmation ? Entre le texte « total = prix * quantite » et le résultat, il y a trois étapes : découper le texte en jetons (analyse lexicale), vérifier la structure et calculer (analyse syntaxique et évaluation), en signalant clairement les erreurs.

## La solution
Un petit langage pour faire des calculs de gestion : variables, affectation (=), les quatre opérations, %, parenthèses, puissance (^), comparaisons (<, >, ==) qui valent 1 ou 0, et l'instruction « afficher ». L'analyseur lexical produit des jetons ; l'analyseur à descente récursive évalue directement selon la priorité des opérateurs. Les erreurs indiquent la ligne et la colonne. Les variables sont stockées dans une petite table.

## Ce qu'on apprend
L'analyse lexicale, la grammaire et la priorité des opérateurs, la descente récursive, la gestion d'erreurs avec setjmp/longjmp et une table de symboles.

## Lancer
```bash
gcc -Wall -Wextra -o mini_langage mini_langage.c -lm
./mini_langage
```

## Exemple de sortie
```
 1 │ prix = 4500
 2 │ quantite = 12
 3 │ remise = 10
 4 │ total = prix * quantite * (1 - remise / 100)
 5 │ afficher total
   → 48600
 6 │ afficher total > 50000
   → 0
 7 │ marge = (total - 30000) / total * 100
 8 │ afficher marge
   → 38.2716
 9 │ 2 ^ 3 ^ 2
   = 512
10 │ 17 % 5 + -3
   = -1
```
