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

static int comparer(const void *a, const void *b) {
    const Etudiant *x = a, *y = b;
    if (x->moyenne > y->moyenne) return -1;
    if (x->moyenne < y->moyenne) return 1;
    return strcmp(x->nom, y->nom);   /* égalité : ordre alphabétique */
}

static void classer(Etudiant *classe, int n) {
    qsort(classe, (size_t)n, sizeof *classe, comparer);
    double somme = 0;
    int admis = 0, rang = 1;
    printf("%4s  %-20s %7s  %s\n", "Rang", "Nom", "Moyenne", "Décision");
    for (int i = 0; i < n; i++) {
        if (i > 0 && classe[i].moyenne != classe[i - 1].moyenne) rang = i + 1;
        int ex_aequo = (i > 0 && classe[i].moyenne == classe[i - 1].moyenne) ||
                       (i + 1 < n && classe[i].moyenne == classe[i + 1].moyenne);
        printf("%3d%s  %-20s %7.2f  %s\n", rang, ex_aequo ? "e" : " ", classe[i].nom,
               classe[i].moyenne, classe[i].moyenne >= 10 ? "admis" : "ajourné");
        somme += classe[i].moyenne;
        admis += classe[i].moyenne >= 10;
    }
    printf("\nMoyenne de la classe : %.2f | admis : %d/%d (%.0f %%)\n",
           somme / n, admis, n, 100.0 * admis / n);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        Etudiant classe[] = {
            {"Adjovi Mireille", 14.5}, {"Houngbo Serge", 9.75}, {"Akpanon Ange", 15.2},
            {"Dossou Carine", 14.5}, {"Kpade Yves", 11.0}, {"Zinsou Paul", 8.5},
            {"Agossa Inès", 12.25}
        };
        classer(classe, (int)(sizeof classe / sizeof classe[0]));
        return 0;
    }
    /* Arguments : Nom=moyenne ... */
    int n = argc - 1;
    Etudiant *classe = calloc((size_t)n, sizeof *classe);
    if (!classe) return 2;
    for (int i = 0; i < n; i++) {
        char *egal = strchr(argv[i + 1], '=');
        char *fin;
        if (!egal || egal == argv[i + 1]) goto erreur;
        double m = strtod(egal + 1, &fin);
        if (*fin || m < 0 || m > 20) goto erreur;
        snprintf(classe[i].nom, sizeof classe[i].nom, "%.*s", (int)(egal - argv[i + 1]), argv[i + 1]);
        classe[i].moyenne = m;
    }
    classer(classe, n);
    free(classe);
    return 0;
erreur:
    fprintf(stderr, "Usage : %s Nom=moyenne [Nom=moyenne ...] (moyennes de 0 à 20)\n", argv[0]);
    free(classe);
    return 1;
}
