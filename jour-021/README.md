# Jour 021 — Simulateur de remboursement de prêt (mensualités)

**Langue :** Java

## Le problème
Avant d'emprunter pour une moto, du matériel ou un stock de marchandises, il faut connaître la mensualité et le coût total du crédit.

## La solution
Formule de l'annuité constante : M = C × t / (1 − (1 + t)^−n), avec t le taux mensuel et n le nombre de mois. On construit ensuite le tableau d'amortissement : à chaque mois, intérêts = capital restant × t, et le reste de la mensualité rembourse le capital.

## Ce qu'on apprend
Math.pow, les tableaux d'amortissement, l'arrondi monétaire et l'affichage en colonnes alignées.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Prêt de 1 200 000 FCFA à 12,00 % sur 12 mois
  Mensualité : 106 619 FCFA
  Coût total du crédit : 79 423 FCFA

  Mois   Mensualité     Intérêts      Capital       Reste dû
     1      106 619       12 000       94 619      1 105 381
     2      106 619       11 054       95 565      1 009 817
     3      106 619       10 098       96 520        913 296
     4      106 619        9 133       97 486        815 811
     5      106 619        8 158       98 460        717 350
     6      106 619        7 174       99 445        617 905
     7      106 619        6 179      100 439        517 466
     8      106 619        5 175      101 444        416 022
     9      106 619        4 160      102 458        313 564
    10      106 619        3 136      103 483        210 081
```
