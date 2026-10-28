# Jour 141 — Détection de mouvement simulée et règles d'alarme

**Langue :** Java

## Le problème
Un système d'alarme maison avec détecteurs de mouvement (PIR) et contacts de porte doit sonner pour un vrai intrus, mais pas pour le chat, une porte qui claque au vent ou le propriétaire qui rentre et désarme. Trop de fausses alarmes, et plus personne n'y prête attention.

## La solution
Une machine à états d'alarme (DÉSARMÉE, ARMÉE, TEMPORISATION D'ENTRÉE, ALARME) et des règles de confirmation : la porte d'entrée lance un délai de 30 s pour taper le code ; un détecteur intérieur seul ne déclenche que s'il est confirmé par un second détecteur ou un deuxième passage dans les 60 s (filtre anti-animal) ; un code faux trois fois déclenche l'alarme. On rejoue trois scénarios : retour du propriétaire, chat, intrus.

## Ce qu'on apprend
Les machines à états avec temporisations, la corrélation d'événements dans une fenêtre de temps et la réduction des fausses alarmes.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Retour du propriétaire (alarme armée) :
   t=  0s  porte            ouverture                  ARMEE → TEMPORISATION
   t=  4s  pir-couloir      mouvement                  TEMPORISATION
   t= 12s  clavier          code faux                  TEMPORISATION
   t= 18s  clavier          code correct               TEMPORISATION → DESARMEE

Le chat passe deux fois très vite (alarme armée) :
   t=100s  pir-salon        mouvement                  ARMEE
   t=102s  pir-salon        mouvement                  ARMEE
   t=400s  pir-salon        mouvement                  ARMEE

Intrus par la fenêtre (alarme armée) :
   t=200s  pir-salon        mouvement                  ARMEE
   t=214s  pir-couloir      mouvement                  ARMEE → ALARME 🔔

```
