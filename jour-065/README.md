# Jour 065 — Gestion de bibliothèque : emprunts et retours de livres

**Langue :** Python

## Le problème
Une bibliothèque de quartier, d'association ou de département prête des livres sur papier ; on perd la trace de qui a quoi et des retards.

## La solution
Trois classes (Livre, Membre, Bibliotheque). Les règles métier sont dans les méthodes : 3 emprunts maximum par membre, 14 jours de prêt, pas d'emprunt si le membre a un retard, file de réservation quand le livre est déjà sorti, pénalité de 50 FCFA par jour de retard. Des exceptions dédiées expliquent chaque refus.

## Ce qu'on apprend
La programmation orientée objet, les exceptions personnalisées, les files de réservation (deque) et l'encapsulation des règles de gestion.

## Lancer
```bash
python3 bibliotheque.py
```

## Exemple de sortie
```
✔ Awa emprunte L1
✘ Kofi emprunte L1 : déjà emprunté → réservé (1 en attente)
✔ Awa emprunte L2
✔ Awa emprunte L3
✘ Awa emprunte L4 : maximum 3 livres
✔ Awa rend L1 le 20/10 (retard 5 j)
✘ Awa emprunte L4 le 20/10 : retard en cours, rendez d'abord vos livres
✔ Kofi emprunte L1 (sa réservation)

Pénalités d'Awa : 250 FCFA
   L1 Le Langage C                 chez Kofi jusqu'au 2026-11-03
   L2 Électronique pour les nuls   chez Awa jusqu'au 2026-10-15
   L3 Une si longue lettre         chez Awa jusqu'au 2026-10-15
   L4 L'Enfant noir                disponible
```
