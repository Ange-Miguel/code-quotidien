/*
 * Jour 040 — Simulateur de lancer de dés et statistiques
 *
 * Problème : au jeu de société, on se demande si « 7 sort vraiment plus
 * souvent » avec deux dés. Simuler des milliers de lancers permet de vérifier
 * la théorie des probabilités par l'expérience.
 * Solution : un générateur pseudo-aléatoire xorshift32 (rapide, reproductible
 * avec une graine) lance deux dés N fois. On compare la fréquence observée de
 * chaque somme (2 à 12) à la probabilité théorique (6 − |s − 7|)/36, avec un
 * histogramme et le test du khi-deux.
 * On apprend : les générateurs pseudo-aléatoires, la loi des grands nombres,
 * les tableaux de compteurs et le test du khi-deux.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static uint32_t etat = 2463534242u;

static uint32_t xorshift32(void) {
    etat ^= etat << 13;
    etat ^= etat >> 17;
    etat ^= etat << 5;
    return etat;
}

/* Dé équilibré : rejet pour éviter le biais du modulo. */
static int de6(void) {
    uint32_t x;
    do { x = xorshift32(); } while (x >= 4294967292u);  /* 4294967292 est multiple de 6 */
    return (int)(x % 6) + 1;
}

static void simuler(long lancers) {
    long compte[13] = {0};
    for (long i = 0; i < lancers; i++) compte[de6() + de6()]++;
    double khi2 = 0;
    printf("%ld lancers de deux dés\n", lancers);
    printf("Somme  Observé   Théorie  Histogramme\n");
    for (int s = 2; s <= 12; s++) {
        double p = (6 - abs(s - 7)) / 36.0, attendu = p * lancers;
        double obs = (double)compte[s] / lancers;
        khi2 += (compte[s] - attendu) * (compte[s] - attendu) / attendu;
        printf("%5d  %6.2f %%  %6.2f %%  ", s, 100 * obs, 100 * p);
        for (int k = 0; k < (int)(obs * 300); k++) putchar('#');
        putchar('\n');
    }
    /* 10 degrés de liberté : seuil à 5 % = 18,31 */
    printf("Khi-deux = %.2f → %s\n\n", khi2,
           khi2 < 18.31 ? "conforme à la théorie (dés équilibrés)" : "écart suspect !");
}

int main(int argc, char *argv[]) {
    long lancers = 0;
    if (argc >= 2) {
        char *fin;
        lancers = strtol(argv[1], &fin, 10);
        if (*fin || lancers < 10 || lancers > 100000000) {
            fprintf(stderr, "Usage : %s [nombre de lancers 10..100000000] [graine]\n", argv[0]);
            return 1;
        }
        if (argc >= 3) etat = (uint32_t)strtoul(argv[2], NULL, 10) | 1u;
        simuler(lancers);
        return 0;
    }
    simuler(100);
    simuler(1000000);
    return 0;
}
