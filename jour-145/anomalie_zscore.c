/*
 * Jour 145 — Détection d'anomalie sur des mesures (score z)
 *
 * Problème : un moteur de pompe, un compresseur ou un groupe électrogène qui
 * commence à s'abîmer vibre ou chauffe un peu plus que d'habitude, bien avant
 * de casser. Détecter tôt ces écarts permet une maintenance préventive.
 * Solution : on apprend en continu la moyenne et la variance du signal
 * normal avec l'algorithme de Welford (en ligne, une passe, numériquement
 * stable, sans stocker l'historique : idéal sur microcontrôleur). Pour chaque
 * nouvelle mesure, le score z = (x − moyenne) / écart-type indique à combien
 * d'écarts-types elle se trouve. Au-delà de 3, c'est une anomalie, qu'on
 * n'intègre pas dans l'apprentissage pour ne pas « s'habituer » à la panne.
 * On apprend : l'algorithme de Welford, le score z, les statistiques en ligne
 * et la détection de dérive progressive (score moyen sur une fenêtre).
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct { long n; double moyenne, m2; } Welford;

static void apprendre(Welford *w, double x) {
    w->n++;
    double delta = x - w->moyenne;
    w->moyenne += delta / w->n;
    w->m2 += delta * (x - w->moyenne);
}

static double ecart_type(const Welford *w) { return w->n > 1 ? sqrt(w->m2 / (w->n - 1)) : 0; }

/* Vibration simulée (mm/s) : normale, puis pics isolés, puis usure progressive à partir de t = 600. */
static double vibration(int t, unsigned *g) {
    *g = *g * 1664525u + 1013904223u;
    double bruit = ((double)(*g >> 8) / 16777216.0 - 0.5) * 0.6;
    double v = 2.2 + bruit;
    if (t == 250 || t == 420) v += 2.5;                 /* chocs ponctuels */
    if (t >= 600) v += (t - 600) * 0.004;               /* roulement qui s'use */
    return v;
}

int main(int argc, char *argv[]) {
    double seuil = 3.0;
    if (argc == 2) {
        char *fin;
        seuil = strtod(argv[1], &fin);
        if (*fin || seuil < 1 || seuil > 10) { fprintf(stderr, "Usage : %s [seuil du score z, 1..10]\n", argv[0]); return 1; }
    } else if (argc > 2) { fprintf(stderr, "Usage : %s [seuil]\n", argv[0]); return 1; }
    Welford w = {0, 0, 0};
    unsigned graine = 145;
    double fenetre[20] = {0};
    int alertes = 0, derive_signalee = 0;
    printf("Surveillance des vibrations d'une pompe (1 mesure par minute), seuil z = %.1f\n\n", seuil);
    for (int t = 0; t < 900; t++) {
        double x = vibration(t, &graine);
        double s = ecart_type(&w);
        double z = (w.n > 30 && s > 0) ? (x - w.moyenne) / s : 0;
        fenetre[t % 20] = z;
        double z_moyen = 0;
        for (int k = 0; k < 20; k++) z_moyen += fenetre[k] / 20;
        if (fabs(z) > seuil) {
            if (alertes++ < 6) printf("t=%3d min  %.2f mm/s  z = %+5.1f  → ANOMALIE ponctuelle\n", t, x, z);
        } else {
            apprendre(&w, x);                            /* on n'apprend que des mesures normales */
        }
        if (!derive_signalee && z_moyen > 1.5 && t > 50) {
            printf("t=%3d min  z moyen sur 20 min = %+.1f → DÉRIVE PROGRESSIVE : planifier une maintenance\n", t, z_moyen);
            derive_signalee = 1;
        }
    }
    printf("\nModèle appris : %.2f ± %.2f mm/s sur %ld mesures normales, %d anomalie(s) au total\n",
           w.moyenne, ecart_type(&w), w.n, alertes);
    return 0;
}
