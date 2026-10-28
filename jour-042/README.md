# Jour 042 — Calculatrice en ligne de commande (4 opérations + priorités simples)

**Langue :** Java

## Le problème
Évaluer une expression comme « 2 + 3 * (4 - 1) / 2 » en respectant la priorité des opérateurs et les parenthèses, comme le fait une vraie calculatrice.

## La solution
Un analyseur à descente récursive. La grammaire a trois niveaux : expression = terme (+|- terme)* ; terme = facteur (*|/|% facteur)* ; facteur = nombre | -facteur | ( expression ). Chaque niveau est une méthode, ce qui encode naturellement la priorité.

## Ce qu'on apprend
L'analyse syntaxique, la récursivité mutuelle, la gestion d'erreurs précises (position de l'erreur) et la division par zéro.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
2 + 3 * 4                = 14
(2 + 3) * 4              = 20
10 / 4 - 1.5             = 1
-(3 - 8) * 2             = 10
17 % 5                   = 2
2 * (3 + (4 - 1)) / 3    = 4
5 / (2 - 2)              → erreur : division par zéro à la position 8 : 5/(2-2)
3 + * 2                  → erreur : nombre attendu à la position 3 : 3+*2
```
