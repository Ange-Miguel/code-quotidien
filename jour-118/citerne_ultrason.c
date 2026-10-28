/*
 * Jour 118 — Simulation d'un capteur ultrason : niveau d'une citerne d'eau
 *
 * Problème : dans beaucoup de maisons, l'eau est stockée dans une citerne ou
 * un tank sur le toit. On ne sait pas combien il en reste avant qu'elle soit
 * vide. Un capteur à ultrasons (HC-SR04, JSN-SR04T étanche) placé sous le
 * couvercle mesure la distance jusqu'à la surface de l'eau.
 * Solution : le capteur renvoie la durée de l'écho (aller-retour) en
 * microsecondes. Distance (cm) = durée × vitesse du son / 2, avec une vitesse
 * du son corrigée par la température (331,3 + 0,606 × T m/s), importante sous
 * le soleil. Niveau = hauteur de la citerne − distance ; on en déduit le
 * pourcentage et le volume restant (cylindre), en filtrant par la médiane de
 * 5 mesures (jour 105) et en rejetant les échos hors limites.
 * On apprend : le principe du sonar, l'influence de la température, le
 * calcul de volume et la robustesse d'une mesure physique.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HAUTEUR_CM 120.0      /* hauteur utile de la citerne */
#define DIAMETRE_CM 110.0
#define PI 3.14159265358979323846
#define ZONE_MORTE_CM 20.0    /* le capteur ne mesure pas en dessous */

static double vitesse_son(double temp_c) { return 331.3 + 0.606 * temp_c; }   /* m/s */

static double distance_cm(double echo_us, double temp_c) {
    return echo_us * 1e-6 * vitesse_son(temp_c) * 100.0 / 2.0;
}

static int comparer(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

static double mediane5(double v[5]) { qsort(v, 5, sizeof v[0], comparer); return v[2]; }

/* Écho simulé pour une hauteur d'eau donnée (quelques mesures parasites). */
static double echo_simule(double eau_cm, double temp_c, int k) {
    double d = HAUTEUR_CM + ZONE_MORTE_CM - eau_cm;
    if (k == 3) return 38000;                         /* écho perdu : délai maximal */
    return (d + ((k * 7) % 5 - 2) * 0.4) * 2.0 / (vitesse_son(temp_c) * 100.0) * 1e6;
}

static void mesurer(double eau_reelle_cm, double temp_c) {
    double d[5];
    int valides = 0;
    for (int k = 0; k < 5; k++) {
        double dist = distance_cm(echo_simule(eau_reelle_cm, temp_c, k), temp_c);
        d[k] = (dist < ZONE_MORTE_CM || dist > HAUTEUR_CM + ZONE_MORTE_CM + 10) ? NAN : dist;
        valides += !isnan(d[k]);
    }
    for (int k = 0; k < 5; k++) if (isnan(d[k])) d[k] = HAUTEUR_CM + ZONE_MORTE_CM;   /* remplacée, la médiane l'ignorera */
    double distance = mediane5(d);
    double niveau = HAUTEUR_CM + ZONE_MORTE_CM - distance;
    if (niveau < 0) niveau = 0;
    double rayon_dm = DIAMETRE_CM / 20.0;
    double litres = PI * rayon_dm * rayon_dm * niveau / 10.0;
    double pct = 100.0 * niveau / HAUTEUR_CM;
    printf("%5.1f °C | distance %6.1f cm | eau %5.1f cm (%3.0f %%) | %5.0f L | %d/5 mesures valides  ",
           temp_c, distance, niveau, pct, litres, valides);
    for (int i = 0; i < 20; i++) putchar(i < pct / 5 ? '#' : '.');
    printf("%s\n", pct < 20 ? "  ⚠ niveau bas" : "");
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        char *f1, *f2;
        double echo = strtod(argv[1], &f1), t = strtod(argv[2], &f2);
        if (*f1 || *f2 || echo <= 0 || t < -20 || t > 70) {
            fprintf(stderr, "Usage : %s [durée d'écho µs] [température °C]\n", argv[0]);
            return 1;
        }
        printf("Écho %.0f µs à %.1f °C → %.1f cm\n", echo, t, distance_cm(echo, t));
        return 0;
    }
    if (argc != 1) { fprintf(stderr, "Usage : %s [écho µs] [température]\n", argv[0]); return 1; }
    double rayon_dm = DIAMETRE_CM / 20.0;
    printf("Citerne : %.0f cm de haut, Ø %.0f cm, capacité %.0f L\n\n", HAUTEUR_CM, DIAMETRE_CM,
           PI * rayon_dm * rayon_dm * HAUTEUR_CM / 10.0);
    double niveaux[] = {115, 84, 51, 18};
    for (int i = 0; i < 4; i++) mesurer(niveaux[i], 30);
    printf("\nMême niveau (84 cm) mais température du capteur non corrigée (20 °C supposé au lieu de 45 °C) :\n");
    double e = echo_simule(84, 45, 0);
    printf("   corrigé : %.1f cm | non corrigé : %.1f cm → erreur de %.1f cm\n",
           distance_cm(e, 45), distance_cm(e, 20), distance_cm(e, 45) - distance_cm(e, 20));
    return 0;
}
