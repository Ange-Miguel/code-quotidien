# Jour 014 — Chiffrement de César : chiffrer et déchiffrer un message

**Langue :** Python

## Le problème
On veut cacher le sens d'un court message (jeu, devinette) et, surtout, comprendre pourquoi les chiffrements trop simples sont cassables.

## La solution
Chaque lettre est décalée de k positions dans l'alphabet. Pour déchiffrer sans la clé, on teste les 26 décalages et on garde celui dont la fréquence des lettres ressemble le plus au français (attaque par analyse de fréquences, score du khi-deux).

## Ce qu'on apprend
Les codes de caractères (ord/chr), l'arithmétique modulaire et une première attaque cryptographique.

## Lancer
```bash
python3 cesar.py
```

## Exemple de sortie
```
Message  : Rendez-vous demain matin devant la bibliotheque de l'universite
Chiffré  : Yluklg-cvbz klthpu thapu klchua sh ipispvaolxbl kl s'bupclyzpal
Déchiffré: Rendez-vous demain matin devant la bibliotheque de l'universite

Attaque sans la clé → clé trouvée : 7
Résultat : Rendez-vous demain matin devant la bibliotheque de l'universite
```
