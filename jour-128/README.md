# Jour 128 — Watchdog simulé : redémarrage si une tâche bloque

**Langue :** Python

## Le problème
Un objet connecté installé sur un toit ou dans un champ ne doit jamais rester planté : un capteur I2C qui ne répond plus, une boucle infinie, une connexion réseau bloquée… et l'appareil ne transmet plus rien pendant des semaines. Personne n'est là pour appuyer sur « reset ».

## La solution
Le chien de garde (watchdog). C'est un compteur matériel qui redémarre le microcontrôleur s'il n'est pas « nourri » (remis à zéro) régulièrement. Le programme ne le nourrit que si toutes les tâches ont donné signe de vie. On simule un appareil dont une tâche se bloque au hasard ; le watchdog détecte le blocage, redémarre, et on enregistre la cause du redémarrage pour le diagnostic.

## Ce qu'on apprend
Le principe du watchdog, le « nourrissage » conditionnel, la reprise après redémarrage et la journalisation des causes de reset.

## Lancer
```bash
python3 watchdog.py
```

## Exemple de sortie
```
24 h de fonctionnement simulées (boucle de 100 ms, watchdog de 2000 ms)

Sans watchdog :   9.9% des mesures envoyées (bloqué sur « stockage »)
Avec watchdog : 100.0% des mesures envoyées, 11 redémarrage(s)

Journal des causes de redémarrage (conservé en mémoire non volatile) :
   02h22m53s  tâche bloquée : radio
   04h18m48s  tâche bloquée : radio
   05h48m19s  tâche bloquée : stockage
   06h26m54s  tâche bloquée : capteurs
   08h00m31s  tâche bloquée : stockage
   14h52m47s  tâche bloquée : radio
   16h04m04s  tâche bloquée : capteurs
   17h25m04s  tâche bloquée : radio
   … et 3 autre(s)
```
