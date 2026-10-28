/*
 * Jour 142 — Planification d'allumage des lumières (scènes horaires)
 *
 * Problème : on veut que l'éclairage extérieur s'allume au coucher du soleil,
 * que la lampe de la chambre s'allume doucement le matin en semaine, et qu'en
 * vacances une « simulation de présence » allume et éteint des pièces de
 * façon crédible, pour décourager les cambrioleurs.
 * Solution : une table de règles : jours de la semaine (masque de 7 bits),
 * heure de début et de fin (en minutes depuis minuit, avec gestion du passage
 * de minuit), lampe concernée et niveau. Pour savoir l'état à un instant, on
 * évalue toutes les règles actives ; la dernière qui s'applique l'emporte. Le
 * mode vacances ajoute un décalage pseudo-aléatoire de ±20 min chaque jour.
 * On apprend : les masques de bits pour les jours, l'arithmétique des heures
 * modulo 24 h, les plages qui passent minuit et la priorité entre règles.
 */
#include <stdio.h>
#include <string.h>

enum { LUN = 1, MAR = 2, MER = 4, JEU = 8, VEN = 16, SAM = 32, DIM = 64, SEMAINE = 31, TOUS = 127 };
static const char *JOURS[] = {"lun", "mar", "mer", "jeu", "ven", "sam", "dim"};
static const char *LAMPES[] = {"Extérieur", "Salon", "Chambre", "Cuisine"};
#define NB_LAMPES 4

typedef struct { const char *nom; int jours; int debut, fin; int lampe, niveau; int vacances_seulement; } Regle;

#define H(h, m) ((h) * 60 + (m))
static const Regle REGLES[] = {
    {"extérieur la nuit", TOUS, H(18, 45), H(6, 15), 0, 100, 0},
    {"réveil progressif", SEMAINE, H(6, 0), H(6, 30), 2, 40, 0},
    {"préparation du matin", SEMAINE, H(6, 30), H(7, 15), 3, 100, 0},
    {"soirée salon", TOUS, H(19, 0), H(22, 30), 1, 80, 0},
    {"lecture du week-end", SAM | DIM, H(21, 30), H(23, 0), 2, 60, 0},
    {"présence : salon", TOUS, H(19, 30), H(22, 0), 1, 100, 1},
    {"présence : chambre", TOUS, H(22, 0), H(23, 10), 2, 50, 1},
};
#define NB_REGLES (int)(sizeof REGLES / sizeof REGLES[0])

static int dans_plage(int minute, int debut, int fin) {
    return debut <= fin ? (minute >= debut && minute < fin) : (minute >= debut || minute < fin);   /* passe minuit */
}

static int decalage_vacances(int jour, int regle) {     /* pseudo-aléatoire mais reproductible : ±20 min */
    unsigned h = (unsigned)(jour * 7919 + regle * 104729);
    h ^= h >> 13; h *= 0x5bd1e995u; h ^= h >> 15;
    return (int)(h % 41) - 20;
}

static void etat(int jour, int minute, int vacances, int niveaux[NB_LAMPES]) {
    memset(niveaux, 0, sizeof(int) * NB_LAMPES);
    for (int r = 0; r < NB_REGLES; r++) {
        const Regle *g = &REGLES[r];
        if (!(g->jours & (1 << jour))) continue;
        /* En vacances : règles de présence + éclairage extérieur ; sinon : règles normales seulement */
        if (vacances ? !(g->vacances_seulement || g->lampe == 0) : g->vacances_seulement) continue;
        int d = vacances && g->vacances_seulement ? decalage_vacances(jour, r) : 0;
        if (dans_plage(minute, (g->debut + d + 1440) % 1440, (g->fin + d + 1440) % 1440)) niveaux[g->lampe] = g->niveau;
    }
}

static void journee(int jour, int vacances) {
    int precedent[NB_LAMPES] = {-1, -1, -1, -1}, n[NB_LAMPES];
    printf("%s%s :\n", JOURS[jour], vacances ? " (mode vacances)" : "");
    for (int m = 0; m < 1440; m++) {
        etat(jour, m, vacances, n);
        for (int l = 0; l < NB_LAMPES; l++)
            if (n[l] != precedent[l]) {
                if (precedent[l] != -1 || n[l]) printf("   %02d:%02d  %-10s %s\n", m / 60, m % 60, LAMPES[l], n[l] ? (n[l] == 100 ? "allumée" : "allumée (atténuée)") : "éteinte");
                precedent[l] = n[l];
            }
    }
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    journee(0, 0);          /* lundi normal */
    journee(5, 0);          /* samedi */
    journee(2, 1);          /* mercredi en vacances */
    journee(3, 1);          /* jeudi en vacances : horaires différents */
    return 0;
}
