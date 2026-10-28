# Jour 071 — Détection de cycle dans des dépendances de tâches

**Langue :** Python

## Le problème
Dans un projet (construction, logiciel, organisation d'événement), certaines tâches dépendent d'autres. Si A attend B, B attend C et C attend A, le projet est bloqué pour toujours : c'est un cycle, souvent difficile à voir.

## La solution
On représente les dépendances par un graphe orienté et on fait un parcours en profondeur (DFS) avec trois couleurs : blanc (non visité), gris (en cours d'exploration), noir (terminé). Rencontrer un sommet gris signifie qu'on est revenu sur ses pas : il y a un cycle, qu'on reconstruit et affiche.

## Ce qu'on apprend
Les graphes orientés, le DFS itératif et récursif, la méthode des trois couleurs et la reconstruction d'un cycle à partir de la pile.

## Lancer
```bash
python3 cycles_dependances.py
```

## Exemple de sortie
```
Projet « Boutique en ligne » :
   Mise en ligne            attend : Paiement mobile, Catalogue
   Paiement mobile          attend : Compte marchand
   Catalogue                attend : Photos produits
   Photos produits          attend : —
   Compte marchand          attend : Statuts société
   Statuts société          attend : —
   ✔ Aucun cycle : le projet peut avancer

Projet « Dossier administratif » :
   Ouvrir compte bancaire   attend : Obtenir RCCM
   Obtenir RCCM             attend : Justificatif de domicile
   Justificatif de domicile attend : Facture à son nom
   Facture à son nom        attend : Ouvrir compte bancaire
   ✘ BLOCAGE : Ouvrir compte bancaire → attend → Obtenir RCCM → attend → Justificatif de domicile → attend → Facture à son nom → attend → Ouvrir compte bancaire
```
