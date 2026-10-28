# Jour 056 — Export d'un rapport de ventes en CSV

**Langue :** Python

## Le problème
Le comptable, l'associé ou l'investisseur veut un fichier qu'il peut ouvrir dans Excel ou Google Sheets : ventes par produit et par semaine, avec totaux et évolution.

## La solution
À partir d'une liste de ventes, on construit un tableau croisé (produits en lignes, semaines en colonnes), on ajoute les totaux et la variation entre la première et la dernière semaine, puis on écrit le CSV avec le séparateur « ; » et l'encodage UTF-8 avec BOM, pour qu'Excel en français l'ouvre correctement avec les accents.

## Ce qu'on apprend
Le module csv (DictWriter), isocalendar pour les semaines, le tableau croisé et les détails pratiques d'encodage des fichiers.

## Lancer
```bash
python3 rapport_csv.py
```

## Exemple de sortie
```
91 ventes → rapport CSV (aperçu) :

Produit;S37;S38;S39;S40;Total;Évolution
Chargeur;42500;27500;35000;47500;152500;+12%
Lampe solaire;60000;72000;90000;78000;300000;+30%
Montre;228000;180000;156000;228000;792000;+0%
Écouteurs;54000;58500;72000;63000;247500;+17%
TOTAL;384500;338000;353000;416500;1492000;
```
