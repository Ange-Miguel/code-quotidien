# Jour 094 — Rappel d'anniversaires à partir d'un fichier

**Langue :** C

## Le problème
On oublie l'anniversaire d'un ami, d'un parent ou d'un client fidèle. Un petit programme lancé chaque matin pourrait prévenir à l'avance.

## La solution
Un fichier texte contient une ligne par personne : « JJ/MM/AAAA;Nom ». On lit le fichier, on calcule pour chacun le nombre de jours avant le prochain anniversaire (en tenant compte du passage d'année et du 29 février) et l'âge qu'il aura. On trie par proximité et on affiche ceux des 30 prochains jours. La date du jour peut être imposée pour tester.

## Ce qu'on apprend
Sscanf pour analyser des lignes, la structure tm et mktime de la bibliothèque C pour les calculs de dates, et qsort sur des structures.

## Lancer
```bash
gcc -Wall -Wextra -o anniversaires anniversaires.c -lm
./anniversaires
```

## Exemple de sortie
```
Démo avec la date du 28/10/2026
Anniversaires dans les 30 prochains jours :
  Maman                      02/11  bientôt (54 ans)
  Serge                      15/11  bientôt (25 ans)
Prochains ensuite :
  Awa                        dans  58 jours
  Inès                      dans  74 jours
  Kofi (né un 29 février)  dans 123 jours
  Ange-Miguel                dans 137 jours
  Carine                     dans 342 jours
  ligne 7 ignorée : mauvaise ligne
```
