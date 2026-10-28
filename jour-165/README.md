# Jour 165 — Gestion de devis client pour prestations graphiques

**Langue :** Java

## Le problème
Un graphiste ou motion designer freelance doit chiffrer rapidement des demandes variées (logo, affiche, vidéo animée…), sans oublier les révisions, l'urgence ou les droits d'utilisation, et suivre ce que devient chaque devis (envoyé, accepté, refusé, expiré).

## La solution
Un catalogue de prestations avec prix de base et options ; un devis est construit à partir d'un brief (prestations, quantités, urgence, révisions supplémentaires) et reçoit un numéro, une date de validité (30 jours) et un acompte de 50 %. Un cycle de vie (enum avec transitions autorisées) empêche par exemple d'accepter un devis expiré. On calcule le taux de conversion des devis. Tarifs donnés à titre d'exemple.

## Ce qu'on apprend
La modélisation d'un document commercial, une machine à états avec transitions contrôlées et les statistiques de suivi commercial.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
DEV-2026-001 — Association exemple — 2026-10-01 (valable jusqu'au 2026-10-31)
   Visuel réseaux sociaux             3 ×    8 000
   Animation motion design 30 s       1 ×  150 000
   Révisions supplémentaires          1 ×   10 000
   TOTAL 184 000 FCFA — acompte à la commande 92 000 FCFA

Refusé par le système : DEV-2026-002 : REFUSE → ACCEPTE interdit

DEV-2026-001  Association exemple    184 000 FCFA  ACCEPTE
DEV-2026-002  Boutique de pagnes     135 000 FCFA  REFUSE
DEV-2026-003  Restaurant maquis      117 000 FCFA  ACCEPTE
DEV-2026-004  Startup IoT             95 000 FCFA  EXPIRE

Taux de conversion : 2/4 devis, 301 000 sur 531 000 FCFA (57 %)
```
