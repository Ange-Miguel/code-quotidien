/*
 * Jour 121 — Surveillance de température d'une chambre froide avec alarmes
 *
 * Problème : une pharmacie (vaccins entre 2 et 8 °C), une poissonnerie ou un
 * vendeur de glaces perd tout son stock si la chambre froide dérive pendant
 * une coupure ou une panne de compresseur. Une simple alarme instantanée
 * sonne trop souvent (porte ouverte 30 s) ; il faut une logique intelligente.
 * Solution : une alarme à plusieurs niveaux : AVERTISSEMENT si la température
 * sort de la plage pendant plus de 5 minutes consécutives, CRITIQUE au-delà
 * de 20 minutes ou si l'écart dépasse 4 °C. On calcule aussi le temps total
 * hors plage et le « degré-minute » (intégrale de l'écart), utilisé pour
 * juger si les produits sont encore bons. Une journée est simulée avec une
 * ouverture de porte et une coupure de courant.
 * On apprend : les alarmes temporisées, les niveaux de gravité, le calcul
 * d'intégrale discrète et la séparation entre mesure et décision.
 */
#include <stdio.h>
#include <stdlib.h>

#define T_MIN 2.0
#define T_MAX 8.0
#define DELAI_AVERT 5
#define DELAI_CRIT 20

typedef enum { NORMAL, AVERTISSEMENT, CRITIQUE } Niveau;
static const char *NIVEAUX[] = {"normal", "AVERTISSEMENT", "CRITIQUE"};

typedef struct { Niveau niveau; int minutes_hors; int total_hors; double degre_minutes; double max; } Surveillance;

static Niveau evaluer(Surveillance *s, double t) {
    double ecart = t > T_MAX ? t - T_MAX : t < T_MIN ? T_MIN - t : 0;
    if (ecart > 0) { s->minutes_hors++; s->total_hors++; s->degre_minutes += ecart; }
    else s->minutes_hors = 0;
    if (t > s->max) s->max = t;
    if (ecart > 4 || s->minutes_hors >= DELAI_CRIT) return CRITIQUE;
    if (s->minutes_hors >= DELAI_AVERT) return AVERTISSEMENT;
    return NORMAL;
}

/* Température simulée minute par minute. */
static double simuler(int minute, double t, unsigned *graine) {
    int porte = minute >= 600 && minute < 608;           /* porte ouverte 8 min à 10 h */
    int coupure = minute >= 840 && minute < 900;         /* coupure de courant 14 h - 15 h */
    *graine = *graine * 1103515245u + 12345u;
    double bruit = ((int)((*graine >> 16) % 100) - 50) / 500.0;
    if (porte) return t + 0.9 + bruit;
    if (coupure) return t + 0.18 + bruit;
    return t + (4.5 - t) * 0.15 + bruit;                /* le compresseur ramène vers 4,5 °C */
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    Surveillance s = {NORMAL, 0, 0, 0, -100};
    unsigned graine = 121;
    double t = 4.5;
    printf("Chambre froide pour vaccins (plage %.0f-%.0f °C), journée simulée minute par minute\n\n", T_MIN, T_MAX);
    for (int m = 0; m < 24 * 60; m++) {
        t = simuler(m, t, &graine);
        Niveau n = evaluer(&s, t);
        if (n != s.niveau) {
            printf("%02dh%02d  %5.1f °C  %s → %s%s\n", m / 60, m % 60, t, NIVEAUX[s.niveau], NIVEAUX[n],
                   n == CRITIQUE ? "  (SMS au pharmacien + sirène)" : n == AVERTISSEMENT ? "  (notification)" : "");
            s.niveau = n;
        }
    }
    printf("\nBilan : %d min hors plage, maximum %.1f °C, exposition %.0f degré-minutes\n", s.total_hors, s.max, s.degre_minutes);
    printf("Verdict : %s\n", s.degre_minutes > 300 ? "produits à faire contrôler avant usage" : "produits a priori conformes");
    return 0;
}
