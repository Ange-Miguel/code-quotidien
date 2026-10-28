# Jour 178 — Moteur de recherche plein texte sur des documents (index inversé)

**Langue :** C

## Le problème
Chercher un mot dans des centaines de cours, de fiches produits ou d'articles en relisant chaque document à chaque recherche est trop lent. Les moteurs de recherche utilisent un index inversé.

## La solution
On construit une fois l'index : pour chaque mot (en minuscules, sans les mots vides comme « le », « de »), la liste des documents où il apparaît, avec le nombre d'occurrences. Une recherche à plusieurs mots intersecte ces listes (tous les mots doivent être présents), puis classe les résultats par score TF-IDF : un mot fréquent dans le document mais rare dans la collection compte beaucoup. Les documents sont de petites fiches d'exemple.

## Ce qu'on apprend
L'index inversé, la tokenisation, les mots vides, le score TF-IDF avec log, et l'intersection de listes.

## Lancer
```bash
gcc -Wall -Wextra -o index_inverse index_inverse.c -lm
./index_inverse
```

## Exemple de sortie
```
Index construit : 40 termes distincts dans 6 documents

Recherche « capteur » :
   0.41  Cours : capteurs de température
   0.41  Cours : protocole MQTT
   0.41  Article : énergie solaire
   0.41  Fiche : Arduino Uno

Recherche « température capteur » :
   2.27  Cours : capteurs de température
   1.50  Fiche : Arduino Uno

Recherche « microcontrôleur » :
   0.41  Cours : capteurs de température
   0.41  Fiche : ESP32
```
