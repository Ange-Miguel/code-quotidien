/*
 * Jour 055 — Lecture d'un fichier CSV de ventes et total par produit
 *
 * Problème : une caisse ou une feuille de calcul exporte les ventes en CSV
 * (date;produit;quantité;prix). On veut le chiffre d'affaires par produit sans
 * ouvrir un tableur, par exemple sur une petite carte embarquée.
 * Solution : on lit le fichier ligne par ligne avec fgets, on découpe chaque
 * ligne avec strtok_r, on valide les champs numériques et on cumule dans une
 * table de produits. Les lignes invalides sont signalées avec leur numéro au
 * lieu de faire planter le programme. En démo, un fichier temporaire est créé.
 * On apprend : fgets, strtok_r, strtol avec contrôle d'erreur, tmpfile et
 * l'agrégation de données.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUITS 64

typedef struct { char nom[48]; long quantite; long montant; } Total;

static Total totaux[MAX_PRODUITS];
static int nb;

static Total *total_pour(const char *nom) {
    for (int i = 0; i < nb; i++) if (strcmp(totaux[i].nom, nom) == 0) return &totaux[i];
    if (nb == MAX_PRODUITS) return NULL;
    snprintf(totaux[nb].nom, sizeof totaux[nb].nom, "%s", nom);
    return &totaux[nb++];
}

static int lire_entier(const char *s, long *v) {
    char *fin;
    if (!s) return 0;
    *v = strtol(s, &fin, 10);
    return *fin == '\0' || *fin == '\n' || *fin == '\r';
}

static int traiter(FILE *f) {
    char ligne[256];
    int numero = 0, erreurs = 0;
    while (fgets(ligne, sizeof ligne, f)) {
        numero++;
        if (numero == 1 || ligne[0] == '\n') continue;   /* en-tête */
        char *etat;
        strtok_r(ligne, ";", &etat);                     /* date, ignorée ici */
        char *produit = strtok_r(NULL, ";", &etat);
        long q, prix;
        if (!produit || !lire_entier(strtok_r(NULL, ";", &etat), &q) ||
            !lire_entier(strtok_r(NULL, ";", &etat), &prix) || q <= 0 || prix < 0) {
            fprintf(stderr, "  ligne %d ignorée (format invalide)\n", numero);
            erreurs++;
            continue;
        }
        Total *t = total_pour(produit);
        if (t) { t->quantite += q; t->montant += q * prix; }
    }
    long ca = 0;
    printf("%-22s %8s %12s\n", "Produit", "Qté", "CA (FCFA)");
    for (int i = 0; i < nb; i++) {
        printf("%-22s %8ld %12ld\n", totaux[i].nom, totaux[i].quantite, totaux[i].montant);
        ca += totaux[i].montant;
    }
    printf("Chiffre d'affaires total : %ld FCFA (%d ligne(s) ignorée(s))\n", ca, erreurs);
    return erreurs;
}

int main(int argc, char *argv[]) {
    FILE *f;
    if (argc == 2) {
        f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
    } else if (argc == 1) {
        f = tmpfile();
        if (!f) { perror("tmpfile"); return 2; }
        fputs("date;produit;quantite;prix\n"
              "2026-10-01;Écouteurs;3;4500\n2026-10-01;Chargeur;5;2500\n"
              "2026-10-02;Écouteurs;2;4500\n2026-10-02;Câble USB-C;10;1500\n"
              "2026-10-03;Chargeur;x;2500\n2026-10-03;Montre;1;12000\n"
              "2026-10-04;Câble USB-C;4;1500\n", f);
        rewind(f);
    } else {
        fprintf(stderr, "Usage : %s [ventes.csv]\n", argv[0]);
        return 1;
    }
    traiter(f);
    fclose(f);
    return 0;
}
