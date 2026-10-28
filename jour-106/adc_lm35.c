/*
 * Jour 106 — Conversion ADC : valeur brute 10 bits → tension → température (LM35)
 *
 * Problème : un Arduino lit un capteur de température LM35 sur une entrée
 * analogique et obtient un nombre entre 0 et 1023, pas des degrés. Il faut
 * convertir correctement, sans nombres à virgule si possible (certains petits
 * microcontrôleurs n'ont pas d'unité de calcul flottant).
 * Solution : tension (mV) = brut × Vref (mV) / 1023 ; le LM35 donne 10 mV par
 * °C, donc T (°C) = tension / 10. On compare un calcul en virgule flottante et
 * un calcul en entiers en dixièmes de degré (arrondi correct), on étudie la
 * résolution (pas d'un degré par point avec Vref = 5 V, bien meilleur avec la
 * référence interne de 1,1 V) et on suréchantillonne pour gagner en précision.
 * On apprend : le fonctionnement d'un convertisseur analogique-numérique, la
 * résolution, l'arithmétique en virgule fixe et l'arrondi entier.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ADC_MAX 1023u

/* Température en dixièmes de °C, calcul entier avec arrondi. */
static int32_t lm35_dixiemes(uint16_t brut, uint32_t vref_mv) {
    /* T(0,1 °C) = brut × vref / 1023 (en mV) ; 1 mV = 0,1 °C pour le LM35 */
    return (int32_t)(((uint32_t)brut * vref_mv + ADC_MAX / 2) / ADC_MAX);
}

static double lm35_flottant(uint16_t brut, double vref_mv) { return brut * vref_mv / ADC_MAX / 10.0; }

/* Lecture ADC simulée : quantification d'une tension réelle (+ petit bruit déterministe). */
static uint16_t adc_lire(double tension_mv, double vref_mv, int bruit) {
    double v = tension_mv / vref_mv * ADC_MAX + bruit * 0.3;
    if (v < 0) v = 0;
    if (v > ADC_MAX) v = ADC_MAX;
    return (uint16_t)(v + 0.5);
}

static void tableau(double vref_mv) {
    printf("Vref = %.0f mV : résolution = %.2f mV = %.2f °C par point\n", vref_mv, vref_mv / ADC_MAX, vref_mv / ADC_MAX / 10);
    printf("  T réelle  brut  T flottante  T entière\n");
    double temps[] = {18.0, 25.3, 30.0, 37.6, 52.0};
    for (int i = 0; i < 5; i++) {
        uint16_t b = adc_lire(temps[i] * 10, vref_mv, 0);
        int32_t d = lm35_dixiemes(b, (uint32_t)vref_mv);
        printf("  %6.1f   %5u  %9.2f   %5d,%d\n", temps[i], b, lm35_flottant(b, vref_mv), d / 10, d % 10);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *fin;
        long b = strtol(argv[1], &fin, 10);
        if (*fin || b < 0 || b > (long)ADC_MAX) { fprintf(stderr, "Usage : %s [valeur ADC 0..1023]\n", argv[0]); return 1; }
        int32_t d = lm35_dixiemes((uint16_t)b, 5000);
        printf("ADC %ld → %ld mV → %d,%d °C (Vref 5 V)\n", b, (b * 5000 + 511) / 1023, d / 10, d % 10);
        return 0;
    }
    if (argc > 2) { fprintf(stderr, "Usage : %s [valeur ADC]\n", argv[0]); return 1; }
    tableau(5000);
    putchar('\n');
    tableau(1100);
    /* Suréchantillonnage : 16 lectures bruitées sommées → 2 bits de résolution en plus */
    uint32_t somme = 0;
    for (int k = 0; k < 16; k++) somme += adc_lire(253, 5000, (k % 5) - 2);
    printf("\nSuréchantillonnage ×16 à Vref 5 V pour 25,3 °C : moyenne %.2f points → %.2f °C\n",
           somme / 16.0, somme / 16.0 * 5000 / ADC_MAX / 10);
    return 0;
}
