# Jour 031 — Tri des notes d'une classe et classement

**Langue :** C

## Le problème
Un enseignant ou un délégué veut classer les étudiants selon leur moyenne, avec gestion des ex æquo (deux étudiants à 14,5 partagent le même rang, le suivant prend le rang d'après : 1, 2, 2, 4).

## La solution
On stocke les étudiants dans un tableau de structures, on trie avec qsort et une fonction de comparaison (moyenne décroissante, puis nom), puis on calcule les rangs en tenant compte des égalités.

## Ce qu'on apprend
Qsort et les pointeurs de fonction, les structures, le tri stable par critères multiples et le classement « olympique ».

## Lancer
```bash
gcc -Wall -Wextra -o classement classement.c -lm
./classement
```

## Exemple de sortie
```
Rang  Nom                  Moyenne  Décision
  1   Akpanon Ange           15.20  admis
  2e  Adjovi Mireille        14.50  admis
  2e  Dossou Carine          14.50  admis
  4   Agossa Inès           12.25  admis
  5   Kpade Yves             11.00  admis
  6   Houngbo Serge           9.75  ajourné
  7   Zinsou Paul             8.50  ajourné

Moyenne de la classe : 12.24 | admis : 5/7 (71 %)
```
