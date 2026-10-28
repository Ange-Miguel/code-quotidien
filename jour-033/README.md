# Jour 033 — Recherche du maximum et du minimum dans une liste de prix

**Langue :** Java

## Le problème
On compare les prix d'un même produit chez plusieurs fournisseurs ou vendeurs : qui est le moins cher, le plus cher, et quel est l'écart ?

## La solution
Un seul parcours de la liste suffit pour trouver à la fois le minimum et le maximum (et leur position). On calcule aussi l'économie réalisée en choisissant le moins cher et le prix médian du marché. On compare avec l'API Stream de Java.

## Ce qu'on apprend
L'algorithme de recherche min/max en un passage, les records, les comparateurs et l'API Stream.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Bouteille de gaz 12,5 kg (recharge)
  Godomey               5 600 FCFA
  Calavi Kpota          5 500 FCFA  ← moins cher
  Akpakpa               5 800 FCFA  ← plus cher
  Cadjèhoun             5 650 FCFA
  Écart : 300 FCFA (5 %) | prix médian : 5 625 FCFA

Smartphone d'entrée de gamme
  Boutique Dantokpa    62 000 FCFA
  En ligne (import)    48 500 FCFA  ← moins cher
  Revendeur Ganhi      70 000 FCFA  ← plus cher
  Écart : 21 500 FCFA (44 %) | prix médian : 62 000 FCFA
```
