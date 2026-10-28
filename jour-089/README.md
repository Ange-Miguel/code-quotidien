# Jour 089 — Analyse d'un fichier journal (log) : erreurs par heure

**Langue :** Python

## Le problème
Un serveur, une passerelle IoT ou une application écrit des milliers de lignes de journal. Quand quelque chose ne va pas, il faut rapidement savoir quand les erreurs ont eu lieu, lesquelles reviennent le plus et si un pic est apparu.

## La solution
Chaque ligne est analysée avec une expression régulière (date, heure, niveau, module, message). On compte les niveaux par heure, on regroupe les messages d'erreur en remplaçant les nombres variables par « N » pour reconnaître les erreurs identiques, et on signale les heures dont le nombre d'erreurs dépasse la moyenne + 2 écarts-types.

## Ce qu'on apprend
Les expressions régulières avec groupes nommés, Counter, la normalisation de messages et la détection simple d'anomalies.

## Lancer
```bash
python3 analyse_logs.py
```

## Exemple de sortie
```
1052 lignes, 1 illisible(s)

Heure  INFO  WARN  ERROR
  00h    29     2      1 █
  01h    45     4      1 █
  02h    47     1      4 ████
  03h    44     7      4 ████
  04h    46     2      3 ███
  05h    27     1      2 ██
  06h    30     2      0 
  07h    28     3      2 ██
  08h    51     0      3 ███
  09h    36     0      0 
  10h    40     4      1 █
  11h    44     6      3 ███
```
