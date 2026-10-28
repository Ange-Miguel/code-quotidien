# Jour 171 — Gestion d'association : membres et cotisations

**Langue :** Java

## Le problème
Une association d'étudiants, un club ou une amicale suit ses membres et leurs cotisations sur un cahier : qui est à jour, qui doit combien, combien a-t-on encaissé, qui peut voter à l'assemblée générale ?

## La solution
Chaque membre a une catégorie (étudiant, actif, bienfaiteur) qui fixe sa cotisation mensuelle, une date d'adhésion et la liste de ses paiements. On calcule ce qui est dû depuis l'adhésion, les arriérés, le statut (à jour, en retard, suspendu après 3 mois d'impayés), la liste électorale (membres à jour depuis au moins 3 mois) et un état de caisse mensuel. Membres et montants fictifs.

## Ce qu'on apprend
YearMonth et les calculs par mois, les records et les streams, les règles d'un règlement intérieur traduites en code.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Situation au 2026-10 :
Membre       Catégorie           Dû      Payé  Arriérés  Statut
Ange         ETUDIANT        10 000    10 000         0  à jour
Awa          ETUDIANT         8 000     6 000     2 000  en retard (2 mois)
M. Kpade     ACTIF           36 000    21 000    15 000  SUSPENDU (5 mois)
Mme Zinsou   BIENFAITEUR    100 000   100 000         0  à jour
Kofi         ETUDIANT         2 000     2 000         0  à jour

Liste électorale (à jour et membre depuis ≥ 3 mois) : [Ange, Mme Zinsou]

Encaissements par mois :
   2025-11    3 000 FCFA ▇
   2025-12    3 000 FCFA ▇
   2026-01  104 000 FCFA ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
   2026-02    4 000 FCFA ▇
```
