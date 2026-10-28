# Jour 109 — Anti-rebond (debounce) logiciel d'un bouton

**Langue :** C

## Le problème
Quand on appuie sur un bouton mécanique, le contact « rebondit » pendant quelques millisecondes : le microcontrôleur voit une rafale 0-1-0-1-0-1 au lieu d'un seul appui. Un compteur d'appuis compte alors 5 ou 10 appuis pour un seul.

## La solution
On échantillonne l'entrée toutes les 1 ms et on ne valide un changement d'état que s'il reste stable pendant N échantillons consécutifs (compteur de stabilité). Variante très compacte : un registre à décalage de 8 bits où l'état n'est accepté que quand les 8 derniers échantillons sont identiques (0x00 ou 0xFF). On compare avec la méthode naïve sur un signal simulé avec rebonds.

## Ce qu'on apprend
Le phénomène de rebond, l'échantillonnage périodique, les compteurs de stabilité, le registre à décalage et la détection de front.

## Lancer
```bash
gcc -Wall -Wextra -o debounce debounce.c -lm
./debounce
```

## Exemple de sortie
```
t(ms)  brut  compteur  registre
 198    0       0         0
 199    0       0         0
 200    1       0         0
 201    0       0         0
 202    0       0         0
 203    0       0         0
 204    1       0         0
 205    1       0         0
 206    1       0         0
 207    1       0         0
 208    1       1         0
 209    1       1         0
 210    1       1         0
 211    1       1         1
```
