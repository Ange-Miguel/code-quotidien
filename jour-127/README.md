# Jour 127 — Minuterie logicielle (software timers) sans blocage

**Langue :** C

## Le problème
Un microcontrôleur n'a que quelques timers matériels, mais un projet domotique a besoin de dizaines de minuteries : éteindre la lumière du couloir après 2 minutes, relancer une connexion après 30 s, couper une pompe au bout de 10 min… sans jamais bloquer le programme avec delay().

## La solution
Un seul compteur de millisecondes (comme millis() d'Arduino) et un tableau de minuteries logicielles : chacune a une échéance, une période (0 = une seule fois) et une fonction de rappel (callback). La fonction timers_traiter() appelée dans la boucle déclenche celles qui sont échues. La comparaison des dates par différence signée reste correcte même quand le compteur 32 bits déborde (après 49,7 jours).

## Ce qu'on apprend
Les pointeurs de fonction (callbacks), les minuteries périodiques et uniques, le débordement des compteurs et la gestion d'un pool statique sans malloc.

## Lancer
```bash
gcc -Wall -Wextra -o software_timers software_timers.c -lm
./software_timers
```

## Exemple de sortie
```
  [4294940295 ms] LED de vie : allumée
  [4294940795 ms] LED de vie : éteinte
  [4294942295 ms] tentative de reconnexion Wi-Fi n°1 → échec
  [4294947295 ms] mouvement détecté : lumière du couloir allumée
  [4294950295 ms] LED de vie : allumée
  [4294950795 ms] LED de vie : éteinte
  [4294960295 ms] LED de vie : allumée
  [4294960795 ms] LED de vie : éteinte
  [       499 ms] LED de vie : éteinte
  [       999 ms] LED de vie : allumée
  [      4999 ms] tentative de reconnexion Wi-Fi n°2 → échec
  [     10499 ms] LED de vie : éteinte
  [     10999 ms] LED de vie : allumée
  [     20499 ms] LED de vie : éteinte
  [     20999 ms] LED de vie : allumée
```
