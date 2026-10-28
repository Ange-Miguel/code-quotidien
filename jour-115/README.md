# Jour 115 — Parseur de trames NMEA GPS ($GPGGA)

**Langue :** C

## Le problème
Un module GPS (NEO-6M, NEO-M8N) envoie sur sa liaison série des lignes de texte au format NMEA 0183, par exemple « $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47 ». Pour un traceur de moto ou de colis, il faut en extraire l'heure, la position en degrés décimaux, l'altitude et la qualité du signal.

## La solution
On vérifie la somme de contrôle (XOR de tous les caractères entre « $ » et « * », comparé aux deux chiffres hexadécimaux finaux), on découpe les champs en gérant les champs vides (strsep, contrairement à strtok), et on convertit le format ddmm.mmmm en degrés décimaux.

## Ce qu'on apprend
Le format NMEA, les sommes de contrôle XOR, strsep et les champs vides, et la conversion degrés-minutes → degrés décimaux.

## Lancer
```bash
gcc -Wall -Wextra -o nmea_gga nmea_gga.c -lm
./nmea_gga
```

## Exemple de sortie
```
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   12:35:19 UTC — 48.117300, 11.516667 — altitude 545.4 m — 8 satellites, HDOP 0.9
   carte : https://www.openstreetmap.org/?mlat=48.11730&mlon=11.51667

$GPGGA,081532,0621.924,N,00225.098,E,1,09,1.1,12.3,M,24.1,M,,*7F
   08:15:32 UTC — 6.365400, 2.418300 — altitude 12.3 m — 9 satellites, HDOP 1.1
   carte : https://www.openstreetmap.org/?mlat=6.36540&mlon=2.41830

$GPGGA,081533,,,,,0,03,,,M,,M,,*69
   08:15:33 UTC — pas de position (recherche de satellites, 3 en vue)

$GPGGA,081532,0621.994,N,00225.098,E,1,09,1.1,12.3,M,24.1,M,,*7F
   ✘ somme de contrôle invalide : trame rejetée
```
