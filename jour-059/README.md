# Jour 059 — Liste chaînée : gestion d'une playlist musicale

**Langue :** Python

## Le problème
Une playlist doit permettre d'ajouter un titre à la fin ou juste après le morceau en cours, de supprimer, de passer au suivant ou au précédent, et de boucler. Insérer au milieu d'un tableau oblige à tout décaler.

## La solution
Une liste doublement chaînée circulaire : chaque nœud connaît son précédent et son suivant. Insérer ou supprimer devient une opération en temps constant une fois le nœud trouvé. On implémente aussi l'itération (pour … in) et le mélange.

## Ce qu'on apprend
Les listes chaînées, les pointeurs (références) prev/next, les méthodes spéciales __iter__ et __len__, et le coût des opérations.

## Lancer
```bash
python3 playlist.py
```

## Exemple de sortie
```
Playlist (4 titres, 14 min 07 s)
   ▶ Morceau A — Artiste 1 (3:34)
     Morceau B — Artiste 2 (3:07)
     Morceau C — Artiste 3 (4:05)
     Morceau D — Artiste 1 (3:21)

Suivant : Morceau B — Artiste 2 (3:07)
Ajout de « Morceau E » juste après le titre en cours
Suivant : Morceau E — Artiste 4 (3:50)
Précédent : Morceau B — Artiste 2 (3:07)
Suppression de « Morceau C » : True

Playlist actuelle (4 titres, 13 min 52 s)
   ▶ Morceau B — Artiste 2 (3:07)
     Morceau E — Artiste 4 (3:50)
```
