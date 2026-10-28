# Jour 142 — Planification d'allumage des lumières (scènes horaires)

**Langue :** C

## Le problème
On veut que l'éclairage extérieur s'allume au coucher du soleil, que la lampe de la chambre s'allume doucement le matin en semaine, et qu'en vacances une « simulation de présence » allume et éteint des pièces de façon crédible, pour décourager les cambrioleurs.

## La solution
Une table de règles : jours de la semaine (masque de 7 bits), heure de début et de fin (en minutes depuis minuit, avec gestion du passage de minuit), lampe concernée et niveau. Pour savoir l'état à un instant, on évalue toutes les règles actives ; la dernière qui s'applique l'emporte. Le mode vacances ajoute un décalage pseudo-aléatoire de ±20 min chaque jour.

## Ce qu'on apprend
Les masques de bits pour les jours, l'arithmétique des heures modulo 24 h, les plages qui passent minuit et la priorité entre règles.

## Lancer
```bash
gcc -Wall -Wextra -o scenes_horaires scenes_horaires.c -lm
./scenes_horaires
```

## Exemple de sortie
```
lun :
   00:00  Extérieur allumée
   06:00  Chambre    allumée (atténuée)
   06:15  Extérieur éteinte
   06:30  Chambre    éteinte
   06:30  Cuisine    allumée
   07:15  Cuisine    éteinte
   18:45  Extérieur allumée
   19:00  Salon      allumée (atténuée)
   22:30  Salon      éteinte
sam :
   00:00  Extérieur allumée
   06:15  Extérieur éteinte
   18:45  Extérieur allumée
   19:00  Salon      allumée (atténuée)
```
