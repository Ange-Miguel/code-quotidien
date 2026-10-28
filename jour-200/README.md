# Jour 200 — Bilan des 200 jours : récapitulatif et statistiques du dépôt

**Langue :** Python

## Le problème
Après 200 jours de programmes quotidiens, il est temps de faire le bilan : combien de lignes écrites dans chaque langage, quels thèmes, quelle progression, quels jours ont produit les plus gros programmes ? Un bon bilan se calcule à partir des données réelles du dépôt, pas de mémoire.

## La solution
Le programme parcourt le dépôt (les dossiers jour-001 à jour-200 et le fichier PLAN.md), compte pour chaque jour les lignes de code utiles (sans lignes vides ni commentaires), identifie le langage, relie chaque jour à sa phase et produit : un tableau par langage, un histogramme par phase, le top 5 des plus longs programmes, la progression de la taille moyenne et une carte de 200 cases façon « graphe de contributions » GitHub.

## Ce qu'on apprend
Pathlib pour parcourir des dossiers, les expressions régulières pour lire un fichier Markdown, le comptage de lignes de code par langage et la présentation d'un rapport final.

## Lancer
```bash
python3 bilan.py
```

## Exemple de sortie
Exécuté dans le dépôt complet, après les 200 jours :
```
=== Bilan du dépôt code-quotidien : 200 jour(s) publiés, 12 002 lignes de code ===

Langage  Programmes   Lignes  Moyenne
C                67     4304       64
Java             66     3871       59
Python           67     3827       57

Lignes par phase :
   Bases utiles               2567 █████████████████
   Structures et fichiers     3283 █████████████████████
   Embarqué et IoT            2971 ███████████████████
   Projets réels              3181 █████████████████████

Les 5 programmes les plus longs :
   jour 193 (C     )  137 lignes — Mini-langage de calcul : analyseur et évaluateur
   jour 100 (C     )  104 lignes — Bilan de la phase 2 : mini-application de gestion de boutique
   jour 132 (Java  )  102 lignes — Décodage d'une commande JSON reçue d'un serveur
   jour 192 (Java  )   96 lignes — Mini-interpréteur de commandes (shell)
   jour 190 (C     )   94 lignes — Conversion Markdown → HTML simple

Carte des 200 jours (█ publié, · à venir) :
   ████████████████████████████████████████
   ████████████████████████████████████████
   ████████████████████████████████████████
   ████████████████████████████████████████
   ████████████████████████████████████████

Bravo pour ces 200 jours de régularité !
```
