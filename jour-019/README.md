# Jour 019 — Conversion de durée : secondes → h/min/s

**Langue :** C

## Le problème
Un compteur de microcontrôleur, un chronomètre ou un journal de serveur donne souvent une durée en secondes (« uptime 1 234 567 s »), peu lisible pour un humain.

## La solution
Divisions entières successives par 86 400, 3 600 et 60, avec le reste à chaque étape. On fait aussi l'opération inverse : « 2j3h15m » vers des secondes, avec un petit analyseur de texte.

## Ce qu'on apprend
La division entière et le modulo, les types entiers longs, et l'écriture d'un mini-analyseur caractère par caractère.

## Lancer
```bash
gcc -Wall -Wextra -o durees durees.c -lm
./durees
```

## Exemple de sortie
```
59 s = 00 h 00 min 59 s
        3600 s = 01 h 00 min 00 s
       86399 s = 23 h 59 min 59 s
     1234567 s = 14 j 06 h 56 min 07 s
    31536000 s = 365 j 00 h 00 min 00 s
2j3h15m    →       184500 s = 2 j 03 h 15 min 00 s
90m        →         5400 s = 01 h 30 min 00 s
1h30m45s   →         5445 s = 01 h 30 min 45 s
```
