# Jour 166 — Suivi du temps passé par projet freelance

**Langue :** C

## Le problème
Un freelance qui jongle entre plusieurs clients ne sait pas combien de temps il a vraiment passé sur chaque projet. Résultat : il sous- estime ses devis et certains projets « au forfait » deviennent non rentables.

## La solution
Un journal de sessions de travail (projet, début, fin) est lu depuis un fichier au format « AAAA-MM-JJ HH:MM HH:MM projet ». Le programme cumule le temps par projet (en gérant les sessions qui passent minuit), compare au temps prévu dans le devis, calcule le taux horaire réel (montant du forfait / heures passées) et signale les projets en dépassement. Données d'exemple dans un fichier temporaire.

## Ce qu'on apprend
La lecture de fichier ligne par ligne, sscanf avec plusieurs champs, les calculs d'heures en minutes et l'analyse de rentabilité.

## Lancer
```bash
gcc -Wall -Wextra -o suivi_temps suivi_temps.c -lm
./suivi_temps
```

## Exemple de sortie
```
Projet             Passé   Prévu Avancement   Taux réel/h
logo-boutique        9h30      8h       119 %      7895 FCFA  ⚠ dépassement de 1.5 h
motion-asso         26h15     20h       131 %      5714 FCFA  ⚠ dépassement de 6.2 h
site-restaurant      7h00     25h        28 %     28571 FCFA
app-zem              3h00     60h         5 %    200000 FCFA

Astuce : comparez le taux réel à votre taux cible pour ajuster vos prochains devis.
  ligne 12 ignorée : 2026-10-08 25:00 26:00 erreur
```
