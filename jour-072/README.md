# Jour 072 — Tri topologique : ordre des cours avec prérequis

**Langue :** Java

## Le problème
Dans un cursus, certains cours exigent d'en avoir suivi d'autres (pas de microcontrôleurs sans électronique numérique ni programmation C). Dans quel ordre suivre les cours, et lesquels peut-on suivre en parallèle le même semestre ?

## La solution
Algorithme de Kahn. On compte pour chaque cours le nombre de prérequis non encore suivis (degré entrant). Les cours à 0 forment le premier semestre ; on les « retire », ce qui diminue le degré de leurs successeurs, et on recommence. S'il reste des cours, il y a un cycle.

## Ce qu'on apprend
Le tri topologique, les degrés entrants, le traitement par niveaux (semestres) et la détection de dépendances impossibles.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Semestre 1 : Algorithmique, Mathématiques, Électricité
Semestre 2 : Programmation C, Réseaux, Électronique numérique
Semestre 3 : Architecture des ordinateurs
Semestre 4 : Microcontrôleurs
Semestre 5 : Systèmes temps réel
Semestre 6 : Projet IoT

Avec une erreur dans la maquette (Algorithmique exige le Projet IoT) :
Impossible : prérequis circulaires entre : [Algorithmique, Microcontrôleurs, Programmation C, Projet IoT, Réseaux, Systèmes temps réel]
```
