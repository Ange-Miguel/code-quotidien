# Jour 015 — Convertisseur décimal / binaire / hexadécimal

**Langue :** Java

## Le problème
En électronique et en programmation de microcontrôleurs, on lit des registres en hexadécimal, on règle des bits en binaire et on raisonne en décimal. Il faut passer de l'un à l'autre sans erreur.

## La solution
On accepte un nombre écrit en décimal (42), binaire (0b101010) ou hexadécimal (0x2A). On le convertit nous-mêmes par divisions successives, puis on vérifie le résultat avec les méthodes de la bibliothèque Java.

## Ce qu'on apprend
Les bases de numération, l'algorithme des divisions successives, StringBuilder et l'affichage des bits par groupes de 4.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
42           → déc 42 | hex 0x2A | oct 52 | bin 0010 1010
0xFF         → déc 255 | hex 0xFF | oct 377 | bin 1111 1111
0b1010_0101  → déc 165 | hex 0xA5 | oct 245 | bin 1010 0101
1023         → déc 1023 | hex 0x3FF | oct 1777 | bin 0011 1111 1111
0x3F8        → déc 1016 | hex 0x3F8 | oct 1770 | bin 0011 1111 1000
```
