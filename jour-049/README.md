# Jour 049 — Compteur de fréquence des lettres d'un texte

**Langue :** C

## Le problème
Quelles lettres reviennent le plus dans un texte français ? C'est la base de l'analyse de fréquences qui permet de casser les chiffrements simples (voir le jour 014) et de comprendre la compression de données.

## La solution
On lit le texte depuis un fichier (ou un texte d'exemple), on compte les 26 lettres dans un tableau indexé par c − 'a' (les lettres accentuées UTF-8 courantes sont ramenées à leur base), puis on trie par fréquence et on affiche un histogramme.

## Ce qu'on apprend
La lecture de fichier caractère par caractère avec fgetc, les tableaux de compteurs indexés, le tri d'indices et l'histogramme texte.

## Lancer
```bash
gcc -Wall -Wextra -o frequences frequences.c -lm
./frequences
```

## Exemple de sortie
```
256 lettres analysées
  e 16.80 % #################################
  a  8.20 % ################
  l  8.20 % ################
  i  7.81 % ###############
  n  7.03 % ##############
  s  7.03 % ##############
  t  6.64 % #############
  o  5.86 % ###########
  u  5.86 % ###########
  c  4.30 % ########
  r  4.30 % ########
  p  3.52 % #######
  d  2.73 % #####
  m  2.73 % #####
```
