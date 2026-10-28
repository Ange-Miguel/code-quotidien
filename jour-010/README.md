# Jour 010 — Générateur de mot de passe aléatoire robuste

**Langue :** C

## Le problème
On réutilise souvent le même mot de passe faible partout. Il faut en créer de longs et variés pour chaque compte (e-mail, banque, GitHub).

## La solution
On lit des octets aléatoires de qualité dans /dev/urandom et on les transforme en caractères pris dans quatre familles (minuscules, majuscules, chiffres, symboles). On garantit au moins un caractère de chaque famille, puis on mélange avec l'algorithme de Fisher-Yates. Le rejet des valeurs trop grandes évite un biais statistique.

## Ce qu'on apprend
La lecture de fichiers spéciaux, le biais du modulo, le mélange de Fisher-Yates et la manipulation de chaînes en C.

## Lancer
```bash
gcc -Wall -Wextra -o mot_de_passe mot_de_passe.c -lm
./mot_de_passe
```

## Exemple de sortie
```
5 mot(s) de passe de 16 caractères :
  eyNQoeg6*44+*BoL
  G7bj%@Pmq=7$-Np3
  -w+N-4#cSXH9VkdF
  7V7BBA@m6?Nv9+6n
  =4$Q2T7qekRGuVp2
```
