# Jour 105 — Filtre médian contre les valeurs aberrantes d'un capteur

**Langue :** Java

## Le problème
Un capteur à ultrasons (HC-SR04) ou un capteur de distance renvoie parfois une valeur complètement fausse (0 cm, 400 cm) à cause d'un écho parasite. La moyenne glissante du jour 104 est « contaminée » par ces pics.

## La solution
Le filtre médian prend la valeur du milieu des N dernières mesures triées : une valeur aberrante isolée ne peut jamais être choisie. On l'implémente sur une fenêtre glissante de taille impaire, on le compare à la moyenne sur un signal avec pics, et on ajoute un rejet par seuil (écart à la médiane supérieur à k × MAD, l'écart absolu médian).

## Ce qu'on apprend
La médiane, sa robustesse face aux valeurs aberrantes, la fenêtre glissante avec ArrayDeque et une statistique robuste (MAD).

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Distance (cm) — fenêtre de 5 mesures
  t    brute  moyenne  médiane
  0    120,3    120,3    120,3
  1    123,3    121,8    121,8
  2    120,8    121,5    120,8
  3    121,7    121,5    121,2
  4    120,7    121,3    120,8
  5    121,4    121,6    121,4
  6      0,0     96,9    120,8  ← écho parasite
  7    122,8     97,3    121,4
  8    124,2     97,8    121,4
  9    125,6     98,8    122,8
 10    121,5     98,8    122,8
 11    125,7    124,0    124,2
 12    121,7    123,7    124,2
```
