/*
 * Jour 001 — Convertisseur FCFA <-> euro <-> dollar
 *
 * Problème : au marché ou en commandant en ligne, on doit souvent convertir
 * un prix d'une monnaie à l'autre. Le FCFA (XOF) a une parité FIXE avec
 * l'euro : 1 EUR = 655,957 XOF. Le dollar, lui, varie : on utilise un taux
 * modifiable en argument.
 *
 * Compilation : gcc -Wall -Wextra -o convertisseur convertisseur.c
 * Utilisation : ./convertisseur <montant> <XOF|EUR|USD> [taux EUR->USD]
 * Exemple     : ./convertisseur 10000 XOF
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define XOF_PAR_EUR 655.957        /* parité fixe garantie */
#define EUR_USD_DEFAUT 1.08        /* taux indicatif, à ajuster */

/* Met une chaîne en majuscules pour accepter "xof", "Eur", etc. */
static void majuscules(char *s) {
    for (; *s; s++) *s = (char)toupper((unsigned char)*s);
}

/* Convertit n'importe quel montant en euros (monnaie pivot). */
static int vers_euro(double montant, const char *devise, double eur_usd, double *euros) {
    if (strcmp(devise, "EUR") == 0) *euros = montant;
    else if (strcmp(devise, "XOF") == 0) *euros = montant / XOF_PAR_EUR;
    else if (strcmp(devise, "USD") == 0) *euros = montant / eur_usd;
    else return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage : %s <montant> <XOF|EUR|USD> [taux EUR->USD]\n", argv[0]);
        return 1;
    }

    char *fin;
    double montant = strtod(argv[1], &fin);
    if (*fin != '\0' || montant < 0) {
        fprintf(stderr, "Montant invalide : %s\n", argv[1]);
        return 1;
    }

    char devise[8];
    strncpy(devise, argv[2], sizeof devise - 1);
    devise[sizeof devise - 1] = '\0';
    majuscules(devise);

    double eur_usd = EUR_USD_DEFAUT;
    if (argc >= 4) {
        eur_usd = strtod(argv[3], &fin);
        if (*fin != '\0' || eur_usd <= 0) {
            fprintf(stderr, "Taux invalide : %s\n", argv[3]);
            return 1;
        }
    }

    double euros;
    if (!vers_euro(montant, devise, eur_usd, &euros)) {
        fprintf(stderr, "Devise inconnue : %s (XOF, EUR ou USD)\n", devise);
        return 1;
    }

    printf("%.2f %s vaut :\n", montant, devise);
    printf("  %12.0f FCFA (XOF)\n", euros * XOF_PAR_EUR);
    printf("  %12.2f EUR\n", euros);
    printf("  %12.2f USD (taux 1 EUR = %.4f USD)\n", euros * eur_usd, eur_usd);
    return 0;
}
