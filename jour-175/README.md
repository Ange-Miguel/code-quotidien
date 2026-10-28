# Jour 175 — Raccourcisseur d'URL en mémoire

**Langue :** C

## Le problème
Une longue adresse de produit ou de formulaire est difficile à partager par SMS ou à imprimer sur une affiche. Les raccourcisseurs (comme bit.ly) la remplacent par un code court, et comptent les clics.

## La solution
Chaque URL reçoit un identifiant numérique croissant, converti en base 62 (0-9, a-z, A-Z) : 7 caractères suffisent pour plus de 3 500 milliards d'adresses. Une table de hachage (adressage ouvert, sondage linéaire) retrouve l'URL longue à partir du code ; une seconde évite de créer deux codes pour la même URL. On compte les visites et on refuse les URL qui ne commencent pas par http:// ou https://.

## Ce qu'on apprend
La conversion en base 62, les tables de hachage à adressage ouvert, la fonction de hachage djb2 et la validation d'entrées.

## Lancer
```bash
gcc -Wall -Wextra -o raccourcisseur raccourcisseur.c -lm
./raccourcisseur
```

## Exemple de sortie
```
https://boutique-exemple.bj/produits/ecouteurs-bluetooth-san… → ww8
https://forms.exemple.com/inscription-tournoi-football-campu… → ww9
https://boutique-exemple.bj/produits/ecouteurs-bluetooth-san… → ww8
ftp://fichiers.exemple.bj/cours.pdf                         … → refusée (protocole non autorisé)

Visites :
   crt.bj/ww8    → redirection vers l'adresse longue
   crt.bj/ww9    → redirection vers l'adresse longue
   crt.bj/ww8    → redirection vers l'adresse longue
   crt.bj/ww8    → redirection vers l'adresse longue
   crt.bj/zzz9   → 404 : lien inconnu
   crt.bj/ww9    → redirection vers l'adresse longue

Statistiques : crt.bj/ww8 = 3 clics, crt.bj/ww9 = 2 clics
Capacité avec 7 caractères en base 62 : 62^7 = 3 521 614 606 208 liens
```
