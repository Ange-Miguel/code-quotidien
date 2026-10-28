# Jour 064 — Arbre binaire de recherche : annuaire de numéros

**Langue :** C

## Le problème
Un annuaire doit permettre d'ajouter, chercher et supprimer des contacts rapidement, et de les lister dans l'ordre alphabétique, sans trier toute la liste à chaque ajout.

## La solution
Un arbre binaire de recherche (ABR) : pour chaque nœud, les noms plus petits sont à gauche, les plus grands à droite. Recherche et insertion descendent l'arbre ; le parcours infixe (gauche, nœud, droite) donne l'ordre alphabétique. La suppression gère les trois cas (0, 1 ou 2 enfants).

## Ce qu'on apprend
L'allocation dynamique (malloc/free), les pointeurs de pointeurs, la récursivité sur les arbres et la libération de la mémoire.

## Lancer
```bash
gcc -Wall -Wextra -o annuaire_abr annuaire_abr.c -lm
./annuaire_abr
```

## Exemple de sortie
```
Annuaire (ordre alphabétique, hauteur de l'arbre : 5) :
  Adjovi               +229 01 97 33 44 55
  Agossa               +229 01 64 12 12 12
  Dossou               +229 01 90 00 00 01
  Houngbo              +229 01 95 10 20 30
  Kpade                +229 01 69 45 45 45
  Mensah               +229 01 61 22 33 44
  Tchibozo             +229 01 51 77 88 99
  Zinsou               +229 01 66 00 11 22
Recherche houngbo    → +229 01 95 10 20 30 (3 étape(s))
Recherche Kpade      → +229 01 69 45 45 45 (4 étape(s))
Recherche Sossou     → introuvable (3 étape(s))

Suppression de Mensah (racine, 2 enfants) : ok
  Adjovi               +229 01 97 33 44 55
```
