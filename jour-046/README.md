# Jour 046 — Planning de révision réparti sur N jours

**Langue :** C

## Le problème
À l'approche des examens, on a plusieurs matières de difficultés différentes et un nombre limité de jours. Comment répartir les heures sans oublier de matière ni surcharger une journée ?

## La solution
Chaque matière reçoit un nombre d'heures proportionnel à son coefficient × sa difficulté. On distribue ensuite les heures jour par jour en donnant toujours la prochaine heure à la matière qui a le plus grand « reste à faire » (méthode du plus fort reste), avec un maximum d'heures par jour et sans mettre deux fois de suite la même matière si possible.

## Ce qu'on apprend
La répartition proportionnelle, les algorithmes gloutons, les tableaux à deux dimensions et l'affichage d'un emploi du temps.

## Lancer
```bash
gcc -Wall -Wextra -o planning_revision planning_revision.c -lm
./planning_revision
```

## Exemple de sortie
```
6 jours × 4 h = 24 h de révision

Jour  1 : Électronique Maths        Électronique Maths       
Jour  2 : Électronique Maths        Électronique Prog. C     
Jour  3 : Électronique Maths        Électronique Prog. C     
Jour  4 : Électronique Réseaux     Maths        Prog. C     
Jour  5 : Électronique Réseaux     Maths        Prog. C     
Jour  6 : Électronique Réseaux     Maths        Anglais     

Bilan :
  Électronique coef 4, difficulté 3 →  9 h (quota 9.0 h)
  Prog. C      coef 3, difficulté 2 →  4 h (quota 4.5 h)
  Maths        coef 3, difficulté 3 →  7 h (quota 6.8 h)
  Réseaux     coef 2, difficulté 2 →  3 h (quota 3.0 h)
  Anglais      coef 1, difficulté 1 →  1 h (quota 0.8 h)
```
