# Jour 151 — Disponibilité des bouteilles de gaz par quartier (recherche du plus proche)

**Langue :** C

## Le problème
Quand la bouteille de gaz est vide, on fait parfois le tour de plusieurs dépôts avant d'en trouver un qui a encore des bouteilles de la bonne taille. Une application qui indique le point de vente le plus proche ayant du stock ferait gagner du temps et du carburant.

## La solution
Chaque point de vente a une position GPS, un stock par taille (6 kg, 12,5 kg) et l'heure de la dernière mise à jour. Pour une demande (position du client, taille voulue), on calcule la distance de Haversine vers chaque point, on écarte ceux sans stock ou dont l'information est trop ancienne (plus de 3 h), et on trie les candidats par distance. Positions et stocks sont fictifs, pour l'exemple.

## Ce qu'on apprend
La combinaison filtre + tri, le calcul de distance GPS en C, la fraîcheur des données et le tri d'indices avec qsort.

## Lancer
```bash
gcc -Wall -Wextra -o gaz_proche gaz_proche.c -lm
./gaz_proche
```

## Exemple de sortie
```
Client à Zogbadjè (campus), bouteille de 12,5 kg :
   (ignoré : Station Godomey, information vieille de 6 h)
   1. Dépôt Tankpè       3.84 km  stock  2  (mis à jour il y a 90 min)  ← conseillé
   2. Dépôt Kpota         3.97 km  stock 14  (mis à jour il y a 30 min)
   3. Boutique Akassato     9.83 km  stock  6  (mis à jour il y a 15 min)

Client à Akassato, bouteille de 6 kg :
   (ignoré : Station Godomey, information vieille de 6 h)
   1. Boutique Akassato     0.62 km  stock  3  (mis à jour il y a 15 min)  ← conseillé
   2. Dépôt Calavi Centre  5.63 km  stock  5  (mis à jour il y a 20 min)
   3. Dépôt Zogbadjè     9.29 km  stock  8  (mis à jour il y a 45 min)

Client à Godomey, bouteille de 12,5 kg :
   (ignoré : Station Godomey, information vieille de 6 h)
   1. Dépôt Tankpè       6.46 km  stock  2  (mis à jour il y a 90 min)  ← conseillé
```
