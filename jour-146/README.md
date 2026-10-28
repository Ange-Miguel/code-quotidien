# Jour 146 — Chiffrement XOR et clé partagée pour une trame IoT

**Langue :** Python

## Le problème
Une trame radio (433 MHz, LoRa) circule en clair : n'importe qui avec un récepteur à 20 000 FCFA peut lire la température, ou pire, la commande « ouvrir le portail ». Comprendre le chiffrement le plus simple, le XOR, aide à voir ce qui protège vraiment… et ce qui ne protège pas.

## La solution
Chiffrer = faire un XOR de chaque octet avec un flux de clé ; déchiffrer = refaire exactement le même XOR (car (m ^ k) ^ k = m). On montre trois versions : (1) clé courte répétée : cassable dès qu'on connaît un morceau du message ; (2) flux de clé dérivé d'une clé secrète et d'un compteur (nonce) différent pour chaque trame, via SHA-256 ; (3) le danger de réutiliser le même flux : le XOR de deux messages chiffrés révèle le XOR des messages clairs. Pour un vrai produit, il faut un algorithme standard (AES-CCM, ChaCha20-Poly1305), jamais un chiffrement maison.

## Ce qu'on apprend
L'opération XOR, les flux de clé, le rôle du nonce, une attaque par texte clair connu, et pourquoi la cryptographie maison est dangereuse.

## Lancer
```bash
python3 chiffrement_xor.py
```

## Exemple de sortie
```
Trame en clair      : PORTAIL:OUVRIR;code=4412

1) Clé courte répétée « K3y » : 1b7c2b1f72300709361e652b026142285c1d2e0e4d7f024b
   L'attaquant devine « PORTAIL: » et retrouve la clé : K3y…
   Message déchiffré par l'attaquant : PORTAIL:OUVRIR;code=4412

2) Flux SHA-256, nonce 1 : ca6c3d6e92e929089c732c5f648180dbb9163532a0fc30af
2) Flux SHA-256, nonce 2 : c39a79cb97c21943e10ce9598c754827e5f27ba26da74610
   Le récepteur (même secret + nonce) retrouve : PORTAIL:OUVRIR;code=4412
   Même message, nonce différent → chiffré totalement différent.

3) Erreur grave : réutiliser le nonce 7 pour deux trames
   c1 XOR c2 = clair1 XOR clair2 : 00000000000000000910041f0c0000000000000000000000
   Les octets nuls révèlent les parties identiques des deux messages : ========?????===========

```
