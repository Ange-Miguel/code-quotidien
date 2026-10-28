# Jour 069 — Calcul d'itinéraire le plus court entre villes (Dijkstra)

**Langue :** Java

## Le problème
Quel est le trajet routier le plus court entre deux villes du Bénin, en passant éventuellement par d'autres villes ? C'est ce que font les GPS et les applications de livraison.

## La solution
Les villes sont les sommets d'un graphe pondéré, les routes ses arêtes (distances approximatives, données d'exemple). L'algorithme de Dijkstra explore les villes dans l'ordre de leur distance au départ, à l'aide d'une file de priorité, et mémorise le prédécesseur de chaque ville pour reconstruire le chemin.

## Ce qu'on apprend
Les graphes pondérés en liste d'adjacence, l'algorithme de Dijkstra, PriorityQueue et la reconstruction d'un chemin.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Cotonou → Parakou : 398 km (13 villes explorées)
   Cotonou → Abomey-Calavi → Allada → Bohicon → Dassa-Zoumé → Parakou
Abomey-Calavi → Natitingou : 595 km (16 villes explorées)
   Abomey-Calavi → Allada → Bohicon → Dassa-Zoumé → Parakou → Djougou → Natitingou
Porto-Novo → Abomey : 174 km (10 villes explorées)
   Porto-Novo → Cotonou → Abomey-Calavi → Allada → Bohicon → Abomey
Ouidah → Kandi : 634 km (15 villes explorées)
   Ouidah → Lokossa → Abomey → Bohicon → Dassa-Zoumé → Parakou → Kandi
```
