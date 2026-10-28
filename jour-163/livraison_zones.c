/*
 * Jour 163 — Calcul des frais de livraison par zone à Cotonou
 *
 * Problème : une boutique en ligne livre à Cotonou et dans les environs. Les
 * frais doivent dépendre de la zone (centre, périphérie, villes voisines), du
 * poids du colis, de l'urgence (livraison express) et être gratuits au-delà
 * d'un certain montant d'achat, pour encourager les grosses commandes.
 * Solution : une grille tarifaire en tableau : chaque zone a un prix de base
 * et un supplément par kilogramme au-delà de 2 kg ; les quartiers sont
 * associés à leur zone par une table de correspondance (recherche insensible
 * à la casse). On ajoute l'express (+50 %, impossible hors zones 1-2), la
 * gratuité conditionnelle et un délai estimé. Grille et quartiers donnés à
 * titre d'exemple.
 * On apprend : les tables de correspondance, la séparation données / calcul,
 * la gestion des cas particuliers et l'arrondi à 100 FCFA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

typedef struct { const char *nom; int base; int par_kg; int delai_h; } Zone;
static const Zone ZONES[] = {
    {"Zone 1 : Cotonou centre", 1000, 200, 4}, {"Zone 2 : Cotonou périphérie", 1500, 250, 6},
    {"Zone 3 : Calavi / Godomey", 2000, 300, 24}, {"Zone 4 : Porto-Novo / Ouidah", 3000, 400, 48},
};
typedef struct { const char *quartier; int zone; } Quartier;
static const Quartier QUARTIERS[] = {
    {"Ganhi", 0}, {"Cadjèhoun", 0}, {"Haie Vive", 0}, {"Gbégamey", 0}, {"Akpakpa", 1}, {"Fidjrossè", 1},
    {"Agla", 1}, {"Vèdoko", 1}, {"Godomey", 2}, {"Calavi Kpota", 2}, {"Zogbadjè", 2}, {"Akassato", 2},
    {"Porto-Novo", 3}, {"Ouidah", 3},
};
#define GRATUIT_DES 50000

static int zone_de(const char *quartier) {
    for (size_t i = 0; i < sizeof QUARTIERS / sizeof QUARTIERS[0]; i++)
        if (!strcasecmp(QUARTIERS[i].quartier, quartier)) return QUARTIERS[i].zone;
    return -1;
}

static int frais(const char *quartier, double kg, long montant, int express) {
    int z = zone_de(quartier);
    if (z < 0) { printf("%-12s : quartier non desservi, contactez-nous\n", quartier); return -1; }
    if (express && z > 1) { printf("%-12s : express indisponible en %s\n", quartier, ZONES[z].nom); return -1; }
    double prix = ZONES[z].base + (kg > 2 ? ceil(kg - 2) * ZONES[z].par_kg : 0);
    if (express) prix *= 1.5;
    int final = (int)(ceil(prix / 100) * 100);
    const char *note = "";
    if (montant >= GRATUIT_DES && !express) { final = 0; note = " (offerte dès 50 000 FCFA)"; }
    printf("%-12s %-27s %4.1f kg %s → %5d FCFA%s, délai ~%d h\n", quartier, ZONES[z].nom, kg, express ? "express " : "standard",
           final, note, express ? ZONES[z].delai_h / 2 : ZONES[z].delai_h);
    return final;
}

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        char *f1, *f2;
        double kg = strtod(argv[2], &f1);
        long montant = argc >= 4 ? strtol(argv[3], &f2, 10) : 0;
        if (*f1 || kg <= 0 || kg > 50 || (argc >= 4 && *f2)) {
            fprintf(stderr, "Usage : %s <quartier> <poids kg> [montant de la commande]\n", argv[0]);
            return 1;
        }
        return frais(argv[1], kg, montant, 0) < 0 ? 1 : 0;
    }
    if (argc == 2) { fprintf(stderr, "Usage : %s <quartier> <poids kg> [montant]\n", argv[0]); return 1; }
    frais("Ganhi", 1.2, 18000, 0);
    frais("Cadjèhoun", 1.2, 18000, 1);
    frais("Akpakpa", 4.5, 32000, 0);
    frais("Calavi Kpota", 7.0, 22000, 0);
    frais("Zogbadjè", 2.0, 64000, 0);
    frais("Godomey", 1.0, 10000, 1);
    frais("Porto-Novo", 3.2, 15000, 0);
    frais("Parakou", 1.0, 10000, 0);
    return 0;
}
