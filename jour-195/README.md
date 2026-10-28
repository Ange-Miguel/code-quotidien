# Jour 195 — Jeu de la vie de Conway

**Langue :** Java

## Le problème
Comment des règles très simples, appliquées localement, peuvent- elles produire des comportements complexes : structures stables, oscillateurs, « vaisseaux » qui se déplacent ? Le jeu de la vie (John Conway, 1970) est l'exemple le plus célèbre d'automate cellulaire, utilisé pour illustrer l'émergence en biologie, en physique et en informatique.

## La solution
Une grille de cellules vivantes ou mortes. À chaque génération : une cellule vivante avec 2 ou 3 voisines survit ; une cellule morte avec exactement 3 voisines naît ; toutes les autres meurent ou restent mortes. On calcule la génération suivante dans une seconde grille (les deux tableaux sont échangés), sur un tore (les bords se rejoignent). On détecte un état déjà vu (cycle) grâce au hachage des grilles.

## Ce qu'on apprend
Les automates cellulaires, le double tampon, l'arithmétique modulaire pour un monde torique et la détection de cycle par hachage.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Génération 0 (population 17)
   ······························
   ··█···························
   ···█··········███·············
   ·███··························
   ······························
   ······························
   ······················██······
   ·····██···············██······
   ····██························
   ·····█························
   ······························
   ······························

Génération 4 (population 22)
```
