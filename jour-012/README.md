# Jour 012 — Compteur de mots, lignes et caractères d'un texte

**Langue :** Java

## Le problème
Un devoir, une description de produit ou un post a souvent une limite de mots ou de caractères ; il faut pouvoir compter précisément.

## La solution
On lit un fichier (ou un texte d'exemple), on compte les lignes, les mots (séquences séparées par des espaces), les caractères avec et sans espaces, et on estime le temps de lecture (200 mots par minute).

## Ce qu'on apprend
La lecture de fichiers avec java.nio, les expressions régulières pour découper un texte et la gestion des exceptions d'E/S.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Texte : exemple intégré
  Lignes                : 3
  Mots                  : 25
  Caractères            : 172
  Caractères sans espace: 148
  Lecture estimée       : 0,1 min
```
