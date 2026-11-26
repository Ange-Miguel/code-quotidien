/*
 * Jour 031 — Tri des notes d'une classe et classement
 *
 * Problème : un enseignant ou un délégué veut classer les étudiants selon
 * leur moyenne, avec gestion des ex æquo (deux étudiants à 14,5 partagent le
 * même rang, le suivant prend le rang d'après : 1, 2, 2, 4).
 * Solution : on stocke les étudiants dans un tableau de structures, on trie
 * avec qsort et une fonction de comparaison (moyenne décroissante, puis nom),
 * puis on calcule les rangs en tenant compte des égalités.
 * On apprend : qsort et les pointeurs de fonction, les structures, le tri
 * stable par critères multiples et le classement « olympique ».
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nom[32];
    double moyenne;
} Etudiant;

