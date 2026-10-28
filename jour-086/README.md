# Jour 086 — Recherche de fichiers en double par empreinte

**Langue :** Python

## Le problème
Le téléphone ou l'ordinateur est plein, souvent à cause des mêmes photos et vidéos reçues plusieurs fois sur WhatsApp, sous des noms différents. Comparer les fichiers deux à deux serait trop long.

## La solution
En trois étapes de plus en plus coûteuses : (1) regrouper par taille (deux fichiers de tailles différentes ne peuvent pas être identiques) ; (2) dans chaque groupe, comparer l'empreinte des 4 premiers Ko ; (3) pour les candidats restants, calculer l'empreinte SHA-256 complète. On affiche les doublons et la place récupérable. Rien n'est supprimé.

## Ce qu'on apprend
Os.walk, hashlib, la lecture par blocs de gros fichiers, et le filtrage progressif pour éviter des calculs inutiles.

## Lancer
```bash
python3 doublons_fichiers.py
```

## Exemple de sortie
```
Dossier de démonstration avec 6 fichiers :

3 copies identiques (50 000 octets) :
   Photos/anniversaire.jpg
   WhatsApp/IMG-20261001(1).jpg
   WhatsApp/IMG-20261001.jpg
2 copies identiques (30 000 octets) :
   Docs/cours_C.pdf
   Docs/cours_C_copie.pdf

2 groupe(s) de doublons  130 000 octets récupérables (rien n'a été supprimé)
```
