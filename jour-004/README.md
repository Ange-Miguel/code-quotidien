# Jour 004 — Convertisseur de températures Celsius / Fahrenheit / Kelvin

**Langue :** C

## Le problème
Les fiches techniques de capteurs (DHT11, LM35, DS18B20) et les sites étrangers n'utilisent pas tous la même unité de température.

## La solution
On convertit d'abord la valeur en Celsius (unité pivot), puis vers les deux autres unités. Une température sous le zéro absolu est refusée.

## Ce qu'on apprend
Les énumérations, le switch, les formules de conversion et la détection de valeurs physiquement impossibles.

## Lancer
```bash
gcc -Wall -Wextra -o temperatures temperatures.c -lm
./temperatures
```

## Exemple de sortie
```
Démo : températures courantes
   25.00 °C |    77.00 °F |   298.15 K
   37.00 °C |    98.60 °F |   310.15 K
   37.00 °C |    98.60 °F |   310.15 K
   26.85 °C |    80.33 °F |   300.00 K
Erreur : température sous le zéro absolu.
```
