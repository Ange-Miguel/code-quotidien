# Jour 123 — Estimation d'autonomie d'une batterie selon la consommation

**Langue :** Java

## Le problème
Un capteur IoT sur batterie (station météo, traceur, compteur d'eau) doit tenir des mois sans intervention. Combien de temps dure une batterie Li-ion de 3 000 mAh si l'appareil dort la plupart du temps et se réveille pour mesurer et envoyer ?

## La solution
On décrit le cycle de fonctionnement en phases (sommeil profond, mesure, émission Wi-Fi ou LoRa) avec leur courant et leur durée. Le courant moyen est la moyenne pondérée par le temps ; l'autonomie = capacité utile (avec marge pour l'autodécharge et le vieillissement) / courant moyen. On compare plusieurs intervalles d'envoi et deux technologies radio, et on cherche l'intervalle minimal pour tenir un an.

## Ce qu'on apprend
Le bilan énergétique d'un objet connecté, la moyenne pondérée, l'impact énorme du mode sommeil, et la recherche d'un paramètre par essais.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Batterie 3 000 mAh (utile 2 400 mAh), sommeil profond 15 µA

Intervalle       Wi-Fi (mA)          Wi-Fi      LoRa (mA)           LoRa
1 min                 7,406          14 j         0,4398         227 j
5 min                 1,493          67 j         0,1000        1000 j
15 min                0,508         197 j         0,0433        2308 j
60 min                0,138         724 j         0,0221        4529 j
240 min               0,046        2184 j         0,0168        5963 j

Pour tenir 1 an en Wi-Fi, il faut envoyer au plus toutes les 29 minutes.
Sans mode sommeil (25 mA en attente) : 3,9 jours seulement !
```
