# Jour 129 — Simulation d'une EEPROM avec usure (wear leveling)

**Langue :** Java

## Le problème
Un compteur d'eau ou d'énergie enregistre son index en mémoire non volatile (EEPROM ou Flash) pour ne pas le perdre en cas de coupure. Mais chaque case ne supporte qu'environ 100 000 écritures : en écrivant toujours à la même adresse toutes les minutes, la mémoire meurt en 70 jours !

## La solution
La répartition d'usure (wear leveling). Au lieu d'une seule case, on utilise un anneau de N emplacements et on écrit chaque nouvelle valeur dans l'emplacement suivant, avec un numéro de séquence. Au démarrage, on retrouve la valeur la plus récente en cherchant le plus grand numéro de séquence valide (avec somme de contrôle). On simule l'usure, une coupure pendant une écriture, et on compare la durée de vie.

## Ce qu'on apprend
Les limites physiques des mémoires, le stockage circulaire, les numéros de séquence, la reprise après coupure et l'estimation de durée de vie.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Après redémarrage, valeur relue : 10 000 L (la dernière écriture, interrompue, est ignorée)
Nouvelle écriture correcte, valeur relue : 10 010 L

Usure après 1 002 écritures sur 64 emplacements : de 15 à 17 écritures par case

Durée de vie avec une sauvegarde par minute (100 000 cycles par case) :
      1 emplacement(s) :       69 jours (0,2 ans)
      8 emplacement(s) :      556 jours (1,5 ans)
     64 emplacement(s) :     4444 jours (12,2 ans)
    512 emplacement(s) :    35556 jours (97,4 ans)
```
