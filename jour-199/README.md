# Jour 199 — Mini-base de données clé-valeur persistante

**Langue :** C

## Le problème
Beaucoup d'applications (configuration d'un appareil, compteur, panier d'un utilisateur) ont juste besoin d'enregistrer des paires clé → valeur et de les retrouver après un redémarrage, sans installer une vraie base de données. Une coupure de courant pendant l'écriture ne doit pas tout corrompre.

## La solution
Un journal en ajout seul (append-only log) : chaque écriture (« SET clé valeur » ou « DEL clé ») est ajoutée à la fin du fichier avec une somme de contrôle, puis fflush/fsync. Au démarrage, on rejoue le journal pour reconstruire la table en mémoire ; une ligne finale abîmée (coupure) est ignorée. Comme le fichier grossit, la compaction réécrit uniquement l'état actuel dans un nouveau fichier, puis le renomme de façon atomique. C'est le principe de Redis (AOF) ou de nombreux moteurs de stockage.

## Ce qu'on apprend
La persistance par journal, la reprise après panne, les sommes de contrôle, rename atomique et la compaction.

## Lancer
```bash
gcc -Wall -Wextra -o base_cle_valeur base_cle_valeur.c -lm
./base_cle_valeur
```

## Exemple de sortie
```
Écritures :
   GET compteur     → 500
   GET mode         → (absent)
   journal : 1397 octets

Coupure de courant pendant une écriture (ligne finale tronquée) :
   (1 ligne(s) abîmée(s) ignorée(s) au chargement)
Redémarrage : 54 opérations rejouées
   GET wifi_ssid    → Maison-Calavi
   GET seuil_temp   → 30
   GET compteur     → 500

Compaction : 1413 → 90 octets, puis relecture : 3 entrées
   GET compteur     → 500
```
