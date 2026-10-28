# Jour 102 — Masques de bits : lecture d'un registre d'état simulé

**Langue :** Java

## Le problème
Un capteur I2C (accéléromètre, chargeur de batterie, module GPS) renvoie souvent un seul octet ou mot de « statut » dans lequel chaque bit ou groupe de bits a une signification : donnée prête, erreur, mode de charge, niveau de batterie… Il faut savoir décoder ce registre.

## La solution
On décrit chaque champ du registre (nom, position, largeur) dans une table, puis on extrait chaque champ avec (registre >>> position) & masque. On simule le registre d'état 16 bits d'un chargeur de batterie et on décode plusieurs valeurs lues, avec un affichage bit par bit.

## Ce qu'on apprend
Les masques multi-bits, le décalage non signé >>> en Java, les tables de description de registres (comme dans une fiche technique) et l'affichage binaire formaté.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Registre = 0x4A0B = 0100 1010 0000 1011
   bits  0-0  DONNEE_PRETE     = 1   → oui
   bits  1-1  ENTREE_PRESENTE  = 1   → alimentation branchée
   bits  2-3  MODE_CHARGE      = 2   → charge rapide
   bits  4-4  SURCHAUFFE       = 0   → non
   bits  5-7  ERREUR           = 0   → aucune
   bits  8-14 NIVEAU_BATTERIE  = 74  → 74 %
   bits 15-15 ALERTE           = 0   → non

Registre = 0x640F = 0110 0100 0000 1111
   bits  0-0  DONNEE_PRETE     = 1   → oui
   bits  1-1  ENTREE_PRESENTE  = 1   → alimentation branchée
   bits  2-3  MODE_CHARGE      = 3   → terminée
   bits  4-4  SURCHAUFFE       = 0   → non
   bits  5-7  ERREUR           = 0   → aucune
```
