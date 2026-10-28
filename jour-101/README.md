# Jour 101 — Manipulation de bits : activer, désactiver, basculer un bit de registre

**Langue :** Python

## Le problème
Sur un microcontrôleur (AVR, STM32, ESP32), on allume une LED, on active une interruption ou on règle une broche en écrivant des bits précis dans un registre de 8, 16 ou 32 bits, sans toucher aux autres bits.

## La solution
Les quatre opérations de base, avec un masque (1 << n) : activer = reg | masque, désactiver = reg & ~masque, basculer = reg ^ masque, lire = (reg >> n) & 1. On simule le registre PORTB d'un AVR (8 bits) qui pilote des LED, et on affiche le registre en binaire après chaque opération. Python n'a pas d'entiers de taille fixe : on applique & 0xFF pour rester sur 8 bits, comme le ferait le matériel.

## Ce qu'on apprend
Les opérateurs bit à bit (|, &, ^, ~, <<, >>), les masques, la lecture-modification-écriture d'un registre et le piège des entiers Python.

## Lancer
```bash
python3 bits_registre.py
```

## Exemple de sortie
```
départ                 PORTB = 0b00000000 = 0x00 =   0   LED7→0 : ○○○○○○○○
activer bit 0          PORTB = 0b00000001 = 0x01 =   1   LED7→0 : ○○○○○○○●
activer bit 3          PORTB = 0b00001001 = 0x09 =   9   LED7→0 : ○○○○●○○●
activer bit 7          PORTB = 0b10001001 = 0x89 = 137   LED7→0 : ●○○○●○○●
basculer bit 3         PORTB = 0b10000001 = 0x81 = 129   LED7→0 : ●○○○○○○●
basculer bit 1         PORTB = 0b10000011 = 0x83 = 131   LED7→0 : ●○○○○○●●
desactiver bit 7       PORTB = 0b00000011 = 0x03 =   3   LED7→0 : ○○○○○○●●
champ bits 4-6 ← 101   PORTB = 0b01010011 = 0x53 =  83   LED7→0 : ○●○●○○●●

Lecture : bit 0 = 1, bit 3 = 0, bit 5 = 0
Piège Python : ~0b00000001 vaut -2 ; avec & 0xFF on obtient 0b11111110
```
