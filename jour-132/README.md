# Jour 132 — Décodage d'une commande JSON reçue d'un serveur

**Langue :** Java

## Le problème
Une passerelle domotique reçoit des ordres de l'application mobile sous forme JSON, par exemple {"appareil":"lampe_salon","action":"on", "luminosite":70}. Elle doit les lire de façon sûre : champs manquants, mauvais types, valeurs hors limites ou texte malformé ne doivent jamais la faire planter ni allumer n'importe quoi.

## La solution
On écrit un petit analyseur JSON (sans bibliothèque externe, ce qui est fréquent sur une petite passerelle) : objets, tableaux, chaînes avec échappements, nombres, booléens et null, avec la position de l'erreur. On valide ensuite la commande avec des règles explicites (liste blanche d'appareils et d'actions, bornes) avant de l'exécuter.

## Ce qu'on apprend
L'analyse syntaxique récursive d'un format réel, les types dynamiques en Java (Object, Map, List), la validation stricte des entrées et le principe de « liste blanche ».

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
{"appareil":"lampe_salon","action":"on","luminosite":70}
   → OK : lampe_salon → on à 70 %
{ "appareil" : "ventilateur", "action" : "off" }
   → OK : ventilateur → off
{"appareil":"four","action":"on"}
   → REJET : appareil inconnu ou absent
{"appareil":"lampe_chambre","action":"on","luminosite":250}
   → REJET : luminosité entière entre 0 et 100 attendue
{"appareil":"portail","action":"ouvrir"}
   → REJET : action invalide
{"appareil":"portail","action":"on"
   → REJET (JSON) : « } » attendu (position 35)
["lampe_salon","on"]
   → REJET : un objet JSON est attendu
{"appareil":"lampe_salon","action":"basculer","note":"Caf\u00e9 \"ouvert\""}
```
