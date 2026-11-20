/*
 * Jour 025 — Calcul du prix de revient d'un produit importé de Chine
 *
 * Problème : un produit acheté 3 € sur une plateforme chinoise ne coûte pas
 * 3 € une fois à Cotonou : il faut ajouter le fret, la douane, le transit et
 * les frais locaux. Sans ce calcul, on vend à perte sans le savoir.
 * Solution : on additionne le prix d'achat converti en FCFA, la part du fret
 * répartie au poids, les droits et taxes calculés sur la valeur CAF (coût +
 * assurance + fret) et les frais fixes répartis sur la quantité. Les taux
 * sont des valeurs d'exemple à adapter.
 * On apprend : les structures, les fonctions qui calculent plusieurs
 * résultats, la notion de valeur CAF et la répartition de coûts.
 */
#include <stdio.h>
#include <stdlib.h>

#define XOF_PAR_EUR 655.957

typedef struct {
    const char *nom;
    double prix_unitaire_eur;
    int quantite;
    double poids_unitaire_kg;
} Lot;

typedef struct {
    double fret_par_kg_fcfa;   /* fret aérien ou maritime groupé */
    double taux_assurance;     /* en fraction de la valeur */
    double taux_douane;        /* droits + taxes, fraction de la valeur CAF */
    double frais_fixes_fcfa;   /* transitaire, dédouanement, transport local */
} Parametres;

static double prix_revient_unitaire(Lot l, Parametres p, int afficher) {
    double achat = l.prix_unitaire_eur * l.quantite * XOF_PAR_EUR;
    double fret = l.poids_unitaire_kg * l.quantite * p.fret_par_kg_fcfa;
    double assurance = achat * p.taux_assurance;
    double caf = achat + fret + assurance;
    double douane = caf * p.taux_douane;
    double total = caf + douane + p.frais_fixes_fcfa;
    double unitaire = total / l.quantite;
    if (afficher) {
        printf("%s : %d unités\n", l.nom, l.quantite);
        printf("  Achat          %12.0f FCFA\n", achat);
        printf("  Fret           %12.0f FCFA\n", fret);
        printf("  Assurance      %12.0f FCFA\n", assurance);
        printf("  Valeur CAF     %12.0f FCFA\n", caf);
        printf("  Douane/taxes   %12.0f FCFA\n", douane);
        printf("  Frais fixes    %12.0f FCFA\n", p.frais_fixes_fcfa);
        printf("  TOTAL          %12.0f FCFA\n", total);
        printf("  => prix de revient : %.0f FCFA/unité (x%.1f le prix d'achat)\n\n",
               unitaire, unitaire / (l.prix_unitaire_eur * XOF_PAR_EUR));
    }
    return unitaire;
}

int main(int argc, char *argv[]) {
    Parametres p = {6500.0, 0.01, 0.40, 35000.0}; /* valeurs d'exemple */
    if (argc == 1) {
        Lot lots[] = {{"Écouteurs Bluetooth", 3.20, 100, 0.08},
                      {"Montres connectées", 9.50, 40, 0.15},
                      {"Lampes solaires", 4.00, 60, 0.45}};
        for (int i = 0; i < 3; i++) prix_revient_unitaire(lots[i], p, 1);
        return 0;
    }
    if (argc != 4) {
        fprintf(stderr, "Usage : %s <prix unitaire €> <quantité> <poids unitaire kg>\n", argv[0]);
        return 1;
    }
    char *f1, *f2, *f3;
    Lot l = {"Votre produit", strtod(argv[1], &f1), (int)strtol(argv[2], &f2, 10), strtod(argv[3], &f3)};
    if (*f1 || *f2 || *f3 || l.prix_unitaire_eur <= 0 || l.quantite <= 0 || l.poids_unitaire_kg <= 0) {
        fprintf(stderr, "Valeurs invalides.\n");
        return 1;
    }
    prix_revient_unitaire(l, p, 1);
    return 0;
}
