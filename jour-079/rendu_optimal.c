/*
 * Jour 079 — Rendu de monnaie optimal (programmation dynamique)
 *
 * Problème : au jour 002, l'algorithme glouton rendait la monnaie de façon
 * optimale avec les coupures FCFA. Mais avec d'autres systèmes de pièces
 * (jetons de jeu, tickets, anciennes monnaies), le glouton peut se tromper :
 * avec des pièces de 1, 3 et 4, rendre 6 donne 4+1+1 au lieu de 3+3.
 * Solution : programmation dynamique. min[m] = nombre minimal de pièces pour
 * le montant m = 1 + min(min[m − p]) sur toutes les pièces p ≤ m. On garde la
 * dernière pièce utilisée pour reconstruire la solution, et on compte aussi
 * le nombre de façons différentes de rendre la somme.
 * On apprend : la programmation dynamique ascendante, la reconstruction de
 * solution, la comparaison avec le glouton et le comptage de combinaisons.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_MONTANT 100000
#define INFINI 1000000

static int mini[MAX_MONTANT + 1], derniere[MAX_MONTANT + 1];

static int optimal(const int *pieces, int nb, int montant) {
    mini[0] = 0;
    for (int m = 1; m <= montant; m++) {
        mini[m] = INFINI;
        for (int i = 0; i < nb; i++)
            if (pieces[i] <= m && mini[m - pieces[i]] + 1 < mini[m]) {
                mini[m] = mini[m - pieces[i]] + 1;
                derniere[m] = pieces[i];
            }
    }
    return mini[montant];
}

static int glouton(const int *pieces, int nb, int montant) {   /* pièces en ordre décroissant */
    int n = 0;
    for (int i = 0; i < nb; i++) { n += montant / pieces[i]; montant %= pieces[i]; }
    return montant ? INFINI : n;
}

static long long facons(const int *pieces, int nb, int montant) {
    static long long f[MAX_MONTANT + 1];
    for (int m = 0; m <= montant; m++) f[m] = m == 0;
    for (int i = 0; i < nb; i++)
        for (int m = pieces[i]; m <= montant; m++) f[m] += f[m - pieces[i]];
    return f[montant];
}

static void comparer(const char *systeme, const int *pieces, int nb, int montant) {
    int o = optimal(pieces, nb, montant), g = glouton(pieces, nb, montant);
    printf("%s, montant %d :\n", systeme, montant);
    if (o >= INFINI) { printf("   impossible à rendre\n\n"); return; }
    printf("   optimal : %d pièce(s) =", o);
    for (int m = montant; m > 0; m -= derniere[m]) printf(" %d", derniere[m]);
    printf("\n   glouton : %s", g >= INFINI ? "échec" : "");
    if (g < INFINI) printf("%d pièce(s)%s", g, g > o ? "  ← pas optimal !" : "");
    printf("\n   nombre de façons de rendre : %lld\n\n", facons(pieces, nb, montant));
}

int main(int argc, char *argv[]) {
    static const int fcfa[] = {500, 250, 200, 100, 50, 25, 10, 5};
    static const int jetons[] = {4, 3, 1};
    static const int tickets[] = {25, 10, 1};
    static const int bizarre[] = {9, 6};
    if (argc == 2) {
        char *fin;
        long m = strtol(argv[1], &fin, 10);
        if (*fin || m < 1 || m > MAX_MONTANT) {
            fprintf(stderr, "Usage : %s [montant 1..%d] (pièces FCFA)\n", argv[0], MAX_MONTANT);
            return 1;
        }
        comparer("Pièces FCFA", fcfa, 8, (int)m);
        return 0;
    }
    comparer("Pièces FCFA", fcfa, 8, 775);
    comparer("Jetons 1, 3, 4", jetons, 3, 6);
    comparer("Tickets 1, 10, 25", tickets, 3, 30);
    comparer("Pièces 6 et 9", bizarre, 2, 21);
    comparer("Pièces 6 et 9", bizarre, 2, 20);
    return 0;
}
