/*
 * Jour 160 — Marketplace d'occasion : recherche et filtres d'annonces
 *
 * Problème : sur une plateforme d'annonces d'occasion (téléphones, motos,
 * meubles), l'acheteur veut combiner plusieurs filtres : catégorie, ville,
 * fourchette de prix, état, mots-clés dans le titre, puis trier par prix ou
 * par date. Il faut que ce soit rapide et que les filtres absents soient
 * simplement ignorés.
 * Solution : chaque annonce est une structure ; un critère de recherche est
 * une structure dont les champs vides signifient « peu importe ». Une seule
 * fonction teste si une annonce correspond (recherche de mots-clés insensible
 * à la casse avec strcasestr), puis on trie les résultats avec qsort et une
 * fonction de comparaison choisie à l'exécution. Annonces fictives.
 * On apprend : la recherche multicritère, les filtres optionnels, les
 * pointeurs de fonction pour choisir un tri et la recherche de sous-chaînes.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NEUF, TRES_BON, BON, A_REPARER } Etat;
static const char *ETATS[] = {"neuf", "très bon", "bon", "à réparer"};

typedef struct { int id; const char *titre, *categorie, *ville; long prix; Etat etat; int jours; } Annonce;

static const Annonce ANNONCES[] = {
    {1, "iPhone 11 64 Go", "téléphone", "Cotonou", 145000, TRES_BON, 2}, {2, "Samsung A14 écran fissuré", "téléphone", "Calavi", 38000, A_REPARER, 1},
    {3, "Moto Haojue 125", "moto", "Calavi", 420000, BON, 6},              {4, "Tecno Spark 10 sous garantie", "téléphone", "Calavi", 72000, NEUF, 3},
    {5, "Canapé 3 places", "meuble", "Cotonou", 90000, BON, 12},           {6, "Samsung A34 128 Go", "téléphone", "Porto-Novo", 115000, TRES_BON, 5},
    {7, "Bajaj Boxer 2022", "moto", "Cotonou", 510000, TRES_BON, 1},       {8, "Itel A70", "téléphone", "Calavi", 42000, BON, 8},
    {9, "Bureau en bois", "meuble", "Calavi", 35000, BON, 4},              {10, "Samsung A14", "téléphone", "Cotonou", 60000, TRES_BON, 9},
};
#define NB (int)(sizeof ANNONCES / sizeof ANNONCES[0])

typedef struct { const char *categorie, *ville, *mot_cle; long prix_min, prix_max; int etat_max; } Critere;

static int correspond(const Annonce *a, const Critere *c) {
    if (c->categorie && strcmp(a->categorie, c->categorie)) return 0;
    if (c->ville && strcmp(a->ville, c->ville)) return 0;
    if (c->mot_cle && !strcasestr(a->titre, c->mot_cle)) return 0;
    if (c->prix_min && a->prix < c->prix_min) return 0;
    if (c->prix_max && a->prix > c->prix_max) return 0;
    if ((int)a->etat > c->etat_max) return 0;
    return 1;
}

static int par_prix(const void *x, const void *y) { long a = (*(Annonce *const *)x)->prix, b = (*(Annonce *const *)y)->prix; return (a > b) - (a < b); }
static int par_date(const void *x, const void *y) { return (*(Annonce *const *)x)->jours - (*(Annonce *const *)y)->jours; }

static void rechercher(const char *description, Critere c, int (*tri)(const void *, const void *)) {
    const Annonce *res[NB];
    int n = 0;
    for (int i = 0; i < NB; i++) if (correspond(&ANNONCES[i], &c)) res[n++] = &ANNONCES[i];
    qsort(res, (size_t)n, sizeof res[0], tri);
    printf("%s → %d résultat(s)\n", description, n);
    for (int i = 0; i < n; i++)
        printf("   #%-2d %-30s %-10s %8ld FCFA  %-9s il y a %d j\n", res[i]->id, res[i]->titre, res[i]->ville, res[i]->prix, ETATS[res[i]->etat], res[i]->jours);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2) {             /* recherche libre par mot-clé */
        Critere c = {NULL, NULL, argv[1], 0, 0, A_REPARER};
        rechercher("Recherche libre", c, par_prix);
        return 0;
    }
    if (argc > 2) { fprintf(stderr, "Usage : %s [mot-clé]\n", argv[0]); return 1; }
    rechercher("Téléphones à Calavi, moins de 80 000 FCFA, triés par prix",
               (Critere){"téléphone", "Calavi", NULL, 0, 80000, A_REPARER}, par_prix);
    rechercher("« samsung » en bon état ou mieux, toutes villes, plus récents d'abord",
               (Critere){NULL, NULL, "samsung", 0, 0, BON}, par_date);
    rechercher("Motos entre 300 000 et 600 000 FCFA", (Critere){"moto", NULL, NULL, 300000, 600000, A_REPARER}, par_prix);
    rechercher("Meubles à Porto-Novo", (Critere){"meuble", "Porto-Novo", NULL, 0, 0, A_REPARER}, par_prix);
    return 0;
}
