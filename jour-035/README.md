# Jour 035 — Nombre en lettres en français (pour chèques et factures)

**Langue :** Python

## Le problème
Sur un chèque, une facture ou un contrat, le montant doit être écrit en toutes lettres (« deux cent cinquante mille francs CFA »). Les règles du français sont piégeuses : soixante-dix, quatre-vingts, cent/cents, et-un…

## La solution
On découpe le nombre en tranches de trois chiffres (unités, milliers, millions, milliards). Chaque tranche est écrite par une fonction dédiée qui applique les règles d'accord : « quatre-vingts » et « deux cents » prennent un s seulement en fin de nombre, « mille » est invariable, « million » et « milliard » prennent un s au pluriel. Orthographe traditionnelle.

## Ce qu'on apprend
La décomposition récursive d'un problème, les cas particuliers d'une langue et les tests par exemples connus.

## Lancer
```bash
python3 nombre_en_lettres.py
```

## Exemple de sortie
```
71 → soixante et onze francs CFA
            180 → cent quatre-vingts francs CFA
          1 960 → mille neuf cent soixante francs CFA
        250 000 → deux cent cinquante mille francs CFA
      3 750 500 → trois millions sept cent cinquante mille cinq cents francs CFA
  1 200 000 000 → un milliard deux cents millions francs CFA
```
