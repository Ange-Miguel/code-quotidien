# Jour 051 — Carnet d'adresses enregistré dans un fichier

**Langue :** Java

## Le problème
Garder les coordonnées de ses clients, fournisseurs et camarades dans un fichier qu'on peut sauvegarder, lire et rechercher, au lieu de les perdre avec un téléphone.

## La solution
Chaque contact est un record (nom, téléphone, e-mail, ville). Le carnet est enregistré au format CSV avec échappement des guillemets et des virgules, relu au démarrage, trié par nom et interrogeable par recherche partielle insensible à la casse.

## Ce qu'on apprend
La persistance en CSV, l'échappement de caractères, les TreeMap pour garder un ordre trié et la gestion des fichiers temporaires.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Fichier CSV écrit :
  nom,telephone,email,ville
  Adjovi Mireille,+229 01 97 33 44 55,mireille@exemple.bj,Abomey-Calavi
  "Boutique ""Le Bon Prix""",+229 01 52 00 00 00,contact@bonprix.bj,Godomey
  "Zinsou, Paul",+229 01 66 00 11 22,paul@exemple.bj,Cotonou

Carnet relu (trié) :
  Adjovi Mireille          +229 01 97 33 44 55  mireille@exemple.bj          Abomey-Calavi
  Boutique "Le Bon Prix"   +229 01 52 00 00 00  contact@bonprix.bj           Godomey
  Zinsou, Paul             +229 01 66 00 11 22  paul@exemple.bj              Cotonou

Recherche « calavi » :
  Adjovi Mireille          +229 01 97 33 44 55  mireille@exemple.bj          Abomey-Calavi
```
