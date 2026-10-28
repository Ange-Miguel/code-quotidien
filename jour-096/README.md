# Jour 096 — Planificateur d'emploi du temps sans conflits

**Langue :** Java

## Le problème
Placer les cours d'une semaine dans des créneaux en respectant les contraintes : un enseignant ne peut pas être à deux endroits à la fois, une classe ne peut pas avoir deux cours en même temps, et certaines salles (laboratoire d'électronique) sont uniques.

## La solution
Backtracking avec heuristique : on place d'abord les cours les plus contraints (ceux qui demandent une ressource rare), on essaie chaque créneau, on vérifie qu'aucune ressource (enseignant, classe, salle) n'est déjà utilisée sur ce créneau, et on revient en arrière si on est bloqué.

## Ce qu'on apprend
La modélisation de contraintes, les Set de ressources occupées par créneau, le backtracking et l'ordre de traitement des variables les plus contraintes.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
14 cours placés sans conflit (14 essais)

Emploi du temps SE2
      Lun               Mar               Mer               Jeu               Ven
08h   Électronique TP   Anglais           —                 —                 Réseaux
10h   Microcontrôleurs  Électronique TP   —                 Maths             —
14h   Maths             Prog. C           —                 —                 —

Emploi du temps GL2
      Lun               Mar               Mer               Jeu               Ven
08h   —                 —                 Maths             Prog. C           —
10h   —                 —                 Anglais           —                 Prog. C
14h   —                 —                 Électronique TP   —                 Réseaux
```
