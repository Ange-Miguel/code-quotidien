/*
 * Jour 151 — Disponibilité des bouteilles de gaz par quartier (recherche du plus proche)
 *
 * Problème : quand la bouteille de gaz est vide, on fait parfois le tour de
 * plusieurs dépôts avant d'en trouver un qui a encore des bouteilles de la
 * bonne taille. Une application qui indique le point de vente le plus proche
 * ayant du stock ferait gagner du temps et du carburant.
 * Solution : chaque point de vente a une position GPS, un stock par taille
 * (6 kg, 12,5 kg) et l'heure de la dernière mise à jour. Pour une demande
 * (position du client, taille voulue), on calcule la distance de Haversine
 * vers chaque point, on écarte ceux sans stock ou dont l'information est
 * trop ancienne (plus de 3 h), et on trie les candidats par distance.
 * Positions et stocks sont fictifs, pour l'exemple.
 * On apprend : la combinaison filtre + tri, le calcul de distance GPS en C,
 * la fraîcheur des données et le tri d'indices avec qsort.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846
#define MAX_AGE_MIN 180

typedef struct { const char *nom; double lat, lon; int stock6, stock12; int maj_min; } Depot;

static const Depot DEPOTS[] = {
    {"Dépôt Kpota", 6.4527, 2.3496, 0, 14, 30},      {"Dépôt Zogbadjè", 6.4170, 2.3427, 8, 0, 45},
    {"Station Godomey", 6.3912, 2.3301, 12, 20, 400}, {"Boutique Akassato", 6.5041, 2.3613, 3, 6, 15},
    {"Dépôt Tankpè", 6.4351, 2.3712, 0, 2, 90},       {"Dépôt Calavi Centre", 6.4485, 2.3557, 5, 0, 20},
};
#define NB (int)(sizeof DEPOTS / sizeof DEPOTS[0])

static double haversine_km(double la1, double lo1, double la2, double lo2) {
    double p1 = la1 * PI / 180, p2 = la2 * PI / 180, dp = p2 - p1, dl = (lo2 - lo1) * PI / 180;
    double h = sin(dp / 2) * sin(dp / 2) + cos(p1) * cos(p2) * sin(dl / 2) * sin(dl / 2);
    return 2 * 6371 * asin(sqrt(h));
}

static double distances[NB];
static int par_distance(const void *a, const void *b) {
    double x = distances[*(const int *)a], y = distances[*(const int *)b];
    return (x > y) - (x < y);
}

static void chercher(const char *client, double lat, double lon, int taille) {
    int candidats[NB], n = 0;
    printf("Client à %s, bouteille de %s kg :\n", client, taille == 6 ? "6" : "12,5");
    for (int i = 0; i < NB; i++) {
        int stock = taille == 6 ? DEPOTS[i].stock6 : DEPOTS[i].stock12;
        distances[i] = haversine_km(lat, lon, DEPOTS[i].lat, DEPOTS[i].lon);
        if (stock > 0 && DEPOTS[i].maj_min <= MAX_AGE_MIN) candidats[n++] = i;
        else if (stock > 0) printf("   (ignoré : %s, information vieille de %d h)\n", DEPOTS[i].nom, DEPOTS[i].maj_min / 60);
    }
    qsort(candidats, (size_t)n, sizeof candidats[0], par_distance);
    if (n == 0) { printf("   Aucun dépôt avec du stock confirmé. Réessayez plus tard.\n\n"); return; }
    for (int k = 0; k < n && k < 3; k++) {
        const Depot *d = &DEPOTS[candidats[k]];
        printf("   %d. %-20s %5.2f km  stock %2d  (mis à jour il y a %d min)%s\n", k + 1, d->nom, distances[candidats[k]],
               taille == 6 ? d->stock6 : d->stock12, d->maj_min, k == 0 ? "  ← conseillé" : "");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc == 4) {
        char *f1, *f2, *f3;
        double lat = strtod(argv[1], &f1), lon = strtod(argv[2], &f2);
        long t = strtol(argv[3], &f3, 10);
        if (*f1 || *f2 || *f3 || fabs(lat) > 90 || fabs(lon) > 180 || (t != 6 && t != 12)) {
            fprintf(stderr, "Usage : %s <latitude> <longitude> <6|12>\n", argv[0]);
            return 1;
        }
        chercher("la position donnée", lat, lon, (int)t);
        return 0;
    }
    if (argc != 1) { fprintf(stderr, "Usage : %s [latitude longitude 6|12]\n", argv[0]); return 1; }
    chercher("Zogbadjè (campus)", 6.4180, 2.3410, 12);
    chercher("Akassato", 6.4990, 2.3590, 6);
    chercher("Godomey", 6.3920, 2.3320, 12);
    return 0;
}
