# Jour 084 — Encodage / décodage Base64

**Langue :** Java

## Le problème
On ne peut pas mettre directement des octets binaires (image, clé, trame de capteur) dans un e-mail, une URL ou un message JSON : ces canaux n'acceptent que du texte. Base64 transforme n'importe quels octets en caractères sûrs (A-Z, a-z, 0-9, + et /).

## La solution
On regroupe les octets par 3 (24 bits) et on découpe ces 24 bits en 4 groupes de 6 bits, chacun devenant un caractère de l'alphabet de 64 symboles. Le signe « = » complète le dernier bloc. On écrit l'encodeur et le décodeur à la main avec des décalages de bits, et on vérifie contre java.util.Base64.

## Ce qu'on apprend
Les opérations sur les bits (<<, >>, &, |), les octets signés en Java, le remplissage (padding) et la vérification par la bibliothèque.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
"Man" (3 octets, sans =)           → TWFu
"Ma" (2 octets, un =)              → TWE=
"M" (1 octet, deux =)              → TQ==
"Bénin 2026 ✓" (UTF-8)             → QsOpbmluIDIwMjYg4pyT
trame capteur binaire A5 01 2C FF 00 7E → pQEs/wB+
Décodage de "SGVsbG8gQ2FsYXZpICE=" → Hello Calavi !
Taille : Base64 ajoute environ 33 % (4 caractères pour 3 octets).
```
