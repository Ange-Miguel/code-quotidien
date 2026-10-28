# Jour 095 — Conversion de fuseaux horaires (Cotonou, Paris, Pékin)

**Langue :** Python

## Le problème
Un importateur parle avec un fournisseur à Guangzhou, un client à Paris et un associé à Montréal. Quelle heure est-il chez chacun, et quel créneau de réunion convient à tout le monde pendant les heures de bureau ?

## La solution
On travaille en temps universel (UTC) et on applique le décalage de chaque ville. Cotonou (UTC+1) et Pékin (UTC+8) n'ont pas d'heure d'été ; Paris (UTC+1/+2) et Montréal (UTC−5/−4) en ont une. On calcule nous-mêmes les dates de changement d'heure (règles de l'Union européenne et de l'Amérique du Nord), sans base de données de fuseaux, puis on cherche les heures où tout le monde est entre 8 h et 18 h.

## Ce qu'on apprend
L'UTC, les décalages horaires, le calcul du « dernier dimanche du mois », timezone et timedelta du module datetime.

## Lancer
```bash
python3 fuseaux.py
```

## Exemple de sortie
```
Changement d'heure 2026 à Paris : 29/03 et 25/10

À 2026-10-20 09:00 UTC :
   Cotonou   mar 20 10:00 (UTC+1)
   Paris     mar 20 11:00 (UTC+2)
   Pékin     mar 20 17:00 (UTC+8)
   Montréal  mar 20 05:00 (UTC-4)
À 2026-11-20 09:00 UTC :
   Cotonou   ven 20 10:00 (UTC+1)
   Paris     ven 20 10:00 (UTC+1)
   Pékin     ven 20 17:00 (UTC+8)
   Montréal  ven 20 04:00 (UTC-5)

Réunion Cotonou + Pékin (8 h–18 h chez tous) : 07h UTC, 08h UTC, 09h UTC

```
