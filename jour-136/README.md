# Jour 136 — Client HTTP qui envoie des mesures à une API

**Langue :** C

## Le problème
Une carte (Raspberry Pi, passerelle Linux) doit envoyer ses mesures à un serveur web par une requête HTTP POST, comme le font les plateformes IoT. Comprendre ce qui circule réellement sur le réseau aide à déboguer quand « ça ne marche pas ».

## La solution
On écrit la requête HTTP à la main avec les sockets POSIX : ligne de requête, en-têtes (Host, Content-Type, Content-Length, Connection) et corps JSON, puis on lit et analyse la réponse (code de statut). Pour que la démo fonctionne sans Internet, le programme lance avec fork() un mini serveur local qui reçoit la requête, l'affiche et répond « 201 Created ».

## Ce qu'on apprend
Socket, connect, send/recv, le format exact d'une requête HTTP, le calcul de Content-Length et fork() pour créer un processus.

## Lancer
```bash
gcc -Wall -Wextra -o client_http client_http.c -lm
./client_http
```

## Exemple de sortie
```
SERVEUR reçu 179 octets :
        | POST /api/mesures HTTP/1.1
        | Host: 127.0.0.1:55031
        | Content-Type: application/json
        | Content-Length: 55
        | Connection: close
        | {"capteur":"serre-01","temperature":28.4,"humidite":71}
CLIENT  ← HTTP/1.1 201 Created
CLIENT  mesure 1 : acceptée (code 201)

SERVEUR reçu 179 octets :
        | POST /api/mesures HTTP/1.1
        | Host: 127.0.0.1:55031
        | Content-Type: application/json
        | Content-Length: 55
```
