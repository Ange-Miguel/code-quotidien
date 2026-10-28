# Jour 118 — Simulation d'un capteur ultrason : niveau d'une citerne d'eau

**Langue :** C

## Le problème
Dans beaucoup de maisons, l'eau est stockée dans une citerne ou un tank sur le toit. On ne sait pas combien il en reste avant qu'elle soit vide. Un capteur à ultrasons (HC-SR04, JSN-SR04T étanche) placé sous le couvercle mesure la distance jusqu'à la surface de l'eau.

## La solution
Le capteur renvoie la durée de l'écho (aller-retour) en microsecondes. Distance (cm) = durée × vitesse du son / 2, avec une vitesse du son corrigée par la température (331,3 + 0,606 × T m/s), importante sous le soleil. Niveau = hauteur de la citerne − distance ; on en déduit le pourcentage et le volume restant (cylindre), en filtrant par la médiane de 5 mesures (jour 105) et en rejetant les échos hors limites.

## Ce qu'on apprend
Le principe du sonar, l'influence de la température, le calcul de volume et la robustesse d'une mesure physique.

## Lancer
```bash
gcc -Wall -Wextra -o citerne_ultrason citerne_ultrason.c -lm
./citerne_ultrason
```

## Exemple de sortie
```
Citerne : 120 cm de haut, Ø 110 cm, capacité 1140 L

 30.0 °C | distance   25.4 cm | eau 114.6 cm ( 96 %) |  1089 L | 4/5 mesures valides  ####################
 30.0 °C | distance   56.4 cm | eau  83.6 cm ( 70 %) |   794 L | 4/5 mesures valides  ##############......
 30.0 °C | distance   89.4 cm | eau  50.6 cm ( 42 %) |   481 L | 4/5 mesures valides  #########...........
 30.0 °C | distance  122.4 cm | eau  17.6 cm ( 15 %) |   167 L | 4/5 mesures valides  ###.................  ⚠ niveau bas

Même niveau (84 cm) mais température du capteur non corrigée (20 °C supposé au lieu de 45 °C) :
   corrigé : 55.2 cm | non corrigé : 52.9 cm → erreur de 2.3 cm
```
