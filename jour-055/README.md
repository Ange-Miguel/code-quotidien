# Jour 055 — Lecture d'un fichier CSV de ventes et total par produit

**Langue :** C

## Le problème
Une caisse ou une feuille de calcul exporte les ventes en CSV (date;produit;quantité;prix). On veut le chiffre d'affaires par produit sans ouvrir un tableur, par exemple sur une petite carte embarquée.

## La solution
On lit le fichier ligne par ligne avec fgets, on découpe chaque ligne avec strtok_r, on valide les champs numériques et on cumule dans une table de produits. Les lignes invalides sont signalées avec leur numéro au lieu de faire planter le programme. En démo, un fichier temporaire est créé.

## Ce qu'on apprend
Fgets, strtok_r, strtol avec contrôle d'erreur, tmpfile et l'agrégation de données.

## Lancer
```bash
gcc -Wall -Wextra -o ventes_csv ventes_csv.c -lm
./ventes_csv
```

## Exemple de sortie
```
Produit                    Qté    CA (FCFA)
Écouteurs                    5        22500
Chargeur                      5        12500
Câble USB-C                 14        21000
Montre                        1        12000
Chiffre d'affaires total : 68000 FCFA (1 ligne(s) ignorée(s))
  ligne 6 ignorée (format invalide)
```
