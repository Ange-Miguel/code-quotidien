# Jour 062 — Recherche dichotomique dans un catalogue trié

**Langue :** Python

## Le problème
Dans un catalogue de 100 000 références triées, chercher un produit en lisant tout un par un est lent. Comment trouver une référence, ou tous les produits dans une fourchette de prix, en quelques étapes ?

## La solution
La recherche dichotomique coupe l'intervalle en deux à chaque comparaison : 17 étapes suffisent pour 100 000 éléments. On écrit la version de base, puis les variantes « première position ≥ x » et « dernière position ≤ x », qui permettent de trouver une plage de prix. On compare avec le module bisect et avec une recherche linéaire.

## Ce qu'on apprend
La recherche dichotomique et ses pièges (bornes, boucle infinie), les variantes lower/upper bound et le module bisect.

## Lancer
```bash
python3 recherche_dichotomique.py
```

## Exemple de sortie
```
Catalogue de 100 000 prix triés
Recherche de 157500 FCFA :
   trouvé en 12 étape(s)
Recherche de 123457 FCFA :
   absent (vérifié en 17 étape(s))
Recherche de 499950 FCFA :
   trouvé en 15 étape(s)

Produits entre 15000 et 15500 FCFA : 108 (indices 2909 à 3016)
200 recherches : dichotomique 0.42 ms | linéaire 15 ms
```
