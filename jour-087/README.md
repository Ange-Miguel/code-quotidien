# Jour 087 — Renommage en masse de fichiers photos par date

**Langue :** Java

## Le problème
Des centaines de photos nommées « IMG_4521.jpg », « DSC0012.JPG » ou « photo (3).jpeg » sont impossibles à classer. On veut des noms comme « 2026-10-14_09-31-05.jpg », triés naturellement par date.

## La solution
Pour chaque image, on lit sa date de dernière modification (les vraies photos ont aussi une date EXIF, non traitée ici), on construit le nouveau nom, on ajoute un suffixe _2, _3… en cas de collision, et on affiche le plan de renommage. Par défaut, c'est une simulation : les fichiers ne sont renommés qu'avec l'option --appliquer.

## Ce qu'on apprend
Java.nio.file (Files, Path, attributs), le formatage de dates, la gestion des collisions de noms et le mode « simulation » avant toute action destructive.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Simulation :
  DSC0012.JPG            → 2026-10-14_11-36-05.jpg
  IMG_4521.jpg           → 2026-10-14_09-31-05.jpg
  IMG_4522.jpg           → 2026-10-14_09-31-05_2.jpg
  photo (3).jpeg         → 2026-10-15_18-51-05.jpg
  video.mp4              → 2026-10-16_19-51-05.mp4
5 fichier(s) à renommer (simulation : ajoutez --appliquer)

Application :
  DSC0012.JPG            → 2026-10-14_11-36-05.jpg
  IMG_4521.jpg           → 2026-10-14_09-31-05.jpg
  IMG_4522.jpg           → 2026-10-14_09-31-05_2.jpg
  photo (3).jpeg         → 2026-10-15_18-51-05.jpg
  video.mp4              → 2026-10-16_19-51-05.mp4
5 fichier(s) renommé(s)
```
