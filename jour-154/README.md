# Jour 154 — Gestion de file d'attente d'une clinique avec priorités

**Langue :** C

## Le problème
Dans une clinique, le premier arrivé n'est pas toujours le premier soigné : une urgence passe avant une consultation de routine. Mais il ne faut pas non plus qu'un patient « peu urgent » attende indéfiniment.

## La solution
Un tas binaire (file de priorité) implémenté dans un tableau. La priorité dépend du niveau de triage (1 = urgence vitale … 4 = non urgent) et du temps d'attente : chaque tranche de 30 minutes d'attente améliore la priorité (vieillissement), et l'ordre d'arrivée départage les égalités. On simule une matinée avec deux médecins et on mesure l'attente par niveau. Les niveaux de triage sont simplifiés, pour l'exemple.

## Ce qu'on apprend
Le tas binaire (insertion et extraction en O(log n)), les fonctions de comparaison à plusieurs critères et le vieillissement des priorités pour éviter la famine.

## Lancer
```bash
gcc -Wall -Wextra -o file_clinique file_clinique.c -lm
./file_clinique
```

## Exemple de sortie
```
Heure   Médecin  Patient            Triage        Attente
08h00   Dr A     M. Adjovi          non urgent      0 min
08h02   Dr B     Mme Dossou         semi-urgent     0 min
08h15   Dr A     M. Houngbo         vital           3 min
08h17   Dr B     Bébé Kpade       urgent         12 min
08h40   Dr A     Mlle Sossou        urgent          9 min
08h42   Dr B     M. Zinsou          non urgent     34 min
08h57   Dr B     Mme Tchibozo       semi-urgent    37 min
09h05   Dr A     Mme Agossa         non urgent     55 min
09h12   Dr B     M. Mensah          non urgent     47 min
09h20   Dr A     Mme Ahouandjinou   semi-urgent    35 min
09h27   Dr B     M. Gbaguidi        non urgent     47 min
09h35   Dr A     M. Dah             non urgent     45 min

Attente moyenne par niveau :  vital 3 min  urgent 10 min  semi-urgent 24 min  non urgent 38 min
```
