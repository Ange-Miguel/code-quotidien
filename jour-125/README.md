# Jour 125 — Calcul de consommation électrique des appareils d'une maison

**Langue :** Python

## Le problème
La facture d'électricité monte et on ne sait pas quel appareil en est responsable. Le congélateur ? Le climatiseur ? Le fer à repasser ? Et que rapporterait le passage aux ampoules LED ou un climatiseur inverter ?

## La solution
Chaque appareil est décrit par sa puissance, son usage quotidien et éventuellement sa veille. On calcule les kWh par mois, la part de chaque appareil (diagramme en barres texte) et le coût selon le prix du kWh. Puis on simule des scénarios d'économie et on calcule le temps de retour sur investissement de chaque changement. Prix et puissances sont des exemples.

## Ce qu'on apprend
La différence entre watts et kWh, l'impact de la veille, le calcul d'un retour sur investissement et la comparaison de scénarios.

## Lancer
```bash
python3 consommation_maison.py
```

## Exemple de sortie
```
Consommation actuelle : 388 kWh/mois = 48 495 FCFA/mois
   Climatiseur      198.0 kWh 51.0% ██████████████████████████
   Ampoules          57.6 kWh 14.8% ███████
   Réfrigérateur     45.0 kWh 11.6% ██████
   Ventilateur       28.8 kWh  7.4% ████
   Pompe à eau       15.8 kWh  4.1% ██
   Téléviseur        12.6 kWh  3.2% ██
   Fer à repasser    10.8 kWh  2.8% █
   Ordinateur         9.8 kWh  2.5% █
   Décodeur TV        6.6 kWh  1.7% █
   Chargeurs          3.1 kWh  0.8% 

Scénarios d'économie (prix indicatifs) :
   Ampoules LED (40 W → 9 W)              économie   5 580 FCFA/mois  investissement   12 000 → rentabilisé en  2.2 mois
   Climatiseur inverter (-40 %)           économie   9 900 FCFA/mois  investissement  250 000 → rentabilisé en 25.3 mois
```
