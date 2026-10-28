# Jour 039 — Calcul de factorielle et nombre de combinaisons possibles

**Langue :** Java

## Le problème
Combien de façons de choisir 5 numéros sur 90 au loto ? De ranger 8 produits sur une étagère ? De former un bureau de 3 personnes parmi 12 ? Ces nombres explosent vite et dépassent la capacité des types int et long.

## La solution
Factorielle avec BigInteger (précision illimitée), arrangements A(n, k) = n!/(n−k)! et combinaisons C(n, k) calculées sans grande factorielle, en multipliant et divisant pas à pas. On montre où « long » déborde.

## Ce qu'on apprend
BigInteger, le dépassement de capacité, Math.multiplyExact et le dénombrement (permutations, arrangements, combinaisons).

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
5! = 120
10! = 3628800
20! = 2432902008176640000
25! = 15511210043330985984000000
Le type long déborde à 21!

Loto 5 numéros sur 90 : 43 949 268 combinaisons
Ranger 8 produits sur une étagère : 40 320 ordres
Président, secrétaire, trésorier parmi 12 : 1 320 bureaux
Équipe de 3 sans rôle parmi 12 : 220 équipes
```
