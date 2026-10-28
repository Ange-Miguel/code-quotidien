# Jour 037 — PGCD et PPCM (partage équitable de lots)

**Langue :** C

## Le problème
Un grossiste a 84 savons et 120 bouteilles d'huile ; il veut faire le plus grand nombre de lots identiques sans rien laisser. Deux bus partent toutes les 12 et 18 minutes : quand repartent-ils ensemble ?

## La solution
Le premier problème est un PGCD (algorithme d'Euclide : pgcd(a, b) = pgcd(b, a mod b)), le second un PPCM (a × b / pgcd). On généralise à plusieurs nombres et on affiche les étapes d'Euclide.

## Ce qu'on apprend
L'algorithme d'Euclide (récursif et itératif), le lien entre PGCD et PPCM, et le risque de dépassement de capacité des entiers.

## Lancer
```bash
gcc -Wall -Wextra -o pgcd_ppcm pgcd_ppcm.c -lm
./pgcd_ppcm
```

## Exemple de sortie
```
Lots identiques sans reste :
  Euclide : pgcd(84, 120) → pgcd(120, 84) → pgcd(84, 36) → pgcd(36, 12) → 12
  84 savons et 120 bouteilles d'huile → 12 lots de 7 savons + 10 bouteilles d'huile

  Euclide : pgcd(252, 198) → pgcd(198, 54) → pgcd(54, 36) → pgcd(36, 18) → 18
  252 cahiers et 198 stylos → 18 lots de 14 cahiers + 11 stylos

Départs simultanés de bus (toutes les 12, 18 et 30 min) :
  ils repartent ensemble toutes les 180 min (3 h 00)
```
