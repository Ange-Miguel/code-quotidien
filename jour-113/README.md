# Jour 113 — Calcul CRC-16 Modbus

**Langue :** Python

## Le problème
Modbus RTU est le protocole le plus répandu dans l'industrie : compteurs d'énergie, variateurs, onduleurs solaires, automates. Chaque trame série se termine par un CRC-16 de 2 octets (octet de poids faible en premier). Pour dialoguer avec un compteur d'énergie depuis un ESP32, il faut savoir construire et vérifier ces trames.

## La solution
CRC-16 Modbus = polynôme 0xA001 (forme réfléchie de 0x8005), valeur initiale 0xFFFF. On l'implémente, on le vérifie (valeur de contrôle 0x4B37 pour « 123456789 »), puis on construit une vraie requête « lire des registres de maintien » (fonction 0x03) et on décode une réponse simulée de compteur d'énergie, en rejetant une réponse corrompue.

## Ce qu'on apprend
Le CRC-16, l'ordre des octets (petit-boutiste / gros-boutiste), le module struct et la structure d'une trame Modbus RTU.

## Lancer
```bash
python3 crc16_modbus.py
```

## Exemple de sortie
```
Contrôle : CRC("123456789") = 0x4B37 (attendu 0x4B37)

Requête « lire 3 registres à partir de 0 » vers le compteur n°1 :
   01 03 00 00 00 03 05 CB
Réponse reçue :
   01 03 06 09 00 02 0D 04 9C B3 3E
   → tension 230.4 V, courant 5.25 A, puissance 1180 W
Réponse corrompue (1 bit) :
   01 03 06 09 10 02 0D 04 9C B3 3E → rejetée : CRC invalide
```
