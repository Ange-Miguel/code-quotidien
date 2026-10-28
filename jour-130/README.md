# Jour 130 — Journal circulaire de données en mémoire limitée

**Langue :** C

## Le problème
Un enregistreur autonome (station météo, traceur de température de vaccins) ne dispose que de quelques kilo-octets de mémoire, mais doit garder un historique utile : chaque minute de la dernière heure, puis des moyennes horaires sur plusieurs jours.

## La solution
Deux journaux circulaires à résolutions différentes. Les mesures brutes entrent dans un anneau de 60 cases ; toutes les 60 mesures, on en calcule le minimum, la moyenne et le maximum qu'on stocke dans un second anneau de 72 cases (3 jours). Les valeurs sont compactées en entiers 16 bits (dixièmes de degré) pour diviser la place par deux. On mesure la mémoire utilisée et on affiche l'historique.

## Ce qu'on apprend
Le sous-échantillonnage (agrégation), la compaction des données en types entiers de taille fixe, sizeof et l'économie de mémoire.

## Lancer
```bash
gcc -Wall -Wextra -o journal_circulaire journal_circulaire.c -lm
./journal_circulaire
```

## Exemple de sortie
```
7200 mesures reçues (5 jours). Mémoire du journal : 572 octets
Stocker toutes les mesures en double aurait demandé 57600 octets.

Dernières 10 minutes (résolution 1 min) :
   23.7 23.9 23.5 23.7 23.3 23.5 23.7 23.3 23.5 23.7 

Dernières 24 heures (min / moyenne / max par heure) :
   h-24  22.4  23.0  23.7  ======
   h-23  22.0  22.3  22.8  ====
   h-22  21.8  22.0  22.4  ====
   h-21  21.8  22.0  22.4  ====
   h-20  22.0  22.3  22.9  ====
   h-19  22.4  23.0  23.6  ======
   h-18  23.3  23.9  24.7  =======
   h-17  24.3  25.0  25.8  ==========
```
