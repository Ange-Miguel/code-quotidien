# Jour 112 — Calcul CRC-8 pour trame de capteur

**Langue :** C

## Le problème
Des capteurs comme le DS18B20 (bus 1-Wire) ou le SHT31 (I2C) ajoutent un octet CRC-8 à chaque mesure. Si le câble est long ou bruité, un bit peut s'inverser : il faut recalculer le CRC pour rejeter les mesures corrompues au lieu d'afficher une température absurde.

## La solution
Le CRC-8 est le reste de la division polynomiale des données par un polynôme générateur (0x31 pour Sensirion, 0x31 réfléchi = 0x8C pour Dallas/Maxim). On l'implémente bit par bit, puis avec une table de 256 entrées (plus rapide), et on vérifie avec les valeurs de référence des fiches techniques. On montre qu'il détecte toutes les erreurs d'un bit.

## Ce qu'on apprend
Le principe du CRC, les décalages et XOR, les variantes (polynôme, valeur initiale, réflexion) et la vérification d'une trame.

## Lancer
```bash
gcc -Wall -Wextra -o crc8 crc8.c -lm
./crc8
```

## Exemple de sortie
```
Vérifications de référence :
  Sensirion(BE EF) = 0x92 (fiche technique : 0x92)
  Maxim("123456789") = 0xA1 (valeur de contrôle : 0xA1)

Trames reçues d'un SHT31 :
  SHT31 66 26 CRC AE → calculé AE : OK, température 24.83 °C
  SHT31 66 2E CRC AE → calculé 17 : CORROMPU

Adresse DS18B20 avec CRC : 28 FF 4C 1A 6E 18 01 36 
Erreurs d'un bit détectées : 56/56
```
