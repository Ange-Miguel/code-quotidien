/*
 * Jour 064 — Arbre binaire de recherche : annuaire de numéros
 *
 * Problème : un annuaire doit permettre d'ajouter, chercher et supprimer des
 * contacts rapidement, et de les lister dans l'ordre alphabétique, sans trier
 * toute la liste à chaque ajout.
 * Solution : un arbre binaire de recherche (ABR) : pour chaque nœud, les noms
 * plus petits sont à gauche, les plus grands à droite. Recherche et insertion
 * descendent l'arbre ; le parcours infixe (gauche, nœud, droite) donne l'ordre
 * alphabétique. La suppression gère les trois cas (0, 1 ou 2 enfants).
 * On apprend : l'allocation dynamique (malloc/free), les pointeurs de
 * pointeurs, la récursivité sur les arbres et la libération de la mémoire.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct Noeud {
    char nom[40];
    char numero[24];
    struct Noeud *gauche, *droite;
} Noeud;

static int inserer(Noeud **racine, const char *nom, const char *numero) {
    while (*racine) {
        int c = strcasecmp(nom, (*racine)->nom);
        if (c == 0) { snprintf((*racine)->numero, sizeof (*racine)->numero, "%s", numero); return 0; }
        racine = c < 0 ? &(*racine)->gauche : &(*racine)->droite;
    }
    Noeud *n = calloc(1, sizeof *n);
    if (!n) return -1;
    snprintf(n->nom, sizeof n->nom, "%s", nom);
    snprintf(n->numero, sizeof n->numero, "%s", numero);
    *racine = n;
    return 1;
}

static Noeud *chercher(Noeud *r, const char *nom, int *etapes) {
    *etapes = 0;
    while (r) {
        (*etapes)++;
        int c = strcasecmp(nom, r->nom);
        if (c == 0) return r;
        r = c < 0 ? r->gauche : r->droite;
    }
    return NULL;
}

static int supprimer(Noeud **racine, const char *nom) {
    while (*racine && strcasecmp(nom, (*racine)->nom) != 0)
        racine = strcasecmp(nom, (*racine)->nom) < 0 ? &(*racine)->gauche : &(*racine)->droite;
    Noeud *n = *racine;
    if (!n) return 0;
    if (n->gauche && n->droite) {                  /* 2 enfants : remplacer par le successeur */
        Noeud **s = &n->droite;
        while ((*s)->gauche) s = &(*s)->gauche;
        Noeud *succ = *s;
        memcpy(n->nom, succ->nom, sizeof n->nom);
        memcpy(n->numero, succ->numero, sizeof n->numero);
        *s = succ->droite;
        free(succ);
    } else {                                       /* 0 ou 1 enfant */
        *racine = n->gauche ? n->gauche : n->droite;
        free(n);
    }
    return 1;
}

static void lister(const Noeud *r) {
    if (!r) return;
    lister(r->gauche);
    printf("  %-20s %s\n", r->nom, r->numero);
    lister(r->droite);
}

static int hauteur(const Noeud *r) {
    if (!r) return 0;
    int g = hauteur(r->gauche), d = hauteur(r->droite);
    return 1 + (g > d ? g : d);
}

static void liberer(Noeud *r) {
    if (!r) return;
    liberer(r->gauche);
    liberer(r->droite);
    free(r);
}

int main(int argc, char *argv[]) {
    Noeud *annuaire = NULL;
    const char *contacts[][2] = {
        {"Mensah", "+229 01 61 22 33 44"}, {"Adjovi", "+229 01 97 33 44 55"}, {"Zinsou", "+229 01 66 00 11 22"},
        {"Houngbo", "+229 01 95 10 20 30"}, {"Dossou", "+229 01 90 00 00 01"}, {"Tchibozo", "+229 01 51 77 88 99"},
        {"Agossa", "+229 01 64 12 12 12"}, {"Kpade", "+229 01 69 45 45 45"}};
    for (size_t i = 0; i < sizeof contacts / sizeof contacts[0]; i++)
        inserer(&annuaire, contacts[i][0], contacts[i][1]);
    printf("Annuaire (ordre alphabétique, hauteur de l'arbre : %d) :\n", hauteur(annuaire));
    lister(annuaire);

    const char *demo[] = {"houngbo", "Kpade", "Sossou"};
    int nb = argc > 1 ? argc - 1 : 3, introuvable = 0;
    for (int i = 0; i < nb; i++) {
        const char *nom = argc > 1 ? argv[i + 1] : demo[i];
        int etapes;
        Noeud *n = chercher(annuaire, nom, &etapes);
        if (n) printf("Recherche %-10s → %s (%d étape(s))\n", nom, n->numero, etapes);
        else { printf("Recherche %-10s → introuvable (%d étape(s))\n", nom, etapes); introuvable = 1; }
    }
    if (argc == 1) {
        printf("\nSuppression de Mensah (racine, 2 enfants) : %s\n", supprimer(&annuaire, "Mensah") ? "ok" : "absent");
        lister(annuaire);
    }
    liberer(annuaire);
    return argc > 1 && introuvable ? 1 : 0;
}
