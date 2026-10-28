# Jour 119 — Alerte de niveau d'eau : pompe automatique

**Langue :** Python

## Le problème
La citerne du toit (jour 118) se remplit grâce à une pompe qui tire l'eau d'un puits ou d'un réservoir au sol. Il faut démarrer la pompe automatiquement quand le niveau est bas, l'arrêter quand la citerne est pleine, et surtout protéger la pompe : jamais de fonctionnement à sec, et pas de démarrages trop rapprochés.

## La solution
Un automate avec deux seuils (hystérésis : marche à 30 %, arrêt à 95 %), une sécurité « manque d'eau » (le niveau ne monte pas alors que la pompe tourne depuis 3 minutes → arrêt et alarme), un délai minimal entre deux démarrages et une alerte par message. On simule une journée de consommation domestique (pics le matin et le soir) et on affiche le journal.

## Ce qu'on apprend
La logique de commande avec sécurités, la gestion d'états temporisés, la simulation d'un système physique et la journalisation.

## Lancer
```bash
python3 pompe_auto.py
```

## Exemple de sortie
```
Journal de la journée (le puits est à sec entre 17 h et 19 h) :
   06h43  démarrage pompe (citerne à 30 %)
   07h33  arrêt pompe (citerne à 96 %)
   13h37  démarrage pompe (citerne à 30 %)
   14h13  arrêt pompe (citerne à 97 %)
   18h51  démarrage pompe (citerne à 30 %)
   18h54  ⚠ ALARME : le niveau ne monte pas (puits vide ?) — pompe arrêtée, SMS envoyé

3 démarrage(s), niveau minimal 0 %, alarme active
```
