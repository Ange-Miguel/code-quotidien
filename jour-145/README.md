# Jour 145 — Détection d'anomalie sur des mesures (score z)

**Langue :** C

## Le problème
Un moteur de pompe, un compresseur ou un groupe électrogène qui commence à s'abîmer vibre ou chauffe un peu plus que d'habitude, bien avant de casser. Détecter tôt ces écarts permet une maintenance préventive.

## La solution
On apprend en continu la moyenne et la variance du signal normal avec l'algorithme de Welford (en ligne, une passe, numériquement stable, sans stocker l'historique : idéal sur microcontrôleur). Pour chaque nouvelle mesure, le score z = (x − moyenne) / écart-type indique à combien d'écarts-types elle se trouve. Au-delà de 3, c'est une anomalie, qu'on n'intègre pas dans l'apprentissage pour ne pas « s'habituer » à la panne.

## Ce qu'on apprend
L'algorithme de Welford, le score z, les statistiques en ligne et la détection de dérive progressive (score moyen sur une fenêtre).

## Lancer
```bash
gcc -Wall -Wextra -o anomalie_zscore anomalie_zscore.c -lm
./anomalie_zscore
```

## Exemple de sortie
```
Surveillance des vibrations d'une pompe (1 mesure par minute), seuil z = 3.0

t=250 min  4.97 mm/s  z = +16.2  → ANOMALIE ponctuelle
t=420 min  4.86 mm/s  z = +15.3  → ANOMALIE ponctuelle
t=670 min  2.76 mm/s  z =  +3.0  → ANOMALIE ponctuelle
t=670 min  z moyen sur 20 min = +1.5 → DÉRIVE PROGRESSIVE : planifier une maintenance
t=699 min  2.88 mm/s  z =  +3.4  → ANOMALIE ponctuelle
t=702 min  2.88 mm/s  z =  +3.4  → ANOMALIE ponctuelle
t=710 min  2.88 mm/s  z =  +3.4  → ANOMALIE ponctuelle

Modèle appris : 2.27 ± 0.24 mm/s sur 770 mesures normales, 130 anomalie(s) au total
```
