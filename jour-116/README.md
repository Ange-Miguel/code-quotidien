# Jour 116 — Distance entre deux coordonnées GPS (formule de Haversine)

**Langue :** Python

## Le problème
Une application de livraison, de covoiturage ou de géolocalisation de bouteilles de gaz doit calculer la distance entre deux points GPS et trouver le point de vente le plus proche d'un client.

## La solution
La Terre étant (presque) une sphère, la distance n'est pas celle d'un plan. La formule de Haversine donne la distance du grand cercle à partir des latitudes et longitudes en radians. On calcule aussi le cap initial (direction à suivre), on compare avec l'approximation plane (valable sur de courtes distances) et on classe des points de vente par proximité. Les coordonnées sont approximatives, pour l'exemple.

## Ce qu'on apprend
La trigonométrie sphérique, les radians, atan2, la précision des approximations et le tri par distance.

## Lancer
```bash
python3 haversine.py
```

## Exemple de sortie
```
De Cotonou à      Haversine  Approx. plane   Écart  Cap
Abomey-Calavi        11.5 km        11.5 km  0.00%  323° NO
Porto-Novo           27.5 km        27.5 km  0.00%   58° NE
Parakou             331.3 km       331.3 km  0.00%    4° N
Natitingou          452.6 km       452.6 km  0.00%  345° N
Lagos               107.6 km       107.6 km  0.00%   80° E
Lomé                135.7 km       135.7 km  0.00%  259° O
Paris              4724.8 km      4724.8 km  0.00%  360° N
Guangzhou         11826.8 km     12064.4 km  2.01%   64° NE

Points de vente les plus proches du client (6.4402, 2.3443) :
   Point gaz Kpota        1508 m vers le NE
   Point gaz Zogbadjè     2586 m vers le S
   Point gaz Godomey      5670 m vers le S
   Point gaz Akassato     7349 m vers le N
```
