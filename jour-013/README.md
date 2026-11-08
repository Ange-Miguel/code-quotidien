# Jour 013 — Détecteur de palindromes (mots et phrases)

**Langue :** C

## Le problème
Vérifier si un mot ou une phrase se lit de la même façon dans les deux sens (« kayak », « Ésope reste ici et se repose »), en ignorant espaces, ponctuation, majuscules et accents.

## La solution
On normalise d'abord le texte : on garde uniquement les lettres et chiffres, en minuscules, et on remplace les lettres accentuées UTF-8 les plus courantes par leur lettre de base. Puis deux indices partent des deux bouts et se rapprochent en comparant les caractères.

## Ce qu'on apprend
Le parcours avec deux pointeurs, l'encodage UTF-8 sur plusieurs octets et la normalisation de texte en C.

## Lancer
```bash
gcc -Wall -Wextra -o palindromes palindromes.c -lm
./palindromes
```

## Exemple de sortie
```
kayak                              → palindrome
Ésope reste ici et se repose      → palindrome
Engage le jeu que je le gagne      → palindrome
Calavi                             → pas un palindrome
Été                              → palindrome
12321                              → palindrome
!!!                                → pas un palindrome
```
