# Jour 085 — Calcul de somme de contrôle (checksum) d'un fichier

**Langue :** C

## Le problème
Un fichier téléchargé, un firmware envoyé à une carte ou une trame reçue par radio a-t-il été corrompu en route ? Il faut une « empreinte » qui change si un seul bit change.

## La solution
On implémente trois sommes de contrôle de puissance croissante : somme simple sur 8 bits, Fletcher-16 (sensible à l'ordre des octets) et CRC-32 (celui des fichiers ZIP et d'Ethernet, avec une table précalculée). On montre qu'en changeant un seul bit, la somme simple peut ne pas réagir à une permutation alors que CRC-32 change complètement.

## Ce qu'on apprend
La lecture binaire de fichiers (fread), les opérations sur les bits, les entiers non signés de taille fixe (uint32_t) et les tables de précalcul.

## Lancer
```bash
gcc -Wall -Wextra -o checksum checksum.c -lm
./checksum
```

## Exemple de sortie
```
"123456789" (référence)           9 octets | somme 0xDD | Fletcher-16 0x1EDE | CRC-32 0xCBF43926
"Montant: 5000 FCFA"               18 octets | somme 0x30 | Fletcher-16 0xD935 | CRC-32 0xAE42346A
"Montant: 5001 FCFA" (1 bit)       18 octets | somme 0x31 | Fletcher-16 0xDF36 | CRC-32 0x651EE7CF
"Montant: 0500 FCFA" (permuté)     18 octets | somme 0x30 | Fletcher-16 0xD435 | CRC-32 0xAC357F41

La somme simple ne voit pas la permutation 5000 → 0500 ; Fletcher et CRC-32 la détectent.
```
