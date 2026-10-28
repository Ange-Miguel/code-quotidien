# Jour 180 — Recommandation de produits « achetés ensemble »

**Langue :** Java

## Le problème
Une boutique veut suggérer au client « souvent acheté avec ce produit » (une coque avec un téléphone, un câble avec un chargeur), pour augmenter le panier moyen. Mais comment savoir quels produits vont vraiment ensemble, et pas seulement ceux qui se vendent beaucoup ?

## La solution
L'analyse d'associations sur l'historique des paniers. Pour une paire A → B : support = part des paniers qui contiennent A et B ; confiance = parmi les paniers avec A, part qui contiennent aussi B ; lift = confiance / part des paniers avec B. Un lift > 1 signifie que A « attire » vraiment B ; un produit très populaire a une forte confiance mais un lift proche de 1. On ne garde que les règles avec un support minimal. Paniers fictifs générés avec des affinités connues.

## Ce qu'on apprend
Les mesures support / confiance / lift, le comptage de paires avec des Map, et le piège des produits populaires.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
2000 paniers analysés, 54 règles avec un support ≥ 2 %

Si le client prend    proposer         support  confiance   lift
piles             → lampe solaire       6,2%       100%   6,47
lampe solaire     → piles               6,2%        40%   6,47
téléphone         → coque              18,5%        69%   3,75
verre trempé      → téléphone          13,7%       100%   3,75
coque             → téléphone          18,5%       100%   3,75
téléphone         → verre trempé       13,7%        51%   3,75
verre trempé      → coque               9,5%        69%   3,74
coque             → verre trempé        9,5%        51%   3,74

« Souvent acheté avec téléphone » :
   coque (69 % des acheteurs, lift 3,8)
   verre trempé (51 % des acheteurs, lift 3,8)
```
