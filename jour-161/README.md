# Jour 161 — Système de notation et avis de vendeurs

**Langue :** Python

## Le problème
Sur une marketplace, un vendeur avec une seule note de 5/5 apparaît « meilleur » qu'un vendeur noté 4,7/5 sur 300 ventes. C'est trompeur : peu d'avis donnent peu de certitude. Et de faux avis groupés peuvent gonfler une note.

## La solution
On classe les vendeurs avec une moyenne bayésienne : on mélange la moyenne du vendeur avec la moyenne générale du site, comme si chaque vendeur avait reçu C avis « moyens » d'office. Avec beaucoup d'avis, sa vraie moyenne domine ; avec peu, il reste près de la moyenne du site. On donne plus de poids aux avis récents et vérifiés (achat confirmé), et on signale les rafales d'avis 5/5 venant de comptes tout neufs. Données fictives.

## Ce qu'on apprend
La moyenne bayésienne, la pondération des données, la détection simple de fraude et le tri par score composite.

## Lancer
```bash
python3 avis_vendeurs.py
```

## Exemple de sortie
```
Moyenne pondérée du site : 4.43/5

Rang Vendeur          Score  Moy. brute  Avis  Remarque
   1 Boutique Kpota    4.49        4.50   300  
   2 Tech Express      4.46        5.00     1  peu d'avis : score prudent
   3 Phone Calavi      4.31        4.20    40  
   4 Super Deal        3.79        4.35    31  ⚠ 25 avis suspects ignorés

Tri naïf par moyenne brute : Tech Express, Boutique Kpota, Super Deal, Phone Calavi
```
