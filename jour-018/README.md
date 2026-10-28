# Jour 018 — Calculateur d'âge exact (années, mois, jours)

**Langue :** Java

## Le problème
Pour un formulaire, un concours ou un dossier administratif, il faut connaître son âge exact à une date donnée, et le nombre de jours avant le prochain anniversaire.

## La solution
On utilise java.time : Period.between donne l'écart en années, mois et jours ; ChronoUnit.DAYS donne le total de jours vécus. Le cas du 29 février est géré automatiquement par la bibliothèque.

## Ce qu'on apprend
L'API java.time (LocalDate, Period, ChronoUnit), le parsing de dates au format ISO et la gestion des dates impossibles.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Né(e) le 2005-03-14, au 2026-10-28 :
  Âge : 21 an(s), 7 mois, 14 jour(s)
  Jours vécus : 7 898
  Prochain anniversaire : 2027-03-14 (dans 137 jour(s))

Né(e) le 2004-02-29, au 2026-10-28 :
  Âge : 22 an(s), 7 mois, 29 jour(s)
  Jours vécus : 8 277
  Prochain anniversaire : 2027-02-28 (dans 123 jour(s))

Né(e) le 1960-08-01, au 2026-10-28 :
  Âge : 66 an(s), 2 mois, 27 jour(s)
  Jours vécus : 24 194
  Prochain anniversaire : 2027-08-01 (dans 277 jour(s))
```
