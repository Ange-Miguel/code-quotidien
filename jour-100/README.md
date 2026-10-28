# Jour 100 — Bilan de la phase 2 : mini-application de gestion de boutique

**Langue :** C

## Le problème
Réunir dans un seul programme ce qu'on a appris pendant les jours 51 à 99 : structures, fichiers, recherche, tri, files et rapports, pour gérer une vraie petite boutique (produits, ventes, tickets de caisse).

## La solution
Un interpréteur de commandes texte (lu depuis l'entrée standard ou un script) : « produit », « vendre », « stock », « rapport », « sauver », « charger ». Le catalogue est trié et interrogé par recherche dichotomique, chaque vente produit un ticket numéroté, le rapport classe les produits par chiffre d'affaires, et l'état peut être sauvegardé dans un fichier CSV.

## Ce qu'on apprend
L'architecture d'un programme complet en C (commandes, données, persistance, rapports), bsearch/qsort et la robustesse face aux commandes invalides.

## Lancer
```bash
gcc -Wall -Wextra -o boutique boutique.c -lm
./boutique
```

## Exemple de sortie
```
> produit ECO01 Écouteurs 4500 10
  ajouté : ECO01
> produit CHG02 Chargeur 2500 20
  ajouté : CHG02
> produit MON03 Montre 12000 3
  ajouté : MON03
> produit LMP04 Lampe_solaire 6000 5
  ajouté : LMP04
> vendre ECO01 3  
  --- TICKET N°0001 ---
  3 × Écouteurs              13500 FCFA
  TOTAL                      13500 FCFA
> vendre MON03 2  
  --- TICKET N°0002 ---
  2 × Montre                  24000 FCFA
```
