# Jour 173 — Tirage au sort équitable (tombola) avec graine

**Langue :** Python

## Le problème
Lors d'une tombola, d'un jeu concours sur les réseaux sociaux ou d'une attribution de places, les participants soupçonnent souvent que le tirage est truqué. Comment faire un tirage au sort que tout le monde peut vérifier après coup ?

## La solution
Un tirage « vérifiable ». Avant le tirage, on publie la liste exacte des tickets et son empreinte SHA-256 (impossible de la modifier ensuite sans que cela se voie). La graine du hasard est un élément que personne ne contrôle et connu de tous après coup (par exemple les numéros d'un tirage public annoncé à l'avance). Graine + liste → tirage déterministe : n'importe qui peut relancer le programme et retrouver exactement les mêmes gagnants. Un participant avec plus de tickets a plus de chances, proportionnellement.

## Ce qu'on apprend
Hashlib, random.Random(graine) et le déterminisme, le tirage sans remise, la pondération et la notion de vérifiabilité.

## Lancer
```bash
python3 tombola.py
```

## Exemple de sortie
```
19 tickets vendus, empreinte publiée AVANT le tirage :
   9e690d091dc9f7458bf3968ceb0de2b12594ae706522a849e7a71bb9bf1989e9
Graine publique : « Loto du 28/10/2026 : 07-19-33-41-58 »

   1er lot : téléphone                → ticket Serge#2
   2e lot : bon d'achat 20 000 FCFA   → ticket Inès#1
   3e lot : casque audio              → ticket Mireille#2

Vérification : relancer le tirage avec la même graine donne les mêmes gagnants ✔
Si l'organisateur ajoute un ticket après coup, l'empreinte change : 4b579dd69d86de6d… ✘

Équité sur 3 000 tirages d'un seul lot (chance attendue = part des tickets) :
   Awa       3 ticket(s) : attendu 15.8%, observé 15.7%
   Kofi      1 ticket(s) : attendu  5.3%, observé  5.2%
   Inès      5 ticket(s) : attendu 26.3%, observé 26.2%
```
