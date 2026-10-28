/*
 * Jour 016 — Table de multiplication formatée pour un enfant
 *
 * Problème : aider un petit frère ou une petite sœur à réviser ses tables, avec
 * un affichage clair et un petit exercice d'entraînement.
 * Solution : on affiche la table demandée (ou la grille complète de 1 à 10),
 * puis on génère des questions dont la réponse est donnée à la fin pour
 * l'auto-correction. Un générateur pseudo-aléatoire à graine fixe rend
 * l'exercice reproductible.
 * On apprend : les boucles imbriquées, le formatage en colonnes avec printf et
 * un générateur pseudo-aléatoire simple (congruence linéaire).
 */
#include <stdio.h>
#include <stdlib.h>

static unsigned long graine = 2026;

/* Générateur congruentiel linéaire : simple et reproductible. */
static int hasard(int min, int max) {
    graine = (graine * 1103515245UL + 12345UL) % 2147483648UL;
    return min + (int)(graine % (unsigned long)(max - min + 1));
}

static void table(int n) {
    printf("Table de %d\n", n);
    for (int i = 1; i <= 10; i++) printf("  %2d × %2d = %3d\n", n, i, n * i);
}

static void grille(void) {
    printf("     |");
    for (int j = 1; j <= 10; j++) printf("%4d", j);
    printf("\n-----+");
    for (int j = 1; j <= 10; j++) printf("----");
    printf("\n");
    for (int i = 1; i <= 10; i++) {
        printf(" %3d |", i);
        for (int j = 1; j <= 10; j++) printf("%4d", i * j);
        printf("\n");
    }
}

static void exercice(int n, int questions) {
    int reponses[20];
    printf("\nEntraînement (%d questions) :\n", questions);
    for (int q = 0; q < questions; q++) {
        int b = hasard(1, 10);
        reponses[q] = n * b;
        printf("  %d) %d × %d = ____\n", q + 1, n, b);
    }
    printf("Réponses :");
    for (int q = 0; q < questions; q++) printf(" %d) %d", q + 1, reponses[q]);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        grille();
        exercice(7, 5);
        return 0;
    }
    char *fin;
    long n = strtol(argv[1], &fin, 10);
    if (*fin != '\0' || n < 1 || n > 20) {
        fprintf(stderr, "Usage : %s <table de 1 à 20>\n", argv[0]);
        return 1;
    }
    table((int)n);
    exercice((int)n, 5);
    return 0;
}
