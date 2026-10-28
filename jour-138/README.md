# Jour 138 — Socket UDP : diffusion de découverte d'appareils

**Langue :** Java

## Le problème
Quand on branche une prise connectée ou une caméra IP, comment l'application du téléphone la trouve-t-elle sans connaître son adresse IP ? Par la découverte : l'application envoie un message « qui est là ? » et chaque appareil répond avec son nom, son type et ses capacités.

## La solution
UDP, sans connexion, est adapté à ces petits messages. Sur un vrai réseau, la question part en diffusion (broadcast 255.255.255.255) ; pour que la démo marche sur n'importe quelle machine, les appareils simulés écoutent chacun sur un port local et la question leur est envoyée un par un. On gère les délais d'attente (un appareil éteint ne répond pas), les réponses dans le désordre et les messages invalides.

## Ce qu'on apprend
DatagramSocket et DatagramPacket, la différence entre UDP et TCP, les délais d'attente (setSoTimeout) et un protocole texte simple.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Question « DECOUVERTE?v1 » envoyée, attente des réponses…
    17 ms  prise-salon     type prise    capacités [on,off,conso]  (port 51931)
    27 ms  camera-portail  type camera   capacités [flux,photo]  (port 39389)
    38 ms  thermostat      type climat   capacités [consigne,mesure]  (port 46917)
3 appareil(s) trouvé(s) sur 4 ; sans réponse : [capteur-cuve]
```
