# Jour 133 — Simulation d'un client MQTT : publier/s'abonner (en mémoire)

**Langue :** C

## Le problème
En IoT, des dizaines de capteurs et d'applications échangent des messages sans se connaître directement. MQTT, le protocole le plus utilisé, repose sur le modèle « publier / s'abonner » : un capteur publie sur un sujet (topic) comme maison/salon/temperature, et tous les abonnés à ce sujet reçoivent le message.

## La solution
On simule en mémoire, en C, le cœur du modèle : une table d'abonnements (sujet → fonction de rappel), une file de messages, la publication qui distribue aux abonnés concernés, le désabonnement et les messages « retenus » (retain) livrés immédiatement aux nouveaux abonnés. Le vrai réseau sera ajouté plus tard ; ici on comprend le principe.

## Ce qu'on apprend
Le modèle publier/s'abonner, les pointeurs de fonction, les tables de structures avec chaînes de caractères et le découplage entre producteurs et consommateurs de données.

## Lancer
```bash
gcc -Wall -Wextra -o mqtt_memoire mqtt_memoire.c -lm
./mqtt_memoire
```

## Exemple de sortie
```
PUBLICATION maison/salon/temperature     « 26.4 »
   → [appli-mobile] affiche maison/salon/temperature = 26.4
   → [climatiseur] maison/salon/temperature : rien à faire
   → [historique] enregistre (maison/salon/temperature, 26.4)
PUBLICATION maison/salon/temperature     « 28.1 » [retenu]
   → [appli-mobile] affiche maison/salon/temperature = 28.1
   → [climatiseur] maison/salon/temperature : démarrage du refroidissement
   → [historique] enregistre (maison/salon/temperature, 28.1)
PUBLICATION maison/cuisine/gaz           « 0.02 »
   (aucun abonné)
PUBLICATION maison/portail/etat          « fermé » [retenu]
   → [appli-mobile] affiche maison/portail/etat = fermé

L'historique se désabonne ; une tablette se connecte plus tard :
   → [tablette] affiche maison/portail/etat = fermé
```
