# Jour 162 — Panier e-commerce avec codes promo

**Langue :** Java

## Le problème
Une boutique en ligne propose des codes promo variés : pourcentage, montant fixe, livraison offerte, « 3 achetés = le moins cher offert », avec un minimum d'achat, une date limite ou une catégorie. Les règles de cumul et les arrondis sont des sources d'erreurs (et de pertes !).

## La solution
Une interface Promotion avec plusieurs implémentations, chacune vérifiant ses conditions et calculant sa remise. Le panier applique au plus un code (le meilleur pour le client si plusieurs sont fournis), plafonne la remise au sous-total, calcule la livraison selon le montant et affiche un récapitulatif clair. Montants en entiers de FCFA.

## Ce qu'on apprend
Les interfaces et le polymorphisme (types scellés), les règles métier encapsulées, la comparaison de solutions et le calcul monétaire sûr.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Panier 1 (le client essaie plusieurs codes) :
   code BIENVENUE10    valable : -1 700 FCFA
   code AUDIO20        valable : -1 800 FCFA
   code TROISPOURDEUX  valable : -3 000 FCFA
   code PROMO2027      refusé : code inconnu
   Écouteurs Bluetooth      2 ×   4 500 =    9 000
   Chargeur rapide          1 ×   3 500 =    3 500
   Câble USB-C              3 ×   1 500 =    4 500
   Sous-total 17 000 | livraison 1 500 | remise (TROISPOURDEUX) -3 000 → TOTAL 15 500 FCFA

Panier 2 :
   code MOINS3000      valable : -3 000 FCFA
   code LIVRAISON0     valable : -1 500 FCFA
   code ETE2026        refusé : code expiré
   code BIENVENUE10    valable : -4 750 FCFA
```
