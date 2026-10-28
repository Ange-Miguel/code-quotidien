# Jour 107 — Simulation d'un thermostat avec hystérésis

**Langue :** Python

## Le problème
Un thermostat simple allume la climatisation dès que la température dépasse 26 °C et l'éteint dès qu'elle passe en dessous. Résultat : autour de 26 °C, le compresseur s'allume et s'éteint des dizaines de fois par heure (« battement »), ce qui l'use et consomme beaucoup.

## La solution
L'hystérésis : deux seuils au lieu d'un. On allume au-dessus de 26,5 °C et on n'éteint qu'en dessous de 25,5 °C. Entre les deux, on garde l'état précédent. On simule une pièce (chaleur extérieure, refroidissement par le climatiseur, bruit du capteur) et on compte les commutations et l'énergie consommée avec et sans hystérésis.

## Ce qu'on apprend
L'hystérésis, la simulation physique pas à pas (modèle thermique simple), l'effet du bruit sur une décision à seuil et la comparaison de deux stratégies de commande.

## Lancer
```bash
python3 thermostat.py
```

## Exemple de sortie
```
Consigne 26 °C, 10 heures simulées, capteur bruité (±0,15 °C)

Hystérésis  Commutations  Clim (h)   T min   T max
     0.0 °C           287       6.5   25.74   26.35
     0.5 °C            87       6.6   25.70   26.46
     1.0 °C            37       6.5   25.40   26.58
     2.0 °C            17       6.5   24.96   27.16

Extrait avec hystérésis de 1 °C (█ = clim allumée) :
   02h00  25.91 °C  █
   02h04  25.70 °C  █
   02h08  25.50 °C  █
   02h12  25.55 °C  ·
   02h16  25.83 °C  ·
   02h20  26.10 °C  ·
```
