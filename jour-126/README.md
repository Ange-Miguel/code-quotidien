# Jour 126 — Ordonnanceur coopératif de tâches (super-loop)

**Langue :** Java

## Le problème
Un microcontrôleur sans système d'exploitation doit faire plusieurs choses « en même temps » : lire un capteur toutes les 100 ms, faire clignoter une LED toutes les 500 ms, envoyer les données toutes les 2 s, surveiller un bouton. Avec des delay(), tout se bloque.

## La solution
Un ordonnanceur coopératif. Chaque tâche a une période et la date de sa prochaine exécution. La boucle principale (super-loop) regarde l'horloge et lance les tâches dues, qui s'exécutent rapidement et rendent la main. On simule une horloge en millisecondes, on mesure la gigue (retard) de chaque tâche, et on montre l'effet d'une tâche trop longue qui retarde toutes les autres : c'est la limite du modèle coopératif.

## Ce qu'on apprend
Le multitâche coopératif, les tâches périodiques, la gestion du temps sans blocage et la mesure de la gigue d'exécution.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
10 secondes simulées, tâches courtes :
   t=    0 ms  lire capteur     (retard 0 ms)
   t=    2 ms  clignoter LED    (retard 2 ms)
   t=    3 ms  lire bouton      (retard 3 ms)
   t=    4 ms  envoyer données  (retard 4 ms)
   t=   20 ms  lire bouton      (retard 0 ms)
   t=  100 ms  lire capteur     (retard 0 ms)
   t=  502 ms  clignoter LED    (retard 2 ms)
   t= 2004 ms  envoyer données  (retard 4 ms)
   Tâche             Période Exécutions  Retard moy.   Retard max
   lire capteur        100 ms        100       0,0 ms         0 ms
   clignoter LED       500 ms         20       2,0 ms         2 ms
   lire bouton          20 ms        500       0,4 ms         3 ms
   envoyer données    2000 ms          5       4,0 ms         4 ms

```
