# Jour 029 — Calcul de marge et prix de vente conseillé

**Langue :** Python

## Le problème
Beaucoup de petits vendeurs confondent marge et coefficient : ajouter 30 % au prix de revient ne donne pas 30 % de marge sur le prix de vente.

## La solution
On calcule les deux notions. Taux de marque (marge / prix de vente) et taux de marge (marge / prix de revient). À partir d'un taux de marque visé, prix de vente = revient / (1 − taux). On arrondit ensuite à un « prix psychologique » se terminant par 900 ou 500 FCFA.

## Ce qu'on apprend
La différence entre deux pourcentages qui se ressemblent, les fonctions pures, l'arrondi personnalisé et les tests par assertions.

## Lancer
```bash
python3 marge.py
```

## Exemple de sortie
```
Écouteurs Bluetooth — revient 2 850 FCFA  taux de marque visé 40%
   prix calculé 4 750 → prix affiché 4 900 FCFA
   marge 2 050 FCFA | taux de marque 41.8% | taux de marge 71.9% | coefficient ×1.72

Montre connectée — revient 8 100 FCFA  taux de marque visé 35%
   prix calculé 12 462 → prix affiché 12 500 FCFA
   marge 4 400 FCFA | taux de marque 35.2% | taux de marge 54.3% | coefficient ×1.54

Lampe solaire — revient 4 600 FCFA  taux de marque visé 30%
   prix calculé 6 571 → prix affiché 6 900 FCFA
   marge 2 300 FCFA | taux de marque 33.3% | taux de marge 50.0% | coefficient ×1.50
```
