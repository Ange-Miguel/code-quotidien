/*
 * Jour 097 — Simulation d'un distributeur automatique (machine à états)
 *
 * Problème : un distributeur de boissons ou de crédit doit réagir correctement
 * à toutes les séquences d'actions : pièces insérées, choix d'un produit
 * épuisé, annulation, monnaie à rendre… Un code plein de « if » imbriqués
 * devient vite impossible à maintenir et plein de bugs.
 * Solution : une machine à états finis. Les états (ATTENTE, PAIEMENT,
 * DISTRIBUTION, RENDU) et les événements (PIECE, CHOIX, ANNULER, FIN) sont des
 * énumérations ; une seule fonction de transition décide du nouvel état et de
 * l'action. On rejoue un scénario d'événements et on trace chaque transition.
 * C'est exactement ainsi qu'on programme les automates sur microcontrôleur.
 * On apprend : les machines à états finis, les enum et switch imbriqués,
 * la séparation entre état, événements et actions, et le traçage.
 */
#include <stdio.h>
#include <string.h>

typedef enum { ATTENTE, PAIEMENT, DISTRIBUTION, RENDU } Etat;
typedef enum { PIECE, CHOIX, ANNULER, FIN } TypeEvenement;
typedef struct { TypeEvenement type; int valeur; } Evenement;

static const char *NOMS_ETATS[] = {"ATTENTE", "PAIEMENT", "DISTRIBUTION", "RENDU"};

typedef struct { const char *nom; int prix; int stock; } Produit;
static Produit produits[] = {{"Eau 50 cl", 200, 3}, {"Jus d'ananas", 500, 1}, {"Soda", 400, 0}};

typedef struct { Etat etat; int credit; int choix; } Machine;

static int piece_valide(int v) { return v == 50 || v == 100 || v == 200 || v == 500; }

static void transition(Machine *m, Evenement e) {
    Etat avant = m->etat;
    switch (m->etat) {
    case ATTENTE:
    case PAIEMENT:
        if (e.type == PIECE) {
            if (piece_valide(e.valeur)) { m->credit += e.valeur; m->etat = PAIEMENT; printf("   pièce %d acceptée, crédit %d", e.valeur, m->credit); }
            else printf("   pièce %d refusée (rendue)", e.valeur);
        } else if (e.type == CHOIX) {
            Produit *p = &produits[e.valeur];
            if (p->stock == 0) printf("   %s : épuisé", p->nom);
            else if (m->credit < p->prix) printf("   %s : %d FCFA, il manque %d", p->nom, p->prix, p->prix - m->credit);
            else { m->choix = e.valeur; m->etat = DISTRIBUTION; printf("   %s choisi", p->nom); }
        } else if (e.type == ANNULER && m->credit > 0) {
            m->etat = RENDU; printf("   annulation");
        } else printf("   événement ignoré");
        break;
    case DISTRIBUTION:
        if (e.type == FIN) {
            produits[m->choix].stock--;
            m->credit -= produits[m->choix].prix;
            printf("   %s distribué", produits[m->choix].nom);
            m->etat = m->credit > 0 ? RENDU : ATTENTE;
        } else printf("   occupé, événement ignoré");
        break;
    case RENDU:
        if (e.type == FIN) { printf("   monnaie rendue : %d FCFA", m->credit); m->credit = 0; m->etat = ATTENTE; }
        else printf("   occupé, événement ignoré");
        break;
    }
    printf("   [%s → %s]\n", NOMS_ETATS[avant], NOMS_ETATS[m->etat]);
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    Machine m = {ATTENTE, 0, -1};
    Evenement scenario[] = {
        {CHOIX, 0}, {PIECE, 100}, {PIECE, 25}, {CHOIX, 1}, {PIECE, 500}, {CHOIX, 2}, {CHOIX, 1},
        {PIECE, 100}, {FIN, 0}, {FIN, 0}, {PIECE, 200}, {ANNULER, 0}, {FIN, 0}, {PIECE, 200}, {CHOIX, 0}, {FIN, 0}
    };
    const char *noms_evt[] = {"PIECE", "CHOIX", "ANNULER", "FIN"};
    for (size_t i = 0; i < sizeof scenario / sizeof scenario[0]; i++) {
        printf("%2zu. %-7s", i + 1, noms_evt[scenario[i].type]);
        transition(&m, scenario[i]);
    }
    printf("\nStocks restants :");
    for (int i = 0; i < 3; i++) printf(" %s=%d", produits[i].nom, produits[i].stock);
    printf("\n");
    return 0;
}
