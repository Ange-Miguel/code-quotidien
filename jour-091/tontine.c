/*
 * Jour 091 — Gestionnaire de tontine : cotisations et tour de chaque membre
 *
 * Problème : dans une tontine, chaque membre cotise la même somme à chaque
 * séance et, à tour de rôle, un membre reçoit la cagnotte. Il faut savoir qui
 * a payé, qui est en retard, qui « bouffe » à quelle séance, et la position
 * de chacun (a-t-il déjà reçu plus ou moins qu'il n'a versé ?).
 * Solution : un tableau [membres][séances] enregistre les paiements. L'ordre
 * de passage est fixé par tirage au sort reproductible (graine) avec le
 * mélange de Fisher-Yates. Pour chaque séance, la cagnotte versée est la somme
 * réellement collectée ; les retards sont suivis et pénalisés.
 * On apprend : les tableaux 2D, le mélange de Fisher-Yates, le suivi de soldes
 * (crédit / débit) et la présentation d'un état récapitulatif.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB 6
#define COTISATION 20000L
#define PENALITE 1000L

static const char *MEMBRES[NB] = {"Awa", "Kofi", "Mireille", "Serge", "Inès", "Yves"};

int main(int argc, char *argv[]) {
    unsigned graine = 91;
    if (argc == 2) {
        char *fin;
        unsigned long g = strtoul(argv[1], &fin, 10);
        if (*fin || g == 0) { fprintf(stderr, "Usage : %s [graine du tirage au sort > 0]\n", argv[0]); return 1; }
        graine = (unsigned)g;
    } else if (argc > 2) {
        fprintf(stderr, "Usage : %s [graine]\n", argv[0]);
        return 1;
    }
    srand(graine);
    int ordre[NB];
    for (int i = 0; i < NB; i++) ordre[i] = i;
    for (int i = NB - 1; i > 0; i--) {             /* tirage au sort : Fisher-Yates */
        int j = rand() % (i + 1);
        int t = ordre[i]; ordre[i] = ordre[j]; ordre[j] = t;
    }
    /* paye[m][s] : 1 = à l'heure, 2 = en retard, 0 = impayé */
    int paye[NB][NB];
    for (int m = 0; m < NB; m++)
        for (int s = 0; s < NB; s++) {
            int r = rand() % 100;
            paye[m][s] = r < 82 ? 1 : r < 95 ? 2 : 0;
        }
    long verse[NB] = {0}, recu[NB] = {0}, penalites[NB] = {0};
    printf("Tontine de %d membres, cotisation %ld FCFA par séance\n\n", NB, COTISATION);
    printf("Séance  Bénéficiaire  Collecté   Impayés\n");
    for (int s = 0; s < NB; s++) {
        long collecte = 0;
        char impayes[128] = "";
        for (int m = 0; m < NB; m++) {
            if (paye[m][s]) { collecte += COTISATION; verse[m] += COTISATION; }
            else { snprintf(impayes + strlen(impayes), sizeof impayes - strlen(impayes), "%s ", MEMBRES[m]); }
            if (paye[m][s] == 2) penalites[m] += PENALITE;
        }
        recu[ordre[s]] += collecte;
        printf("%4d    %-12s %9ld   %s\n", s + 1, MEMBRES[ordre[s]], collecte, impayes[0] ? impayes : "—");
    }
    printf("\nMembre      Versé     Reçu     Pénalités  Solde\n");
    for (int m = 0; m < NB; m++) {
        long solde = recu[m] - verse[m] - penalites[m];
        printf("%-9s %8ld %9ld %9ld %+9ld%s\n", MEMBRES[m], verse[m], recu[m], penalites[m], solde,
               verse[m] < COTISATION * NB ? "  ⚠ doit encore cotiser" : "");
    }
    return 0;
}
