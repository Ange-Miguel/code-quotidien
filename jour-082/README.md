# Jour 082 — Compression RLE d'un texte

**Langue :** C

## Le problème
Un capteur envoie de longues séries de valeurs identiques (« 0000000011111000 ») et une liaison radio LoRa ou série coûte cher en octets. Une compression très simple, peu gourmande en calcul, est utile sur microcontrôleur.

## La solution
Le codage par plages (Run-Length Encoding) remplace chaque série de caractères identiques par le nombre de répétitions suivi du caractère : « AAAABBB » → « 4A3B ». On écrit l'encodeur, le décodeur, on vérifie l'aller-retour et on mesure le taux de compression, y compris le cas défavorable d'un texte sans répétitions (qui grossit !).

## Ce qu'on apprend
Le parcours de chaînes, l'écriture dans un tampon avec snprintf, le décodage d'un format et la notion de taux de compression.

## Lancer
```bash
gcc -Wall -Wextra -o rle rle.c -lm
./rle
```

## Exemple de sortie
```
AAAAAAAAAABBBBBCCCCCCCCCCCCCCCDDDDDD ( 36) → 10A5B15C6D                   ( 10) : -72 %
________....____________....________ ( 36) → 8_4.12_4.8_                  ( 11) : -69 %
BBBBBBBBNNNNNNNBBBBBBBBNNNNNNN       ( 30) → 8B7N8B7N                     (  8) : -73 %
Calavi                               (  6) → 1C1a1l1a1v1i                 ( 12) : +100 %
Le 3 mai                             → impossible (chiffres ou trop long)

Astuce : RLE est efficace sur des images en noir et blanc ou des mesures stables,
inefficace sur du texte ordinaire.
```
