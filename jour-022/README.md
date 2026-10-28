# Jour 022 — Validateur de numéro de téléphone béninois (+229 01 ...)

**Langue :** C

## Le problème
Dans un formulaire (livraison, inscription), les clients écrivent leur numéro de mille façons : « 0166546144 », « +229 01 66 54 61 44 », « 00229-01.66.54.61.44 ». Il faut vérifier et normaliser.

## La solution
On retire espaces, points, tirets et parenthèses ; on accepte les préfixes +229 ou 00229 ; le numéro national doit alors faire 10 chiffres et commencer par 01 (format en vigueur depuis la migration de 2024). On affiche le numéro au format international lisible.

## Ce qu'on apprend
Le parcours de chaînes, isdigit, strncmp, la normalisation de données saisies par des humains et les codes de retour d'erreur.

## Lancer
```bash
gcc -Wall -Wextra -o telephone telephone.c -lm
./telephone
```

## Exemple de sortie
```
+229 01 66 54 61 44        → +229 01 66 54 61 44
0166546144                 → +229 01 66 54 61 44
00229-01.97.00.00.00       → +229 01 97 00 00 00
97 00 00 00                → invalide : longueur incorrecte (10 chiffres attendus)
+229 01 66 54 61 4A        → invalide : caractère non autorisé
(229) 0161000000           → +229 01 61 00 00 00
```
