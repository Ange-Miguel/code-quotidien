# Jour 196 — Simulation de propagation d'une rumeur dans un réseau

**Langue :** C

## Le problème
Comment une information (vraie ou fausse) se répand-elle dans un groupe WhatsApp, un quartier ou une école ? Pourquoi certaines rumeurs s'éteignent-elles vite alors que d'autres touchent tout le monde ? Et qu'est-ce qui freine le plus une fausse information ?

## La solution
Un réseau social simulé (graphe « petit monde » : chacun connaît ses voisins proches plus quelques personnes au hasard) et le modèle de propagation à trois états : ignorant, propagateur, sceptique. Chaque jour, un propagateur transmet la rumeur à chacun de ses contacts avec une probabilité p ; il cesse de la propager quand il rencontre quelqu'un qui la connaît déjà (perte d'intérêt) ou après vérification. On compare plusieurs scénarios : rumeur peu partagée, très partagée, et avec démenti précoce.

## Ce qu'on apprend
Les graphes en listes d'adjacence, le modèle de propagation (proche des modèles d'épidémie), la simulation stochastique et la moyenne sur plusieurs essais.

## Lancer
```bash
gcc -Wall -Wextra -o rumeur rumeur.c -lm
./rumeur
```

## Exemple de sortie
```
Réseau de 500 personnes (6.4 contacts en moyenne), 30 essais par scénario

Scénario                                Touchés   Durée
Rumeur peu partagée (p=0,10)                 40%     67 j  ################
Rumeur virale (p=0,40)                        94%     25 j  #####################################
Virale, démenti au jour 3                     8%     10 j  ###
Virale, démenti au jour 8                    35%     18 j  ##############
Virale, public prudent (vérif. 25 %)         49%     27 j  ###################

Un démenti rapide et un public qui vérifie freinent bien plus la rumeur qu'un démenti tardif.
```
