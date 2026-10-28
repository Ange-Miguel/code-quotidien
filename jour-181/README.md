# Jour 181 — Planification de tournées de livraison (plus proche voisin)

**Langue :** C

## Le problème
Un livreur à moto doit déposer une douzaine de colis entre Cotonou et Calavi, puis revenir au dépôt. Dans quel ordre passer pour faire le moins de kilomètres possible ? C'est le célèbre problème du voyageur de commerce : impossible d'essayer tous les ordres (12! = 479 millions).

## La solution
L'heuristique du plus proche voisin (toujours aller au client le plus proche non encore servi) donne vite une tournée correcte. On l'améliore ensuite avec 2-opt : on inverse un morceau de la tournée tant que cela raccourcit le trajet (cela supprime les croisements). On compare avec l'ordre de saisie des commandes. Coordonnées fictives en kilomètres.

## Ce qu'on apprend
Les heuristiques pour les problèmes difficiles, l'algorithme glouton du plus proche voisin, l'amélioration locale 2-opt et la mesure du gain obtenu.

## Lancer
```bash
gcc -Wall -Wextra -o tournee_livraison tournee_livraison.c -lm
./tournee_livraison
```

## Exemple de sortie
```
Ordre de saisie des commandes : 43.2 km
   Dépôt Akpakpa → Ganhi → Cadjèhoun → Fidjrossè → Godomey → Calavi Kpota → Zogbadjè → Akassato → Vèdoko → Agla → Haie Vive → Cococodji → Gbégamey → Dépôt Akpakpa

Plus proche voisin : 35.6 km
   Dépôt Akpakpa → Ganhi → Gbégamey → Vèdoko → Haie Vive → Cadjèhoun → Agla → Godomey → Cococodji → Zogbadjè → Calavi Kpota → Akassato → Fidjrossè → Dépôt Akpakpa

Après amélioration 2-opt : 31.9 km
   Dépôt Akpakpa → Ganhi → Gbégamey → Cadjèhoun → Fidjrossè → Cococodji → Zogbadjè → Akassato → Calavi Kpota → Godomey → Agla → Haie Vive → Vèdoko → Dépôt Akpakpa

2-opt : 7 inversion(s), gain 3.7 km ; gain total 26 % par rapport à l'ordre de saisie
Carburant économisé sur 20 tournées par mois : ~6 L (2,5 L/100 km)
```
