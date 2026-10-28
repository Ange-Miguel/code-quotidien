# Jour 110 — Simulation d'un PWM : rapport cyclique et vitesse d'un moteur

**Langue :** Python

## Le problème
Un microcontrôleur ne sait sortir que 0 V ou 5 V. Comment faire varier la vitesse d'un ventilateur, la luminosité d'une LED ou la position d'un servomoteur ? Avec la modulation de largeur d'impulsion (PWM) : on alterne très vite 0 et 5 V, et c'est la proportion du temps à 5 V (le rapport cyclique) qui compte.

## La solution
On génère le signal PWM pour plusieurs rapports cycliques, on calcule la tension moyenne (rapport × 5 V), puis on simule un moteur à courant continu (modèle du premier ordre : il « lisse » le signal grâce à son inertie) et on observe sa vitesse se stabiliser. On calcule aussi les registres d'un timer (prédiviseur, valeur TOP) pour obtenir une fréquence donnée sur un microcontrôleur à 16 MHz.

## Ce qu'on apprend
Le PWM, le rapport cyclique, la tension moyenne, le filtrage naturel par l'inertie et le réglage des timers d'un microcontrôleur.

## Lancer
```bash
python3 pwm.py
```

## Exemple de sortie
```
Rapport  Signal (2 périodes)                        V moy   Vitesse
     0%  ________________________________________   0.00V       0 tr/min
    25%  █████_______________█████_______________   1.25V     732 tr/min
    50%  ██████████__________██████████__________   2.50V    1437 tr/min
    75%  ███████████████_____███████████████_____   3.75V    2234 tr/min
   100%  ████████████████████████████████████████   5.00V    3000 tr/min

Démarrage à 50 % (le moteur lisse le signal grâce à son inertie) :
   période  0 :    221 tr/min ▇▇
   période  8 :   1118 tr/min ▇▇▇▇▇▇▇▇▇▇▇
   période 16 :   1353 tr/min ▇▇▇▇▇▇▇▇▇▇▇▇▇
   période 24 :   1415 tr/min ▇▇▇▇▇▇▇▇▇▇▇▇▇▇
   période 32 :   1432 tr/min ▇▇▇▇▇▇▇▇▇▇▇▇▇▇
   période 40 :   1436 tr/min ▇▇▇▇▇▇▇▇▇▇▇▇▇▇
   période 48 :   1437 tr/min ▇▇▇▇▇▇▇▇▇▇▇▇▇▇
```
