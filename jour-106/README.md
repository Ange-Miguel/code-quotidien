# Jour 106 — Conversion ADC : valeur brute 10 bits → tension → température (LM35)

**Langue :** C

## Le problème
Un Arduino lit un capteur de température LM35 sur une entrée analogique et obtient un nombre entre 0 et 1023, pas des degrés. Il faut convertir correctement, sans nombres à virgule si possible (certains petits microcontrôleurs n'ont pas d'unité de calcul flottant).

## La solution
Tension (mV) = brut × Vref (mV) / 1023 ; le LM35 donne 10 mV par °C, donc T (°C) = tension / 10. On compare un calcul en virgule flottante et un calcul en entiers en dixièmes de degré (arrondi correct), on étudie la résolution (pas d'un degré par point avec Vref = 5 V, bien meilleur avec la référence interne de 1,1 V) et on suréchantillonne pour gagner en précision.

## Ce qu'on apprend
Le fonctionnement d'un convertisseur analogique-numérique, la résolution, l'arithmétique en virgule fixe et l'arrondi entier.

## Lancer
```bash
gcc -Wall -Wextra -o adc_lm35 adc_lm35.c -lm
./adc_lm35
```

## Exemple de sortie
```
Vref = 5000 mV : résolution = 4.89 mV = 0.49 °C par point
  T réelle  brut  T flottante  T entière
    18.0      37      18.08      18,1
    25.3      52      25.42      25,4
    30.0      61      29.81      29,8
    37.6      77      37.63      37,6
    52.0     106      51.81      51,8

Vref = 1100 mV : résolution = 1.08 mV = 0.11 °C par point
  T réelle  brut  T flottante  T entière
    18.0     167      17.96      18,0
    25.3     235      25.27      25,3
    30.0     279      30.00      30,0
    37.6     350      37.63      37,6
    52.0     484      52.04      52,0
```
