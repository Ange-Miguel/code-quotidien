# Jour 090 — Générateur de factures en texte formaté

**Langue :** Java

## Le problème
Un freelance (graphisme, motion design, développement) ou une petite boutique doit envoyer des factures propres, numérotées, avec les totaux justes, sans payer un logiciel.

## La solution
Une facture est composée de lignes (désignation, quantité, prix unitaire, remise). Les montants sont en entiers (FCFA) pour éviter les erreurs d'arrondi des nombres à virgule. On calcule sous-total, remise, TVA optionnelle et total, on écrit le montant en lettres simplifié et on produit un document texte aligné, prêt à imprimer ou à convertir en PDF.

## Ce qu'on apprend
Les records, StringBuilder et String.format pour la mise en page, les calculs monétaires en entiers et la numérotation de documents.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
AKPANON Ange-Miguel — Création graphique & développement
Abomey-Calavi, Bénin

FACTURE N° 2026-0042                                      Date : 2026-10-28
Client : Association exemple (Cotonou)

------------------------------------------------------------------------------
Désignation                              Qté         P.U.   Rem.       Montant
------------------------------------------------------------------------------
Animation motion design 30 s               1      150 000     0%       150 000
Déclinaisons formats réseaux sociaux       3       15 000    10%        40 500
Affiche A3 événement                       2       25 000     0%        50 000
Retouches supplémentaires (heure)          4        5 000    50%        10 000
------------------------------------------------------------------------------
                                               Total hors taxe :       250 500
```
