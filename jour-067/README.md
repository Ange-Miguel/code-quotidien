# Jour 067 — Gestion des notes d'étudiants avec bulletin

**Langue :** C

## Le problème
Produire pour chaque étudiant un bulletin clair : notes de devoirs et d'examen par matière, moyenne pondérée, rang dans la classe, appréciation, et validation des crédits (UE) selon les règles du système LMD (moyenne ≥ 10 dans l'UE).

## La solution
Un tableau à deux dimensions [étudiants][matières] de structures de notes. Moyenne de matière = 40 % devoir + 60 % examen ; moyenne générale pondérée par les crédits ; crédits acquis si moyenne de matière ≥ 10. Le rang est calculé en comparant les moyennes.

## Ce qu'on apprend
Les tableaux 2D de structures, les constantes partagées, l'impression formatée d'un document et le calcul de rang.

## Lancer
```bash
gcc -Wall -Wextra -o bulletin bulletin.c -lm
./bulletin
```

## Exemple de sortie
```
==================== BULLETIN — Semestre 3 ====================
Étudiant : Akpanon Ange         Rang : 2/4

Matière (crédits)         Devoir  Examen  Moyenne Crédits
Électronique analogique(6)   14.00   15.50    14.90   acquis
Programmation C        (5)   17.00   16.00    16.40   acquis
Mathématiques         (5)   12.00   11.00    11.40   acquis
Anglais technique      (2)   15.00   14.00    14.40   acquis

Moyenne générale : 14.29/20 | Crédits : 18/18
Appréciation : Très bon semestre.

==================== BULLETIN — Semestre 3 ====================
Étudiant : Dossou Carine        Rang : 3/4

```
