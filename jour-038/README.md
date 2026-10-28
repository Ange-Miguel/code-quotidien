# Jour 038 — Suite de Fibonacci et croissance d'une population

**Langue :** Python

## Le problème
Comment évolue une population qui se reproduit (le célèbre problème des lapins de Fibonacci), et pourquoi certaines méthodes de calcul deviennent inutilisables quand n grandit ?

## La solution
On compare trois façons de calculer F(n) : récursive naïve (exponentielle), récursive avec mémoïsation, et itérative. On mesure le nombre d'appels et le temps. On montre que F(n+1)/F(n) tend vers le nombre d'or, puis on simule un élevage de poulets avec un taux de mortalité.

## Ce qu'on apprend
La récursivité, la mémoïsation (functools.lru_cache), la complexité exponentielle contre linéaire et la convergence d'une suite.

## Lancer
```bash
python3 fibonacci.py
```

## Exemple de sortie
```
F(10) = 55 | naïf : 177 appels  0.0 ms | rapide : 0.005 ms
F(20) = 6765 | naïf : 21 891 appels  0.9 ms | rapide : 0.004 ms
F(25) = 75025 | naïf : 242 785 appels  10.9 ms | rapide : 0.007 ms

Rapport F(n+1)/F(n) → nombre d'or φ ≈ 1,6180339887
   n =  5 : 1.6000000000
   n = 10 : 1.6181818182
   n = 20 : 1.6180339985
   n = 40 : 1.6180339887

Élevage : 1 couple(s) au départ, mortalité 0%/mois
   mois  3 :      3 couples
   mois  6 :     13 couples
   mois  9 :     55 couples
   mois 12 :    233 couples
```
