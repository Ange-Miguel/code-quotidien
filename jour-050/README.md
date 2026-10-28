# Jour 050 — Quiz de culture générale à choix multiples avec score

**Langue :** Python

## Le problème
Réviser en s'amusant, ou animer une soirée, avec un quiz qui mélange questions et réponses, compte les points et affiche un bilan.

## La solution
Les questions sont des données (liste de dictionnaires) séparées du moteur du quiz. Les choix sont mélangés à chaque partie, la bonne réponse est suivie par son texte et non sa position. Un mode démo fait jouer un joueur simulé ; le mode --jouer interroge vraiment l'utilisateur.

## Ce qu'on apprend
La séparation données / logique, random.shuffle avec une graine, le calcul de score par catégorie et la validation d'une saisie.

## Lancer
```bash
python3 quiz.py
```

## Exemple de sortie
```
1. Quelle est la capitale officielle du Bénin ?
   A) Abomey
   B) Cotonou
   C) Porto-Novo
   D) Parakou
   → D : raté, c était Porto-Novo

2. Quel est le plus long fleuve d'Afrique ?
   A) Le Zambèze
   B) Le Nil
   C) Le Niger
   D) Le Congo
   → B : bonne réponse !

3. Qui a créé le langage C ?
```
