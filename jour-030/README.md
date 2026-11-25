# Jour 030 — Chronomètre avec tours intermédiaires

**Langue :** Java

## Le problème
Pour un entraînement sportif, un test de vitesse ou la mesure du temps d'exécution d'un programme, on veut un chronomètre qui enregistre des tours et donne le meilleur, le pire et la moyenne.

## La solution
Une classe Chronometre basée sur System.nanoTime (horloge monotone, insensible aux changements d'heure). L'horloge est injectable via une interface LongSupplier : la démo utilise une horloge simulée pour un résultat reproductible, le mode --reel mesure les appuis sur Entrée.

## Ce qu'on apprend
L'injection de dépendance, les interfaces fonctionnelles, les horloges monotones et les statistiques simples sur une liste.

## Lancer
```bash
javac Main.java
java Main
```

