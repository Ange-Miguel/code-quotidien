/*
 * Jour 043 — Conversion d'unités de longueur, masse et volume
 *
 * Problème : un fournisseur chinois donne des pouces et des livres, une recette
 * étrangère des onces et des tasses, un chantier des pieds. Il faut convertir
 * sans se tromper de catégorie (on ne convertit pas des kg en litres !).
 * Solution : chaque unité est décrite par sa catégorie et son facteur vers
 * l'unité de base (mètre, kilogramme, litre). Convertir = multiplier par le
 * facteur de départ puis diviser par celui d'arrivée. Une table unique
 * remplace des dizaines de formules.
 * On apprend : les tables de données plutôt que du code répété, la recherche
 * dans un tableau de structures, strcmp et la vérification de cohérence.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { LONGUEUR, MASSE, VOLUME } Categorie;
static const char *CATEGORIES[] = {"longueur", "masse", "volume"};

typedef struct {
    const char *symbole;
    const char *nom;
    Categorie categorie;
    double vers_base;   /* combien d'unités de base dans 1 unité */
} Unite;

static const Unite UNITES[] = {
    {"mm", "millimètre", LONGUEUR, 0.001}, {"cm", "centimètre", LONGUEUR, 0.01},
    {"m", "mètre", LONGUEUR, 1.0},         {"km", "kilomètre", LONGUEUR, 1000.0},
    {"in", "pouce", LONGUEUR, 0.0254},     {"ft", "pied", LONGUEUR, 0.3048},
    {"mi", "mile", LONGUEUR, 1609.344},
    {"g", "gramme", MASSE, 0.001},         {"kg", "kilogramme", MASSE, 1.0},
    {"t", "tonne", MASSE, 1000.0},         {"lb", "livre", MASSE, 0.45359237},
    {"oz", "once", MASSE, 0.028349523125},
    {"ml", "millilitre", VOLUME, 0.001},   {"l", "litre", VOLUME, 1.0},
    {"gal", "gallon US", VOLUME, 3.785411784}, {"cup", "tasse US", VOLUME, 0.2365882365},
};
#define NB_UNITES (sizeof UNITES / sizeof UNITES[0])

static const Unite *trouver(const char *symbole) {
    for (size_t i = 0; i < NB_UNITES; i++)
        if (strcmp(UNITES[i].symbole, symbole) == 0) return &UNITES[i];
    return NULL;
}

static int convertir(double valeur, const char *de, const char *vers) {
    const Unite *a = trouver(de), *b = trouver(vers);
    if (!a || !b) {
        fprintf(stderr, "Unité inconnue : %s\n", !a ? de : vers);
        return 0;
    }
    if (a->categorie != b->categorie) {
        fprintf(stderr, "Impossible : %s (%s) → %s (%s)\n", a->nom, CATEGORIES[a->categorie],
                b->nom, CATEGORIES[b->categorie]);
        return 0;
    }
    printf("%10.3f %-4s = %12.4f %s\n", valeur, a->symbole, valeur * a->vers_base / b->vers_base, b->symbole);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        convertir(6.5, "in", "cm");      /* écran de téléphone */
        convertir(5.5, "ft", "m");
        convertir(2.2, "lb", "kg");
        convertir(25, "kg", "lb");       /* sac de riz */
        convertir(1, "gal", "l");
        convertir(2, "cup", "ml");
        convertir(42.195, "km", "mi");   /* marathon */
        convertir(3, "kg", "l");         /* refusé */
        printf("\nUnités disponibles :");
        for (size_t i = 0; i < NB_UNITES; i++) printf(" %s", UNITES[i].symbole);
        printf("\n");
        return 0;
    }
    char *fin;
    double v = argc == 4 ? strtod(argv[1], &fin) : 0;
    if (argc != 4 || *fin) {
        fprintf(stderr, "Usage : %s <valeur> <unité de départ> <unité d'arrivée>\n", argv[0]);
        return 1;
    }
    return convertir(v, argv[2], argv[3]) ? 0 : 1;
}
