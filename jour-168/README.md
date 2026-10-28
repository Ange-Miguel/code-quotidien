# Jour 168 — Budget personnel 50/30/20

**Langue :** Java

## Le problème
Un étudiant ou un jeune actif reçoit un revenu (bourse, salaire, missions freelance) qui fond sans qu'il sache où. La règle 50/30/20 est un repère simple : 50 % pour les besoins (logement, nourriture, transport), 30 % pour les envies (sorties, vêtements), 20 % pour l'épargne et les dettes.

## La solution
Chaque dépense du mois est rangée dans une catégorie, elle-même associée à un des trois groupes. On compare la répartition réelle à la cible, on affiche les écarts en barres, et on propose des ajustements concrets : réduire en priorité les postes « envies » les plus importants pour revenir à 20 % d'épargne. Montants d'exemple.

## Ce qu'on apprend
Les enums avec attributs, le regroupement avec les Collectors (groupingBy, summingInt), les pourcentages et la génération de conseils.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Revenu du mois : 150 000 FCFA

Groupe          Réel      %  Cible
BESOINS       83 000    55%    50%  ██████████████████████  trop élevé
ENVIES        50 000    33%    30%  █████████████  trop élevé
EPARGNE       17 000    11%    20%  █████  insuffisant

Pour atteindre 20 % d'épargne (30 000 FCFA), il manque 13 000 FCFA. Pistes :
   - réduire « maquis, cinéma » de 9 000 FCFA
   - réduire « chaussures » de 4 000 FCFA
```
