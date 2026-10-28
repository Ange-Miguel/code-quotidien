# Jour 135 — Serveur HTTP minimal qui renvoie l'état d'un capteur

**Langue :** Java

## Le problème
On veut consulter la température et l'humidité de sa serre ou de sa chambre froide depuis le navigateur du téléphone, sur le réseau local, sans installer d'application.

## La solution
Un petit serveur HTTP avec com.sun.net.httpserver (inclus dans le JDK). Deux routes : « / » renvoie une page HTML lisible sur mobile, et « /api/capteur » renvoie l'état en JSON pour une application. Les mesures évoluent dans un thread séparé ; un AtomicReference garantit qu'on lit toujours un état cohérent. En démo, le programme démarre le serveur sur un port libre, s'interroge lui-même avec HttpClient puis s'arrête ; avec --serveur il reste allumé sur le port 8080.

## Ce qu'on apprend
Le protocole HTTP (méthode, chemin, code de statut, en-têtes), HttpServer et HttpClient, le JSON fait main et le partage d'état entre threads.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Serveur démarré sur un port libre ; interrogation automatique :
GET /api/capteur  → 200 application/json; charset=utf-8
   {"temperature":28.0,"humidite":65,"heure":"09:21:25"}
GET /             → 200 text/html; charset=utf-8
   <h1>Serre</h1><p>Température : <b>28,0 °C</b></p><p>Humidité…
GET /inconnu      → 404 text/plain; charset=utf-8
   Page introuvable
POST /api/capteur → 405 Méthode non autorisée
Serveur arrêté.
```
