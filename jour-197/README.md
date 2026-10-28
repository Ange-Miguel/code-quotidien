# Jour 197 — Simulation de trafic à un carrefour

**Langue :** Python

## Le problème
À un carrefour encombré (avec beaucoup de motos et de voitures), les feux ont des durées fixes : la rue principale attend parfois au rouge alors que la rue secondaire est vide. Peut-on réduire l'attente avec des feux « intelligents » qui s'adaptent au trafic, grâce à des capteurs ?

## La solution
Simulation seconde par seconde. Les véhicules arrivent au hasard (processus de Poisson) sur chaque voie avec un débit différent ; au vert, un véhicule passe toutes les 2 secondes. Trois stratégies : cycle fixe 30 s / 30 s, cycle proportionnel au trafic moyen, et feu adaptatif qui donne le vert à la file la plus longue (avec un vert minimal de 10 s et un maximal de 60 s, plus 3 s d'orange). On mesure l'attente moyenne et la longueur maximale des files.

## Ce qu'on apprend
La simulation à pas de temps, les arrivées aléatoires (processus de Poisson), les files d'attente et la comparaison de stratégies de commande.

## Lancer
```bash
python3 trafic_carrefour.py
```

## Exemple de sortie
```
2 h de pointe : Nord-Sud (principale) 1080 véh/h, Est-Ouest (secondaire) 360 véh/h

Stratégie                    Attente moy.  File max N-S  File max E-O  Passés
Cycle fixe 30 s / 30 s              578 s           489            10    2344
Cycle proportionnel 45/15            22 s            13            15    2820
Feu adaptatif (capteurs)             14 s            13             9    2823

Le feu adaptatif donne le vert là où les véhicules attendent vraiment.
```
