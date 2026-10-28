/*
 * Jour 184 — Gestion d'une file d'impression partagée
 *
 * Problème : dans un cybercafé, un secrétariat ou une salle informatique,
 * plusieurs personnes envoient des documents à la même imprimante. Un gros
 * document de 200 pages bloque tout le monde pendant que d'autres attendent
 * pour une seule page, et on veut facturer chaque utilisateur.
 * Solution : une file d'attente circulaire des travaux d'impression, avec
 * une politique choisie à l'exécution : FIFO (premier arrivé, premier servi)
 * ou SJF (le plus court d'abord, qui minimise l'attente moyenne). Une
 * protection anti-famine fait passer un travail qui attend depuis trop
 * longtemps. On simule l'imprimante (pages par minute), on mesure l'attente
 * moyenne et maximale, et on édite la facture par utilisateur.
 * On apprend : l'ordonnancement de travaux (FIFO, plus court d'abord), la
 * famine et le vieillissement, et la comparaison de politiques.
 */
#include <stdio.h>
#include <string.h>

#define MAX 32
#define PAGES_PAR_MIN 20
#define PRIX_PAGE 25
#define ATTENTE_MAX 15   /* minutes avant de passer en priorité */

typedef struct { int id; char utilisateur[16]; int pages; int arrivee; } Travail;
typedef enum { FIFO, SJF } Politique;

static const Travail ENTREES[] = {
    {1, "Awa", 3, 0}, {2, "Kofi", 120, 1}, {3, "Inès", 1, 2}, {4, "Serge", 8, 2}, {5, "Awa", 2, 4}, {6, "Carine", 45, 5},
    {7, "Yves", 1, 6}, {8, "Kofi", 4, 8}, {9, "Inès", 15, 9}, {10, "Paul", 2, 12}, {11, "Serge", 60, 13}, {12, "Mireille", 1, 14},
};
#define NB (int)(sizeof ENTREES / sizeof ENTREES[0])

static void simuler(Politique pol) {
    Travail file[MAX];
    int nb = 0, prochain = 0, termines = 0, attente_max = 0;
    double attente_tot = 0, temps = 0;
    printf("Politique %s :\n", pol == FIFO ? "FIFO (ordre d'arrivée)" : "plus court d'abord + anti-famine");
    while (termines < NB) {
        while (prochain < NB && ENTREES[prochain].arrivee <= temps) file[nb++] = ENTREES[prochain++];
        if (nb == 0) { temps = ENTREES[prochain].arrivee; continue; }
        int choix = 0;
        if (pol == SJF)
            for (int i = 1; i < nb; i++) {
                int vieux_i = temps - file[i].arrivee >= ATTENTE_MAX, vieux_c = temps - file[choix].arrivee >= ATTENTE_MAX;
                if (vieux_i != vieux_c ? vieux_i : file[i].pages < file[choix].pages) choix = i;
            }
        Travail t = file[choix];
        memmove(&file[choix], &file[choix + 1], sizeof(Travail) * (size_t)(nb - choix - 1));
        nb--;
        int attente = (int)(temps - t.arrivee);
        attente_tot += attente;
        if (attente > attente_max) attente_max = attente;
        if (pol == SJF || termines < 5) printf("   %5.1f min  #%-2d %-8s %3d p.  attendu %2d min\n", temps, t.id, t.utilisateur, t.pages, attente);
        temps += (double)t.pages / PAGES_PAR_MIN;
        termines++;
    }
    if (pol == FIFO) printf("   …\n");
    printf("   → attente moyenne %.1f min, maximale %d min, tout imprimé en %.1f min\n\n", attente_tot / NB, attente_max, temps);
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    simuler(FIFO);
    simuler(SJF);
    printf("Facturation (%d FCFA la page) :\n", PRIX_PAGE);
    const char *vus[NB];
    int nv = 0;
    for (int i = 0; i < NB; i++) {
        int deja = 0;
        for (int k = 0; k < nv; k++) if (!strcmp(vus[k], ENTREES[i].utilisateur)) deja = 1;
        if (deja) continue;
        vus[nv++] = ENTREES[i].utilisateur;
        int pages = 0;
        for (int j = 0; j < NB; j++) if (!strcmp(ENTREES[j].utilisateur, ENTREES[i].utilisateur)) pages += ENTREES[j].pages;
        printf("   %-9s %4d pages  %6d FCFA\n", ENTREES[i].utilisateur, pages, pages * PRIX_PAGE);
    }
    return 0;
}
