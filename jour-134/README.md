# Jour 134 — Broker MQTT minimal en mémoire avec topics à jokers

**Langue :** Python

## Le problème
Dans MQTT, un abonné peut écouter plusieurs sujets à la fois avec des jokers : « maison/+/temperature » (toutes les pièces) ou « maison/# » (tout ce qui concerne la maison). Le broker (le serveur central) doit savoir à qui livrer chaque message, avec une qualité de service et des messages retenus.

## La solution
Un broker en mémoire. Les abonnements sont stockés avec leur filtre ; la correspondance sujet / filtre suit exactement les règles MQTT : « + » remplace un niveau, « # » (seulement à la fin) remplace tous les niveaux restants, y compris aucun. On teste les règles avec des cas connus, puis on simule une maison avec plusieurs capteurs et applications.

## Ce qu'on apprend
Les règles de correspondance des topics MQTT, la découpe par niveaux, la validation des filtres et la conception d'un broker simple.

## Lancer
```bash
python3 broker_mqtt.py
```

## Exemple de sortie
```
8 tests de correspondance réussis

PUBLICATION maison/salon/temperature     « 28.4 » → 3 abonné(s)
PUBLICATION maison/chambre/temperature   « 26.1 » → 3 abonné(s)
PUBLICATION maison/entree/mouvement      « 1 » → 3 abonné(s)
PUBLICATION maison/portail/etat          « ouvert » → 3 abonné(s)
PUBLICATION ferme/serre/humidite         « 64 » → 1 abonné(s)

tableau-de-bord  a reçu 5 message(s) : portail/etat, salon/temperature, chambre/temperature, entree/mouvement, portail/etat
securite         a reçu 3 message(s) : portail/etat, entree/mouvement, portail/etat
historique       a reçu 6 message(s) : portail/etat, salon/temperature, chambre/temperature, entree/mouvement, portail/etat, serre/humidite
climatisation    a reçu 2 message(s) : salon/temperature, chambre/temperature

Abonnement refusé : filtre invalide : maison/#/salon
```
