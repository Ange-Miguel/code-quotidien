# Jour 057 — Pile (stack) : vérifier l'équilibre des parenthèses d'une formule

**Langue :** Java

## Le problème
Une formule de tableur, une expression mathématique ou un code source avec une parenthèse, un crochet ou une accolade mal fermé ne fonctionne pas, et l'erreur est difficile à repérer à l'œil.

## La solution
On parcourt le texte ; chaque symbole ouvrant est empilé avec sa position ; chaque symbole fermant doit correspondre au sommet de la pile. À la fin, la pile doit être vide. On indique précisément la position et le type de l'erreur, en ignorant ce qui est entre guillemets.

## Ce qu'on apprend
La structure de pile (LIFO) avec ArrayDeque, les records, les correspondances avec une Map et le signalement précis des erreurs.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
=SOMME(A1:A10)*(B2+C2)         → équilibrée ✔
{ a[i] = (b + c) * d; }        → équilibrée ✔
((2 + 3) * [4 - 1]             → ERREUR : « ( » ouvert en position 1 n'est jamais fermé
      ((2 + 3) * [4 - 1]
      ^
f(x) = [x + 1)                 → ERREUR : « ) » en position 14 ne correspond pas à « [ » ouvert en position 8
      f(x) = [x + 1)
                   ^
=SI(A1>10;"ok (bon";"non")     → équilibrée ✔
a + b)                         → ERREUR : « ) » en position 6 ferme quelque chose qui n'est pas ouvert
      a + b)
           ^
```
