# Jour 114 — Encodage et décodage d'une trame série personnalisée

**Langue :** Java

## Le problème
Une carte capteur envoie ses mesures à une passerelle par liaison série (UART) ou radio. Les octets arrivent en flux continu, parfois coupés ou avec du bruit : comment savoir où commence une trame, combien elle contient d'octets et si elle est intacte ?

## La solution
On définit un format : [0xAA][0x55][longueur][type][données…] [somme XOR]. Le décodeur est une machine à états qui lit octet par octet : recherche de l'en-tête, lecture de la longueur, des données, vérification du contrôle. Il se resynchronise tout seul après une erreur. Les mesures (température, humidité, batterie) sont encodées en entiers signés de 16 bits, gros-boutistes, avec ByteBuffer.

## Ce qu'on apprend
La conception d'un protocole, ByteBuffer et l'ordre des octets, le décodage en flux par machine à états et la resynchronisation.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Trame 1 : AA 55 08 01 03 0B 1F 1B DA 0E 80 59
Flux reçu : 56 octets (bruit, trame abîmée, trame coupée)
  ✔ capteur 3 : 28,47 °C, 71,30 % HR, batterie 3712 mV
  ✔ capteur 7 : -4,25 °C, 40,00 % HR, batterie 3305 mV
  ✘ trames rejetées : 2
```
