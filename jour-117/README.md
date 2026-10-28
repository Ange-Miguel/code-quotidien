# Jour 117 — Géorepérage : un point est-il dans une zone ?

**Langue :** Java

## Le problème
Un traceur GPS sur une moto ou un colis doit déclencher une alerte quand il sort d'une zone autorisée (quartier, ville, entrepôt), ou signaler une arrivée à destination. C'est le géorepérage (geofencing).

## La solution
Deux types de zones. Zone circulaire : on compare la distance au centre (Haversine, jour 116) au rayon. Zone polygonale : algorithme du lancer de rayon : on trace une demi-droite depuis le point et on compte les côtés du polygone traversés ; impair = dedans. On suit ensuite un trajet GPS simulé et on détecte les entrées et sorties de zone (changements d'état).

## Ce qu'on apprend
L'algorithme du point dans un polygone, les interfaces Java (Zone), la détection d'événements par comparaison avec l'état précédent.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
08:00  6.4445, 2.3468
08:03  6.4410, 2.3450 ◀ SORTIE Entrepôt;
08:06  6.4330, 2.3430
08:09  6.4240, 2.3415
08:12  6.4200, 2.3420 ▶ ENTRÉE Campus (zone autorisée);
08:15  6.4160, 2.3440
08:18  6.4100, 2.3460 ◀ SORTIE Campus (zone autorisée);
08:21  6.4080, 2.3500
08:24  6.4150, 2.3460 ▶ ENTRÉE Campus (zone autorisée);
08:27  6.4300, 2.3440 ◀ SORTIE Campus (zone autorisée);
08:30  6.4438, 2.3472 ▶ ENTRÉE Entrepôt;
```
