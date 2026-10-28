# Jour 047 — Détecteur de doublons dans une liste de contacts

**Langue :** Python

## Le problème
Après plusieurs changements de téléphone, le répertoire contient « Maman », « maman », « Maman Calavi » avec le même numéro écrit de trois façons. Il faut repérer et fusionner les doublons.

## La solution
On normalise chaque numéro (chiffres seulement, indicatif +229 et préfixe 01 harmonisés) et chaque nom (minuscules, sans accents ni espaces superflus). Deux contacts sont des doublons s'ils ont le même numéro normalisé, ou des noms très proches (ratio difflib ≥ 0,85). On propose une fiche fusionnée qui garde le nom le plus complet et tous les numéros.

## Ce qu'on apprend
La normalisation de données, unicodedata, difflib et le regroupement avec une structure union-find.

## Lancer
```bash
python3 doublons_contacts.py
```

## Exemple de sortie
```
8 contacts → 5 contacts uniques, 3 groupe(s) de doublons

Doublons :
   - 'Maman'            +229 01 97 12 34 56
   - 'maman '           97123456
   → fusion : Maman : +229 0197123456

Doublons :
   - 'Koffi Mensah'     01 61 22 33 44
   - 'Kofi Mensah'      0161223355
   → fusion : Koffi Mensah : +229 0161223344, +229 0161223355

Doublons :
   - 'Père Gérard'      00229 01 95 00 11 22
   - 'Pere Gerard'      +229 0195001122
```
