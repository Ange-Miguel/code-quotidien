# Jour 143 — Compteur d'énergie : kWh cumulés et coût en FCFA

**Langue :** Python

## Le problème
Un compteur d'énergie connecté (module PZEM-004T sur ESP32, par exemple) mesure la puissance instantanée en watts plusieurs fois par minute. Mais la facture se paie en kilowattheures : il faut intégrer la puissance dans le temps, puis présenter la consommation par heure, par jour et le coût.

## La solution
L'énergie est l'intégrale de la puissance ; on l'approche par la méthode des trapèzes entre deux mesures horodatées (pas forcément régulières : une mesure peut manquer). On cumule par tranche horaire, on repère l'heure de pointe, on applique un tarif par tranches (jour 008) et on projette la facture du mois. La journée de mesures est simulée.

## Ce qu'on apprend
L'intégration numérique (méthode des trapèzes), la différence entre puissance et énergie, l'agrégation par heure et la projection mensuelle.

## Lancer
```bash
python3 compteur_energie.py
```

## Exemple de sortie
```
2822 mesures (une toutes les 30 s, 2 % perdues) sur 24 h

   00h      119 Wh ▇▇
   01h      116 Wh ▇▇
   02h      115 Wh ▇▇
   03h      124 Wh ▇▇
   04h      107 Wh ▇▇
   05h      123 Wh ▇▇
   06h     1023 Wh ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
   07h     1016 Wh ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
   08h      117 Wh ▇▇
   09h      114 Wh ▇▇
   10h      115 Wh ▇▇
   11h      125 Wh ▇▇
   12h     1218 Wh ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇  ← pointe
```
