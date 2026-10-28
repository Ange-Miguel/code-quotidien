# Jour 122 — Détection de coupure de courant et calcul de durée (journal)

**Langue :** Python

## Le problème
Les coupures d'électricité perturbent commerces, cliniques et ateliers. Pour se plaindre au fournisseur, dimensionner un onduleur ou un groupe électrogène, il faut des chiffres : combien de coupures, quand, combien de temps au total ?

## La solution
Une petite carte (ESP32 sur batterie) note toutes les minutes la tension du secteur. On détecte une coupure quand la tension tombe sous 150 V pendant au moins 2 relevés (pour ignorer les micro-coupures), et la fin quand elle repasse au-dessus de 180 V (hystérésis). On reconstitue la liste des coupures, on calcule les statistiques par jour et par tranche horaire, et la disponibilité en pourcentage.

## Ce qu'on apprend
La détection d'événements dans une série temporelle, la fusion de micro-coupures, datetime et les statistiques de disponibilité.

## Lancer
```bash
python3 coupures.py
```

## Exemple de sortie
```
10080 relevés sur 7 jours → 6 coupure(s) d'au moins 2 min

   lun 19/10 20:55 → 21:40  ( 45 min)
   mer 21/10 02:14 → 05:42  (208 min)
   jeu 22/10 07:19 → 08:49  ( 90 min)
   ven 23/10 20:25 → 21:55  ( 90 min)
   sam 24/10 02:21 → 03:06  ( 45 min)
   dim 25/10 20:05 → 23:05  (180 min)

Durée totale sans courant : 10:58:00 | disponibilité 93.47 %
Plus longue coupure : 3:28:00 le 21/10
Répartition : nuit (0-6 h) 2, matin (6-12 h) 1, après-midi (12-18 h) 0, soirée (18-24 h) 3
```
