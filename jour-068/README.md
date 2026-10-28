# Jour 068 — Suivi des présences d'une classe

**Langue :** Python

## Le problème
Un délégué ou un enseignant note les présences sur papier ; à la fin du semestre, impossible de savoir rapidement qui a dépassé le nombre d'absences autorisé ou quels jours la classe est la plus vide.

## La solution
Les présences sont stockées dans une matrice étudiants × séances (P = présent, A = absent, R = retard, J = absence justifiée). On calcule le taux de présence par étudiant et par séance, on alerte au-delà de 20 % d'absences non justifiées, et on trouve le jour de la semaine le plus touché.

## Ce qu'on apprend
Les dictionnaires de listes, les compréhensions, Counter, le regroupement par jour de la semaine et la production d'alertes.

## Lancer
```bash
python3 presences.py
```

## Exemple de sortie
```
24 séances (lundi, mercredi, vendredi) du 2026-09-07 au 2026-10-30

Étudiant            Prés.  Abs. Just.  Ret.  Taux
Adjovi Mireille        16     7     1     1    71%  ⚠ dépasse 20 % d'absences
Agossa Inès            16     8     0     0    67%  ⚠ dépasse 20 % d'absences
Akpanon Ange           18     5     1     0    79%  ⚠ dépasse 20 % d'absences
Dossou Carine          13    10     1     1    58%  ⚠ dépasse 20 % d'absences
Houngbo Serge          22     2     0     1    92%
Kpade Yves             16     6     2     2    75%  ⚠ dépasse 20 % d'absences
Zinsou Paul            18     6     0     3    75%  ⚠ dépasse 20 % d'absences

Absences moyennes par séance selon le jour :
   lundi      1.4 ▇▇▇▇▇▇
   mercredi   1.6 ▇▇▇▇▇▇
   vendredi   2.5 ▇▇▇▇▇▇▇▇▇▇
```
