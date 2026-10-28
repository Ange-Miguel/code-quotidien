/*
 * Jour 034 — Conversion de chiffres romains ↔ arabes
 *
 * Problème : les chiffres romains apparaissent encore sur les horloges, les
 * chapitres, les noms de rois (Béhanzin régnait au XIXe siècle) et les
 * événements (« IIIe édition »). Il faut savoir les lire et les écrire.
 * Solution : arabe → romain : algorithme glouton avec la table des valeurs
 * incluant les formes soustractives (CM, XC, IV…). Romain → arabe : on lit de
 * gauche à droite ; si un symbole est plus petit que le suivant, on le
 * soustrait. On valide en reconvertissant le résultat (aller-retour).
 * On apprend : les tableaux parallèles, l'algorithme glouton, la validation
 * par aller-retour et la gestion des entrées malformées (« IIII », « VX »).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const int VALEURS[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
static const char *SYMBOLES[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

static void vers_romain(int n, char *sortie) {
    sortie[0] = '\0';
    for (int i = 0; i < 13; i++)
        while (n >= VALEURS[i]) {
            strcat(sortie, SYMBOLES[i]);
            n -= VALEURS[i];
        }
}

static int valeur(char c) {
    switch (toupper((unsigned char)c)) {
        case 'I': return 1;   case 'V': return 5;   case 'X': return 10;
        case 'L': return 50;  case 'C': return 100; case 'D': return 500;
        case 'M': return 1000; default: return 0;
    }
}

/* Retourne la valeur, ou -1 si le nombre romain est mal formé. */
static int vers_arabe(const char *r) {
    int total = 0;
    size_t n = strlen(r);
    if (n == 0 || n > 15) return -1;
    for (size_t i = 0; i < n; i++) {
        int v = valeur(r[i]), suivant = i + 1 < n ? valeur(r[i + 1]) : 0;
        if (!v || (i + 1 < n && !suivant)) return -1;
        total += v < suivant ? -v : v;
    }
    char verification[32];
    if (total < 1 || total > 3999) return -1;
    vers_romain(total, verification);           /* forme canonique ? */
    for (size_t i = 0; i < n; i++)
        if (toupper((unsigned char)r[i]) != verification[i]) return -1;
    return verification[n] == '\0' ? total : -1;
}

static int traiter(const char *texte) {
    char *fin;
    long n = strtol(texte, &fin, 10);
    char r[32];
    if (*fin == '\0') {
        if (n < 1 || n > 3999) { fprintf(stderr, "%s : hors de [1, 3999]\n", texte); return 0; }
        vers_romain((int)n, r);
        printf("%-12s → %s\n", texte, r);
        return 1;
    }
    int a = vers_arabe(texte);
    if (a < 0) { fprintf(stderr, "%s : nombre romain invalide\n", texte); return 0; }
    printf("%-12s → %d\n", texte, a);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        const char *demo[] = {"1960", "2026", "1894", "3999", "XIX", "MCMLX", "mmxxvi", "IIII", "VX"};
        for (int i = 0; i < 9; i++) traiter(demo[i]);
        return 0;
    }
    int ok = 1;
    for (int i = 1; i < argc; i++) ok &= traiter(argv[i]);
    return ok ? 0 : 1;
}
