# Jour 137 — Socket TCP : échange de messages capteur ↔ passerelle

**Langue :** Python

## Le problème
Plusieurs capteurs d'un même site (atelier, ferme) envoient leurs mesures à une passerelle locale, qui peut aussi leur répondre (accusé de réception, nouvelle consigne). TCP garantit l'ordre et la livraison, mais c'est un flux d'octets : il faut découper soi-même les messages.

## La solution
Un serveur TCP (la passerelle) qui accepte plusieurs clients en parallèle grâce aux threads, et un protocole simple : chaque message est une ligne JSON terminée par « \\n ». Les capteurs se connectent, s'identifient, envoient leurs mesures et reçoivent un accusé de réception ; la passerelle renvoie une nouvelle période de mesure si la température est élevée. Tout fonctionne sur la machine locale (127.0.0.1), sur un port choisi par le système.

## Ce qu'on apprend
Socket, bind, listen, accept, connect, le découpage d'un flux en messages (makefile/readline), les threads et le verrouillage d'une structure partagée.

## Lancer
```bash
python3 socket_tcp.py
```

## Exemple de sortie
```
Passerelle à l'écoute sur 127.0.0.1:43283

   capteur-serre connecté, période 60 s
   capteur-serre envoie 29.5 °C → ok, prochaine mesure dans 60 s
   capteur-serre envoie 30.1 °C → ok, prochaine mesure dans 60 s
   capteur-serre envoie un message abîmé → {'erreur': 'JSON invalide'}
   capteur-four connecté, période 60 s
   capteur-four envoie 33.0 °C → ok, prochaine mesure dans 60 s
   capteur-four envoie 36.4 °C → ok, prochaine mesure dans 10 s
   capteur-four envoie 38.2 °C → ok, prochaine mesure dans 10 s
   capteur-four envoie un message abîmé → {'erreur': 'JSON invalide'}
   capteur-stock connecté, période 60 s
   capteur-stock envoie 24.0 °C → ok, prochaine mesure dans 60 s
   capteur-stock envoie un message abîmé → {'erreur': 'JSON invalide'}

```
