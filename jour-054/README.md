# Jour 054 — Gestionnaire de tâches (to-do) avec priorités, sauvegardé en fichier

**Langue :** Java

## Le problème
Entre les cours, les projets freelance et les commandes, on oublie des tâches importantes et on traite d'abord les plus faciles.

## La solution
Chaque tâche a une priorité (HAUTE, MOYENNE, BASSE) et une échéance. Une PriorityQueue les ordonne : d'abord la priorité, puis la date la plus proche. Les tâches sont enregistrées dans un fichier texte et les tâches en retard sont signalées.

## Ce qu'on apprend
PriorityQueue et Comparator composés, les enums, LocalDate et la sérialisation simple d'objets dans un fichier.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Tâches par ordre de traitement :
 1. [HAUTE  ] Livrer l'animation au client           2026-10-27 (EN RETARD de 1 j)
 2. [HAUTE  ] Réviser électronique numérique         2026-10-31 (dans 3 j)
 3. [MOYENNE] Pousser le code du jour                2026-10-28 (aujourd'hui)
 4. [MOYENNE] Commander le stock d'écouteurs         2026-11-04 (dans 7 j)
 5. [BASSE  ] Répondre aux messages WhatsApp         2026-10-28 (aujourd'hui)

À faire maintenant : Livrer l'animation au client
```
