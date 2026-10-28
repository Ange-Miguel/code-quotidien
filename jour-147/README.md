# Jour 147 — Authentification HMAC simple d'un message capteur

**Langue :** Java

## Le problème
Chiffrer cache le contenu, mais n'empêche pas un attaquant de modifier une trame ou de rejouer une ancienne commande « ouvrir le portail » enregistrée la veille. Il faut prouver que le message vient bien de l'appareil légitime, qu'il n'a pas été modifié et qu'il est récent.

## La solution
Un code d'authentification HMAC-SHA256 calculé avec une clé secrète partagée sur (identifiant, compteur, horodatage, contenu). Le récepteur recalcule le HMAC et le compare en temps constant ; il refuse un compteur déjà vu (rejeu) ou un horodatage trop ancien. On tronque le HMAC à 8 octets pour économiser la bande passante radio, un compromis courant.

## Ce qu'on apprend
Javax.crypto.Mac et HMAC, la comparaison en temps constant (MessageDigest.isEqual), la protection contre le rejeu et la troncature.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Message signé : telecommande-01|41|1793174400|OUVRIR_PORTAIL | tag cd9e0d749a470ba4

Message légitime                       → ACCEPTÉ : OUVRIR_PORTAIL
Rejeu du même message 10 s plus tard   → REFUSÉ : rejeu détecté (compteur déjà utilisé)
Contenu modifié par un attaquant       → REFUSÉ : signature invalide (message modifié ou faux)
Faux message sans la clé               → REFUSÉ : signature invalide (message modifié ou faux)
Message enregistré hier puis renvoyé   → REFUSÉ : message trop ancien
Nouveau message légitime               → ACCEPTÉ : FERMER_PORTAIL

Tag tronqué à 8 octets : un faussaire a 1 chance sur 2^64 de deviner au hasard.
```
