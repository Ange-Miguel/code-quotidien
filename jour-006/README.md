# Jour 006 — Vérificateur d'année bissextile et nombre de jours du mois

**Langue :** Java

## Le problème
Un échéancier, un calendrier de paiement ou un horodatage de capteur doit savoir si février a 28 ou 29 jours.

## La solution
Une année est bissextile si elle est divisible par 4, sauf les siècles, qui doivent être divisibles par 400 (2000 oui, 1900 non). On en déduit le nombre de jours de chaque mois et de l'année.

## Ce qu'on apprend
Les opérateurs logiques, le modulo, le switch moderne de Java et la comparaison avec la bibliothèque java.time pour vérifier notre résultat.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
1900 : non bissextile (365 jours)
2000 : bissextile (366 jours)
2024 : bissextile (366 jours)
2026 : non bissextile (365 jours)
2028 : bissextile (366 jours)

Jours par mois en 2028 :
  janvier    31
  février    29
  mars       31
  avril      30
  mai        31
  juin       30
  juillet    31
  août       31
```
