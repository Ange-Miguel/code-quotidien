/*
 * Jour 157 — Détection de prix anormaux dans une liste de médicaments
 *
 * Problème : pour un même médicament, les prix relevés dans différentes
 * pharmacies ou facturés par différents services peuvent varier. Certains
 * écarts sont normaux, d'autres révèlent une erreur de saisie (un zéro de
 * trop) ou une surfacturation. Comment les repérer automatiquement sans
 * fixer à la main un seuil pour chaque produit ?
 * Solution : la méthode des quartiles (règle de Tukey) : pour chaque
 * médicament, on calcule Q1, la médiane et Q3 des prix observés ; l'écart
 * interquartile IQR = Q3 − Q1. Un prix est suspect s'il sort de
 * [Q1 − 1,5 IQR ; Q3 + 1,5 IQR]. Cette méthode est robuste : un prix
 * aberrant ne déforme pas les seuils, contrairement à la moyenne. Données
 * fictives.
 * On apprend : les quartiles et l'interpolation, la règle de Tukey, le
 * regroupement de données par clé en C et le tri de nombres réels.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { const char *medicament; const char *source; double prix; } Releve;

static const Releve RELEVES[] = {
    {"Paracétamol 500 mg x16", "Pharmacie A", 600}, {"Paracétamol 500 mg x16", "Pharmacie B", 650},
    {"Paracétamol 500 mg x16", "Pharmacie C", 600}, {"Paracétamol 500 mg x16", "Pharmacie D", 6000},
    {"Paracétamol 500 mg x16", "Clinique E", 700}, {"Paracétamol 500 mg x16", "Pharmacie F", 625},
    {"Amoxicilline 500 mg x12", "Pharmacie A", 1800}, {"Amoxicilline 500 mg x12", "Pharmacie B", 1750},
    {"Amoxicilline 500 mg x12", "Pharmacie C", 1900}, {"Amoxicilline 500 mg x12", "Clinique E", 3200},
    {"Amoxicilline 500 mg x12", "Pharmacie F", 1850}, {"Amoxicilline 500 mg x12", "Pharmacie D", 180},
    {"SRO sachet", "Pharmacie A", 150}, {"SRO sachet", "Pharmacie B", 150}, {"SRO sachet", "Clinique E", 200},
    {"SRO sachet", "Pharmacie C", 175},
};
#define NB (int)(sizeof RELEVES / sizeof RELEVES[0])

static int comparer(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

/* Quantile par interpolation linéaire sur un tableau trié. */
static double quantile(const double *t, int n, double q) {
    double pos = q * (n - 1);
    int i = (int)pos;
    return i + 1 < n ? t[i] + (pos - i) * (t[i + 1] - t[i]) : t[i];
}

static int analyser(const char *medicament) {
    double prix[NB];
    int n = 0, suspects = 0;
    for (int i = 0; i < NB; i++) if (!strcmp(RELEVES[i].medicament, medicament)) prix[n++] = RELEVES[i].prix;
    if (n < 4) { printf("%s : pas assez de relevés (%d)\n\n", medicament, n); return 0; }
    qsort(prix, (size_t)n, sizeof prix[0], comparer);
    double q1 = quantile(prix, n, 0.25), med = quantile(prix, n, 0.5), q3 = quantile(prix, n, 0.75), iqr = q3 - q1;
    double bas = q1 - 1.5 * iqr, haut = q3 + 1.5 * iqr;
    printf("%s : médiane %.0f FCFA, plage normale [%.0f ; %.0f]\n", medicament, med, bas, haut);
    for (int i = 0; i < NB; i++) {
        if (strcmp(RELEVES[i].medicament, medicament)) continue;
        double p = RELEVES[i].prix;
        const char *verdict = p > haut ? "⚠ TROP CHER (erreur ou surfacturation ?)" : p < bas ? "⚠ TROP BAS (erreur de saisie ?)" : "ok";
        printf("   %-12s %7.0f FCFA  %+5.0f %%  %s\n", RELEVES[i].source, p, 100 * (p - med) / med, verdict);
        suspects += p > haut || p < bas;
    }
    printf("\n");
    return suspects;
}

int main(int argc, char *argv[]) {
    if (argc == 2) return analyser(argv[1]) >= 0 ? 0 : 1;
    if (argc > 2) { fprintf(stderr, "Usage : %s [\"nom du médicament\"]\n", argv[0]); return 1; }
    const char *produits[] = {"Paracétamol 500 mg x16", "Amoxicilline 500 mg x12", "SRO sachet"};
    int total = 0;
    for (int i = 0; i < 3; i++) total += analyser(produits[i]);
    printf("%d prix suspect(s) à vérifier.\n", total);
    return 0;
}
