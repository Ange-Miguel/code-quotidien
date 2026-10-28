# Jour 066 — Système de réservation de places de bus

**Langue :** Java

## Le problème
Une compagnie de transport Cotonou–Parakou vend des places sur papier ; des places sont vendues deux fois et on ne sait pas combien il en reste par trajet.

## La solution
Chaque voyage a un plan de sièges (4 par rangée, couloir au milieu). On peut réserver un siège précis ou le premier libre (fenêtre de préférence), annuler, et afficher le plan. Chaque réservation reçoit un code unique. Les méthodes sont synchronisées pour éviter les doubles ventes si plusieurs guichets réservent en même temps.

## Ce qu'on apprend
Les tableaux à deux dimensions, les Optional, la génération de codes, la synchronisation des accès concurrents et l'affichage d'un plan.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Réservation 3B : COT-001
Réservation 3B à nouveau : refusée (déjà prise)
Premier siège fenêtre : COT-002
Annulation de COT-002 : ok
Plan du bus Cotonou-Parakou (X = occupé)
   1 [ ][X]  [X][X]
   2 [X][X]  [X][X]
   3 [X][X]  [X][X]
   4 [X][X]  [X][X]
   5 [X][X]  [ ][X]
   6 [X][X]  [ ][X]
  3 place(s) libre(s) sur 24
Réservations enregistrées : 21
```
