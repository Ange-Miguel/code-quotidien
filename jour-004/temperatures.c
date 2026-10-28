/*
 * Jour 004 — Convertisseur de températures Celsius / Fahrenheit / Kelvin
 *
 * Problème : les fiches techniques de capteurs (DHT11, LM35, DS18B20) et les
 * sites étrangers n'utilisent pas tous la même unité de température.
 * Solution : on convertit d'abord la valeur en Celsius (unité pivot), puis vers
 * les deux autres unités. Une température sous le zéro absolu est refusée.
 * On apprend : les énumérations, le switch, les formules de conversion et la
 * détection de valeurs physiquement impossibles.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum { CELSIUS, FAHRENHEIT, KELVIN, INCONNUE } Unite;

static Unite lire_unite(const char *s) {
    switch (toupper((unsigned char)s[0])) {
        case 'C': return CELSIUS;
        case 'F': return FAHRENHEIT;
        case 'K': return KELVIN;
        default:  return INCONNUE;
    }
}

static double vers_celsius(double v, Unite u) {
    switch (u) {
        case FAHRENHEIT: return (v - 32.0) * 5.0 / 9.0;
        case KELVIN:     return v - 273.15;
        default:         return v;
    }
}

static int convertir(double valeur, Unite u) {
    double c = vers_celsius(valeur, u);
    if (c < -273.15) {
        fprintf(stderr, "Erreur : température sous le zéro absolu.\n");
        return 0;
    }
    printf("%8.2f °C | %8.2f °F | %8.2f K\n", c, c * 9.0 / 5.0 + 32.0, c + 273.15);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        puts("Démo : températures courantes");
        convertir(25, CELSIUS);     /* ambiance d'une pièce */
        convertir(37, CELSIUS);     /* corps humain */
        convertir(98.6, FAHRENHEIT);
        convertir(300, KELVIN);
        convertir(-5, KELVIN);      /* impossible : refusée */
        return 0;
    }
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <valeur> <C|F|K>\n", argv[0]);
        return 1;
    }
    char *fin;
    double v = strtod(argv[1], &fin);
    Unite u = lire_unite(argv[2]);
    if (*fin != '\0' || u == INCONNUE) {
        fprintf(stderr, "Entrée invalide. Exemple : %s 30 C\n", argv[0]);
        return 1;
    }
    return convertir(v, u) ? 0 : 1;
}
