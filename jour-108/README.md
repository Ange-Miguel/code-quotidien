# Jour 108 — Machine à états d'un feu tricolore

**Langue :** Java

## Le problème
Un carrefour avec feux pour voitures et bouton d'appel piéton. Les règles de sécurité sont strictes : jamais de vert en même temps pour les voitures et les piétons, durées minimales, et un mode « orange clignotant » en cas de panne. C'est un exemple typique de programme embarqué.

## La solution
Une machine à états avec une énumération d'états, chacun portant sa durée et l'état des lampes. Une boucle avance d'une seconde à la fois ; les événements (bouton piéton, panne, réparation) sont traités par une méthode de transition. Une assertion vérifie à chaque seconde l'invariant de sécurité.

## Ce qu'on apprend
Les enums riches (avec attributs et méthodes), la machine à états temporisée, les invariants de sécurité et la simulation pas à pas.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
t=  0s  VERT_VOITURES   voitures : VERT               piétons : rouge
t=  5s  événement : bouton
t= 20s  ORANGE          voitures : ORANGE             piétons : rouge
t= 23s  ROUGE_SECURITE  voitures : rouge              piétons : rouge
t= 25s  VERT_PIETONS    voitures : rouge              piétons : VERT
t= 35s  PIETONS_FIN     voitures : rouge              piétons : vert clignotant
t= 38s  VERT_VOITURES   voitures : VERT               piétons : rouge
t= 50s  événement : bouton
t= 58s  événement : bouton
t= 58s  ORANGE          voitures : ORANGE             piétons : rouge
t= 61s  ROUGE_SECURITE  voitures : rouge              piétons : rouge
t= 63s  VERT_PIETONS    voitures : rouge              piétons : VERT
t= 73s  PIETONS_FIN     voitures : rouge              piétons : vert clignotant
t= 75s  événement : panne
t= 75s  CLIGNOTANT      voitures : orange clignotant  piétons : éteint
```
