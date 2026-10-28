# Jour 182 — Emploi du temps d'examens sans conflit (coloration de graphe)

**Langue :** Python

## Le problème
À la fin du semestre, il faut programmer les examens de toutes les matières. Deux matières suivies par un même étudiant ne peuvent pas avoir lieu sur le même créneau. Combien de créneaux faut-il au minimum, et comment les répartir ?

## La solution
C'est un problème de coloration de graphe. Chaque matière est un sommet ; une arête relie deux matières qui ont au moins un étudiant en commun. Colorier le graphe (deux sommets reliés ont des couleurs différentes) revient à attribuer des créneaux. On utilise l'algorithme DSatur (on colorie d'abord le sommet qui a le plus de couleurs différentes chez ses voisins), généralement meilleur que le glouton simple, et on vérifie que l'emploi du temps est valide. Inscriptions fictives.

## Ce qu'on apprend
La modélisation par un graphe de conflits, la coloration gloutonne et DSatur, et la vérification d'une solution.

## Lancer
```bash
python3 examens_coloration.py
```

## Exemple de sortie
```
9 matières, 22 paires en conflit
   Anglais           incompatible avec : Gestion, Maths, Réseaux, Statistiques, Électronique
   Bases de données  incompatible avec : Gestion, Prog. C, Réseaux, Statistiques
   Gestion           incompatible avec : Anglais, Bases de données, Microcontrôleurs, Réseaux, Statistiques
   Maths             incompatible avec : Anglais, Prog. C, Réseaux, Statistiques, Électronique
   Microcontrôleurs  incompatible avec : Gestion, Prog. C, Réseaux, Électronique
   Prog. C           incompatible avec : Bases de données, Maths, Microcontrôleurs, Réseaux, Électronique
   Réseaux           incompatible avec : Anglais, Bases de données, Gestion, Maths, Microcontrôleurs, Prog. C, Électronique
   Statistiques      incompatible avec : Anglais, Bases de données, Gestion, Maths
   Électronique      incompatible avec : Anglais, Maths, Microcontrôleurs, Prog. C, Réseaux

Glouton (ordre alphabétique) : 5 créneaux | DSatur : 4 créneaux

Emploi du temps des examens (DSatur) :
   lun 8h   : Réseaux, Statistiques
```
