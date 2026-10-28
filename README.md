# Code quotidien

Un petit programme par jour pendant **200 jours**, en **C**, **Python** ou **Java**.
Chaque programme résout un problème concret du quotidien, du plus simple au plus avancé, avec une orientation **systèmes embarqués et IoT**.

## Organisation
- `PLAN.md` : la liste chronologique des 200 problèmes et leur statut
- `jour-XXX/` : le code du jour et un `README.md` qui explique le problème, la solution et ce qu'on apprend

## Les 4 phases
1. **Jours 1-50** — Les bases utiles
2. **Jours 51-100** — Structures de données et fichiers
3. **Jours 101-150** — Systèmes embarqués et IoT
4. **Jours 151-200** — Problèmes réels et mini-projets

## Automatisation
Une GitHub Action (`.github/workflows/publication-quotidienne.yml`) s'exécute chaque jour à 8 h 45 (heure du Bénin).
Elle prend le prochain programme dans la branche `reserve`, coche la ligne dans `PLAN.md` et publie le commit du jour.
On peut aussi la lancer à la main depuis l'onglet **Actions** → **Publication quotidienne** → **Run workflow**.

Auteur : Ange-Miguel AKPANON.
