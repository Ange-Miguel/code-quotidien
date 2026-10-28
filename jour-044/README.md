# Jour 044 — Calcul de surface et de peinture nécessaire pour une pièce

**Langue :** Python

## Le problème
Avant d'aller chez le quincaillier, il faut savoir combien de pots de peinture acheter. Trop peu oblige à revenir, trop coûte cher.

## La solution
Surface des murs = périmètre × hauteur, moins les ouvertures (portes, fenêtres), plus éventuellement le plafond. On multiplie par le nombre de couches, on divise par le rendement (m²/litre), on ajoute 10 % de marge, puis on choisit la combinaison de pots (15 L, 5 L, 1 L) la moins chère qui couvre le besoin.

## Ce qu'on apprend
La géométrie appliquée, les dataclasses, l'arrondi au supérieur et une recherche exhaustive de la meilleure combinaison d'achats.

## Lancer
```bash
python3 peinture.py
```

## Exemple de sortie
```
Chambre 3 × 4 m : 47.9 m² à peindre, 2 couches → 10.5 L (marge 10 % incluse)
   Achat conseillé : 2 × 5 L  1 × 1 L = 28 000 FCFA

Salon 5 × 6 m : 89.9 m² à peindre, 2 couches → 19.8 L (marge 10 % incluse)
   Achat conseillé : 1 × 15 L  1 × 5 L = 44 500 FCFA

Boutique (murs seuls) : 70.6 m² à peindre, 1 couches → 7.8 L (marge 10 % incluse)
   Achat conseillé : 1 × 5 L  3 × 1 L = 21 500 FCFA
```
