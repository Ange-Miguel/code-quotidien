# Jour 140 — Simulation d'une maison connectée : lampes, portes, capteurs

**Langue :** Python

## Le problème
Avant d'acheter du matériel domotique, on veut tester les scénarios : « quand la porte d'entrée s'ouvre la nuit, allumer l'entrée et prévenir », « à 22 h, tout éteindre sauf la chambre », « s'il fait plus de 30 °C dans le salon, lancer le ventilateur ». Tout cela peut se simuler.

## La solution
Des classes pour chaque type d'appareil (Lampe, Porte, Capteur, Ventilateur) qui héritent d'une classe Appareil ; un bus d'événements simple (observateur) : chaque changement d'état publie un événement ; des règles (condition + action) écoutent ces événements. On rejoue une journée simulée et on affiche le journal des automatismes déclenchés.

## Ce qu'on apprend
L'héritage et le polymorphisme, le patron de conception Observateur, les règles « si … alors … » et la simulation par événements.

## Lancer
```bash
python3 maison_connectee.py
```

## Exemple de sortie
```
Journal de la journée :
   06:30  porte d'entrée     ouverte = True
   06:30  lampe entrée       allumee = True
   06:30  ⚑ notification : porte ouverte de nuit
   06:31  porte d'entrée     ouverte = False
   12:00  sonde salon        temperature = 29.5
   13:30  sonde salon        temperature = 31.2
   13:30  ventilateur salon  marche = True
   16:00  sonde salon        temperature = 30.4
   18:00  sonde salon        temperature = 27.6
   18:00  ventilateur salon  marche = False
   19:30  lampe salon        allumee = True
   20:00  lampe chambre      allumee = True
   21:15  porte d'entrée     ouverte = True
   21:15  ⚑ notification : porte ouverte de nuit
```
