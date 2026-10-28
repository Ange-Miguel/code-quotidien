# Jour 158 — Prise de rendez-vous médical sans chevauchement

**Langue :** Python

## Le problème
Un cabinet médical gère ses rendez-vous dans un cahier ; deux patients se retrouvent sur le même créneau, ou un médecin est réservé pendant sa pause. Une application doit proposer uniquement des créneaux réellement libres, selon la durée de chaque type de consultation.

## La solution
Pour chaque médecin, les rendez-vous sont gardés triés par heure de début (module bisect). Deux intervalles [a, b[ et [c, d[ se chevauchent si a < d et c < b. Pour proposer des créneaux, on parcourt les plages d'ouverture et on « saute » les rendez-vous existants, avec une marge de 5 minutes entre deux patients. On gère aussi l'annulation, qui libère le créneau.

## Ce qu'on apprend
La représentation d'intervalles de temps, le test de chevauchement, bisect.insort pour garder une liste triée et la recherche de créneaux libres.

## Lancer
```bash
python3 rendez_vous.py
```

## Exemple de sortie
```
Awa     08:00 consultation  → réservé
Kofi    08:10 suivi         → refusé (chevauchement ou hors horaires)
Kofi    08:25 suivi         → réservé
Carine  11:45 échographie   → refusé (chevauchement ou hors horaires)
Carine  09:00 échographie   → réservé
Yves    12:30 vaccination   → refusé (chevauchement ou hors horaires)
Inès    09:20 consultation  → refusé (chevauchement ou hors horaires)
Paul    09:35 vaccination   → réservé

Carine annule son échographie : ok
Inès demande le créneau libéré de 09:00 → réservé

Agenda de Dr Houngbo le 03/11/2026 :
   08:00-08:20  Awa (consultation)
   08:25-08:40  Kofi (suivi)
```
