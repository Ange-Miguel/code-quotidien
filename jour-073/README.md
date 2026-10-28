# Jour 073 — Matrice : rotation d'une image en niveaux de gris

**Langue :** C

## Le problème
Une caméra embarquée (ESP32-CAM, capteur d'image) est souvent montée de travers : l'image arrive tournée de 90°. Il faut la redresser avec peu de mémoire, comme sur un microcontrôleur.

## La solution
Une image en niveaux de gris est une matrice d'octets (0 = noir, 255 = blanc). Rotation de 90° dans le sens horaire : le pixel (l, c) va en (c, H − 1 − l). Pour une image carrée, on la tourne en place, sans deuxième tableau, en échangeant les pixels quatre par quatre, couche par couche. On affiche le résultat en caractères ASCII et on vérifie que quatre rotations ramènent l'image d'origine.

## Ce qu'on apprend
Les tableaux 2D, les transformations d'indices, l'algorithme en place (économie de mémoire) et l'affichage ASCII d'une image.

## Lancer
```bash
gcc -Wall -Wextra -o rotation_image rotation_image.c -lm
./rotation_image
```

## Exemple de sortie
```
Image d'origine (flèche vers la droite) :
   ==                      
   ==                      
   ==              @@      
   ==              @@##    
   ==              @@##**  
   ==@@@@@@@@@@@@@@@@##**==
   ==@@@@@@@@@@@@@@@@##**==
   ==              @@##**  
   ==              @@##    
   ==              @@      
   ==                      
   ==                      

Après rotation de 90° (en place) :
```
