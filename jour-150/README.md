# Jour 150 — Bilan de la phase 3 : passerelle IoT simulée complète

**Langue :** Java

## Le problème
Réunir en un seul programme les briques des jours 101 à 149 : des capteurs envoient des trames binaires avec contrôle d'intégrité, une passerelle les vérifie, filtre les mesures, détecte les anomalies, publie sur des sujets de type MQTT, applique des règles d'automatisme et journalise le tout. C'est l'architecture d'une vraie installation IoT.

## La solution
Plusieurs classes qui coopèrent : Capteur (produit des trames signées avec CRC et compteur anti-rejeu), Passerelle (décode, vérifie, filtre par médiane, calcule un score z), Bus (publier / s'abonner avec jokers), Regles (actions domotiques) et Journal. Une simulation de 2 heures avec un capteur défaillant, des trames corrompues et un rejeu montre que chaque protection joue son rôle.

## Ce qu'on apprend
L'architecture d'un système IoT de bout en bout, la composition de classes, et comment les mécanismes vus séparément se complètent.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
 16 min  ✘ trame corrompue (CRC)
 20 min  ✘ trame corrompue (CRC)
        ▶ serre/zone2/temperature = 33,2 °C : ventilation ON
 22 min  ✘ trame corrompue (CRC)
 44 min  ⚠ capteur 1 : valeur aberrante 85,0 (z=27), filtrée
 59 min  ✘ trame corrompue (CRC)
 64 min  ✘ trame corrompue (CRC)
 70 min  ✘ capteur 2 : rejeu refusé
        ▶ toutes zones < 31 °C : ventilation OFF
 91 min  ✘ trame corrompue (CRC)
 96 min  ✘ trame corrompue (CRC)
106 min  ✘ trame corrompue (CRC)
110 min  ✘ trame corrompue (CRC)
114 min  ✘ trame corrompue (CRC)

```
