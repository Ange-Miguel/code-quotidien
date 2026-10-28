# Jour 088 — Organisation automatique des téléchargements par extension

**Langue :** C

## Le problème
Le dossier « Téléchargements » finit par contenir des centaines de PDF, images, vidéos, archives et installateurs mélangés.

## La solution
On parcourt le dossier avec opendir/readdir, on détermine la catégorie de chaque fichier d'après son extension (table de correspondance), on crée les sous-dossiers nécessaires (Documents, Images, Vidéos, Archives, Programmes, Autres) et on déplace les fichiers avec rename. Si un fichier du même nom existe déjà, on ajoute un suffixe. Par défaut c'est une simulation ; --appliquer effectue les déplacements. La démo travaille dans un dossier temporaire.

## Ce qu'on apprend
L'API POSIX des répertoires (opendir, readdir, stat, mkdir, rename), la gestion des chemins en C et le mode simulation.

## Lancer
```bash
gcc -Wall -Wextra -o ranger_telechargements ranger_telechargements.c -lm
./ranger_telechargements
```

## Exemple de sortie
```
Simulation :
  projet.zip                 → Archives/
  chanson.mp3                → Audio/
  notes                      → Autres/
  WhatsApp.apk               → Programmes/
  Cours_Electronique.pdf     → Documents/
  tuto_arduino.mp4           → Vidéos/
  main.c                     → Code/
  logo.PNG                   → Images/
  facture_octobre.xlsx       → Documents/
  IMG-20261012.jpg           → Images/
10 fichier(s) à ranger (simulation, ajoutez --appliquer)

Application :
  projet.zip                 → Archives/
```
