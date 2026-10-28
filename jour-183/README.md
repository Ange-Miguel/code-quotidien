# Jour 183 — Répartition équitable des tâches ménagères

**Langue :** Java

## Le problème
Dans une colocation ou une famille, les tâches (vaisselle, balayage, courses, eau, linge) sont rarement réparties équitablement. Les tâches pénibles retombent toujours sur les mêmes, et on se dispute.

## La solution
Chaque tâche a un « poids » (pénibilité × durée) et une fréquence par semaine. On répartit les occurrences de la semaine en donnant à chaque fois la tâche la plus lourde restante à la personne la moins chargée qui est disponible ce jour-là (algorithme LPT, « plus longue tâche d'abord », avec une file de priorité). On tient compte des absences et de préférences (une personne qui déteste une tâche la reçoit en dernier recours). On affiche le planning et la charge de chacun.

## Ce qu'on apprend
L'équilibrage de charge, l'algorithme LPT, PriorityQueue et la prise en compte de contraintes simples.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Planning de la semaine :
   Lun : Linge (Kofi), Courses au marché (Ange), Puiser l'eau (Ange), Vaisselle (Awa), Balayer (Kofi), Poubelles (Kofi)
   Mar : Vaisselle (Kofi), Balayer (Kofi)
   Mer : Puiser l'eau (Serge), Vaisselle (Awa), Poubelles (Kofi)
   Jeu : Courses au marché (Awa), Vaisselle (Serge), Balayer (Ange)
   Ven : Puiser l'eau (Serge), Vaisselle (Ange), Poubelles (Awa)
   Sam : Vaisselle (Awa), Balayer (Ange)
   Dim : Vaisselle (Serge)

Charge de chacun (points de pénibilité) :
   Ange    16  ▇▇▇▇▇▇▇▇
   Kofi    15  ▇▇▇▇▇▇▇
   Awa     15  ▇▇▇▇▇▇▇
   Serge   14  ▇▇▇▇▇▇▇
Écart entre le plus et le moins chargé : 2 points sur 60 au total (3 %)
```
