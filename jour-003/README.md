# Jour 003 — Calculateur de moyenne semestrielle avec coefficients (notes IFRI)

**Langue :** Java

## Le problème
À l'IFRI comme ailleurs, la moyenne du semestre dépend des coefficients des matières ; la calculer à la main est source d'erreurs.

## La solution
Chaque matière est un objet (nom, note, coefficient). La moyenne pondérée vaut somme(note × coef) / somme(coef). On affiche aussi la mention et les matières sous 10 à rattraper.

## Ce qu'on apprend
Les records Java, les listes, les boucles, la moyenne pondérée et la lecture d'arguments au format nom:note:coef.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Matière                     Note  Coef
Électronique numérique     13,50     4
Programmation C            15,00     3
Mathématiques               9,00     3
Réseaux                    12,00     2
Anglais                    14,00     1

Moyenne semestrielle : 12,62 / 20 — Assez bien
À rattraper : Mathématiques
```
