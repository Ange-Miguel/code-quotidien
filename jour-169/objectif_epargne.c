/*
 * Jour 169 — Objectifs d'épargne avec projection
 *
 * Problème : on veut acheter un ordinateur, payer une formation, constituer
 * l'apport d'un projet ou un fonds d'urgence. Combien mettre de côté chaque
 * mois pour y arriver à temps ? Et si on épargne un montant fixe, quand
 * atteindra-t-on chaque objectif ?
 * Solution : pour chaque objectif (montant cible, déjà épargné, échéance),
 * on calcule la mensualité nécessaire avec la formule de la valeur future
 * d'une suite de versements (si l'épargne rapporte des intérêts) :
 * versement = (cible − déjà × (1+t)^n) × t / ((1+t)^n − 1). Puis on simule
 * mois par mois le partage d'un budget d'épargne entre les objectifs par
 * ordre de priorité, et on affiche une barre de progression et la date
 * d'atteinte. Montants d'exemple.
 * On apprend : la valeur future, les versements périodiques, la simulation
 * mois par mois avec priorités et l'affichage de progression.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct { const char *nom; double cible, epargne; int mois_restants; int priorite; int atteint_au_mois; } Objectif;

static double mensualite(double cible, double deja, int mois, double taux_mensuel) {
    if (taux_mensuel == 0) return (cible - deja) / mois;
    double f = pow(1 + taux_mensuel, mois);
    return (cible - deja * f) * taux_mensuel / (f - 1);
}

static void barre(double fraction) {
    int n = (int)(fraction * 20 + 0.5);
    putchar('[');
    for (int i = 0; i < 20; i++) putchar(i < n ? '#' : '.');
    printf("] %3.0f %%", fraction * 100);
}

int main(int argc, char *argv[]) {
    double budget = 40000, taux = 0.035 / 12;     /* épargne rémunérée à 3,5 %/an (exemple) */
    if (argc == 2) {
        char *fin;
        budget = strtod(argv[1], &fin);
        if (*fin || budget <= 0) { fprintf(stderr, "Usage : %s [épargne mensuelle disponible en FCFA]\n", argv[0]); return 1; }
    } else if (argc > 2) { fprintf(stderr, "Usage : %s [épargne mensuelle]\n", argv[0]); return 1; }
    Objectif obj[] = {
        {"Fonds d'urgence", 150000, 40000, 6, 1, 0}, {"Ordinateur portable", 350000, 60000, 10, 2, 0},
        {"Formation en ligne", 90000, 0, 5, 3, 0}, {"Apport projet startup", 1000000, 100000, 24, 4, 0},
    };
    int n = 4;
    double besoin = 0;
    printf("Mensualités nécessaires pour respecter chaque échéance :\n");
    for (int i = 0; i < n; i++) {
        double m = mensualite(obj[i].cible, obj[i].epargne, obj[i].mois_restants, taux);
        besoin += m;
        printf("   %-24s %8.0f FCFA/mois pendant %2d mois\n", obj[i].nom, m, obj[i].mois_restants);
    }
    printf("   Total nécessaire : %.0f FCFA/mois ; disponible : %.0f FCFA/mois%s\n\n", besoin, budget,
           besoin > budget ? " → toutes les échéances ne pourront pas être tenues" : "");

    printf("Simulation avec %.0f FCFA/mois, par ordre de priorité :\n", budget);
    for (int mois = 1; mois <= 48; mois++) {
        double reste = budget;
        for (int i = 0; i < n; i++) obj[i].epargne *= 1 + taux;
        for (int p = 1; p <= n && reste > 0; p++)
            for (int i = 0; i < n; i++)
                if (obj[i].priorite == p && obj[i].epargne < obj[i].cible) {
                    double v = fmin(reste, obj[i].cible - obj[i].epargne);
                    obj[i].epargne += v;
                    reste -= v;
                    if (obj[i].epargne >= obj[i].cible - 0.5) obj[i].atteint_au_mois = mois;
                }
        if (mois == 6 || mois == 12) {
            printf(" après %2d mois :\n", mois);
            for (int i = 0; i < n; i++) { printf("   %-24s ", obj[i].nom); barre(fmin(1, obj[i].epargne / obj[i].cible)); printf("\n"); }
        }
    }
    printf("\nDates d'atteinte :\n");
    for (int i = 0; i < n; i++)
        printf("   %-24s %s mois %d (échéance souhaitée : mois %d)%s\n", obj[i].nom, obj[i].atteint_au_mois ? "" : "au-delà de",
               obj[i].atteint_au_mois ? obj[i].atteint_au_mois : 48, obj[i].mois_restants,
               obj[i].atteint_au_mois && obj[i].atteint_au_mois <= obj[i].mois_restants ? " ✔" : " ✘");
    return 0;
}
