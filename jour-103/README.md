# Jour 103 — Buffer circulaire pour données de capteur

**Langue :** C

## Le problème
Un capteur produit des mesures à intervalle régulier (souvent dans une interruption), tandis que la boucle principale les traite ou les envoie quand elle peut. Il faut un tampon de taille fixe, sans malloc, qui ne bloque jamais le producteur.

## La solution
Un buffer circulaire (ring buffer) : un tableau, un indice d'écriture et un indice de lecture qui reviennent à zéro en fin de tableau. Avec une taille puissance de 2, le modulo devient un simple masque (& (TAILLE − 1)), très rapide sur microcontrôleur. Quand il est plein, on écrase la plus ancienne mesure et on compte les pertes.

## Ce qu'on apprend
Le buffer circulaire, l'astuce du masque pour les puissances de 2, les types entiers de taille fixe et la notion producteur / consommateur.

## Lancer
```bash
gcc -Wall -Wextra -o buffer_circulaire buffer_circulaire.c -lm
./buffer_circulaire
```

## Exemple de sortie
```
Le capteur écrit 5 mesures :
   [  269  276  270  277  271    .    .    . ]  rempli 5/8, pertes 0
La boucle principale en lit 3 :
   lu 269 (26.9 °C)
   lu 276 (27.6 °C)
   lu 270 (27.0 °C)
   [    .    .    .  277  271    .    .    . ]  rempli 2/8, pertes 0
Le capteur écrit 10 mesures pendant que la boucle est occupée (débordement) :
   [  273  280  274  281  275  269  276  279 ]  rempli 8/8, pertes 4
La boucle vide le tampon (les plus anciennes ont été écrasées) :
   279 273 280 274 281 275 269 276 
   [    .    .    .    .    .    .    .    . ]  rempli 0/8, pertes 4
Après débordement des compteurs 32 bits : 0 1 2 3 4 (ordre conservé)
```
