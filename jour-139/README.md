# Jour 139 — Protocole de commande domotique texte (ON/OFF/STATUS)

**Langue :** C

## Le problème
Une carte domotique (Arduino, ESP32) reçoit des ordres par liaison série, Bluetooth ou Wi-Fi. Un protocole texte simple, que l'on peut taper à la main dans un terminal, facilite énormément les tests et le dépannage : « ON LAMPE1 », « DIM SALON 40 », « STATUS ».

## La solution
Chaque ligne est découpée en mots ; le premier est la commande, cherchée dans une table (nom, nombre d'arguments, fonction). Chaque commande vérifie ses arguments et répond toujours par « OK … » ou « ERR code message », ce qui permet à un programme de lire la réponse facilement. Les sorties (relais, variateur) sont simulées dans un tableau d'état. Une commande HELP liste les commandes disponibles.

## Ce qu'on apprend
La conception d'un protocole texte, les tables de dispatch (pointeurs de fonction), la validation des arguments et les codes d'erreur.

## Lancer
```bash
gcc -Wall -Wextra -o protocole_domotique protocole_domotique.c -lm
./protocole_domotique
```

## Exemple de sortie
```
> HELP
OK | ON <sortie> | OFF <sortie> | DIM <sortie> <0-100> | STATUS | HELP
> ON lampe1
OK LAMPE1=ON
> DIM salon 40
OK SALON=40%
> dim SALON 140
ERR 422 niveau 0-100 attendu
> ON GARAGE
ERR 404 sortie inconnue
> OFF
ERR 400 usage : OFF <sortie>
> ALLUMER LAMPE2
ERR 405 commande inconnue (HELP pour la liste)
> on pompe
```
