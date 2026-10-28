/*
 * Jour 052 — Gestion de stock d'une boutique (ajout, vente, alerte rupture)
 *
 * Problème : une boutique perd des ventes quand un produit populaire est en
 * rupture, et immobilise de l'argent dans des produits qui ne partent pas.
 * Solution : chaque produit a un stock, un seuil d'alerte et un prix. Les
 * opérations (réception, vente) sont des fonctions qui vérifient les règles
 * (pas de vente au-delà du stock). On calcule la valeur du stock et la liste
 * des produits à recommander, avec une quantité suggérée.
 * On apprend : les tableaux de structures, les fonctions qui renvoient un
 * code d'erreur, la recherche par référence et les rapports de synthèse.
 */
#include <stdio.h>
#include <string.h>

#define MAX_PRODUITS 50

typedef struct {
    char ref[12];
    char nom[40];
    int stock, seuil;
    long prix_achat, prix_vente;
    long ventes;
} Produit;

typedef enum { OK, INTROUVABLE, STOCK_INSUFFISANT, QUANTITE_INVALIDE } Code;
static const char *MESSAGES[] = {"ok", "produit introuvable", "stock insuffisant", "quantité invalide"};

static Produit catalogue[MAX_PRODUITS];
static int nb_produits;

static Produit *chercher(const char *ref) {
    for (int i = 0; i < nb_produits; i++)
        if (strcmp(catalogue[i].ref, ref) == 0) return &catalogue[i];
    return NULL;
}

static void ajouter(const char *ref, const char *nom, int stock, int seuil, long achat, long vente) {
    if (nb_produits >= MAX_PRODUITS) return;
    Produit *p = &catalogue[nb_produits++];
    snprintf(p->ref, sizeof p->ref, "%s", ref);
    snprintf(p->nom, sizeof p->nom, "%s", nom);
    p->stock = stock; p->seuil = seuil; p->prix_achat = achat; p->prix_vente = vente; p->ventes = 0;
}

static Code vendre(const char *ref, int quantite) {
    Produit *p = chercher(ref);
    if (!p) return INTROUVABLE;
    if (quantite <= 0) return QUANTITE_INVALIDE;
    if (quantite > p->stock) return STOCK_INSUFFISANT;
    p->stock -= quantite;
    p->ventes += quantite;
    return OK;
}

static Code recevoir(const char *ref, int quantite) {
    Produit *p = chercher(ref);
    if (!p) return INTROUVABLE;
    if (quantite <= 0) return QUANTITE_INVALIDE;
    p->stock += quantite;
    return OK;
}

static void operation(const char *libelle, const char *ref, int q, Code (*f)(const char *, int)) {
    Code c = f(ref, q);
    printf("  %-9s %-6s x%-3d → %s\n", libelle, ref, q, MESSAGES[c]);
}

static void rapport(void) {
    long valeur = 0, marge = 0;
    printf("\n%-6s %-24s %6s %6s %10s\n", "Réf", "Produit", "Stock", "Seuil", "Valeur");
    for (int i = 0; i < nb_produits; i++) {
        Produit *p = &catalogue[i];
        valeur += (long)p->stock * p->prix_achat;
        marge += p->ventes * (p->prix_vente - p->prix_achat);
        printf("%-6s %-24s %6d %6d %10ld%s\n", p->ref, p->nom, p->stock, p->seuil,
               (long)p->stock * p->prix_achat, p->stock <= p->seuil ? "  ⚠ à recommander" : "");
    }
    printf("Valeur du stock : %ld FCFA | marge réalisée : %ld FCFA\n", valeur, marge);
    printf("\nBon de commande suggéré (remonter à 3 × le seuil) :\n");
    for (int i = 0; i < nb_produits; i++)
        if (catalogue[i].stock <= catalogue[i].seuil)
            printf("  %-24s : %d unité(s)\n", catalogue[i].nom, 3 * catalogue[i].seuil - catalogue[i].stock);
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) {
        fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n");
        return 1;
    }
    ajouter("SAV01", "Savon de Marseille", 40, 10, 250, 400);
    ajouter("HUI05", "Huile 5 L", 12, 5, 6500, 7800);
    ajouter("RIZ25", "Riz 25 kg", 6, 4, 14500, 16500);
    ajouter("SUC01", "Sucre 1 kg", 20, 8, 600, 800);
    puts("Journal des opérations :");
    operation("vente", "SAV01", 25, vendre);
    operation("vente", "HUI05", 8, vendre);
    operation("vente", "RIZ25", 7, vendre);   /* refusée */
    operation("vente", "RIZ25", 3, vendre);
    operation("vente", "LAIT1", 1, vendre);   /* inconnu */
    operation("réception", "SAV01", 20, recevoir);
    operation("vente", "SUC01", 14, vendre);
    rapport();
    return 0;
}
