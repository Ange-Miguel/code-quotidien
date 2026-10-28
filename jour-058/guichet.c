/*
 * Jour 058 — File d'attente (queue) : simulation d'un guichet de banque
 *
 * Problème : une agence de banque ou de mobile money se demande combien de
 * guichets ouvrir pour que les clients n'attendent pas trop, sans payer des
 * agents inutiles.
 * Solution : on simule minute par minute l'arrivée aléatoire des clients
 * (probabilité par minute) dans une file FIFO implémentée par un tableau
 * circulaire. Chaque guichet libre prend le client en tête de file. On mesure
 * l'attente moyenne et maximale et la longueur de file, pour 1, 2 et 3
 * guichets.
 * On apprend : la file circulaire (enfiler/défiler en O(1)), la simulation à
 * événements discrets et l'interprétation de résultats de simulation.
 */
#include <stdio.h>
#include <stdlib.h>

#define CAPACITE 512

typedef struct {
    int arrivees[CAPACITE];
    int tete, queue, taille;
} File;

static int enfiler(File *f, int minute) {
    if (f->taille == CAPACITE) return 0;
    f->arrivees[f->queue] = minute;
    f->queue = (f->queue + 1) % CAPACITE;
    f->taille++;
    return 1;
}

static int defiler(File *f) {
    int v = f->arrivees[f->tete];
    f->tete = (f->tete + 1) % CAPACITE;
    f->taille--;
    return v;
}

static unsigned long graine;
static double hasard(void) {   /* uniforme dans [0, 1[ */
    graine = graine * 6364136223846793005UL + 1442695040888963407UL;
    return (double)(graine >> 11) / 9007199254740992.0;
}

static void simuler(int guichets, double proba_arrivee, int duree_service_max, int minutes) {
    File file = {{0}, 0, 0, 0};
    int occupe[8] = {0};
    long servis = 0, attente_totale = 0;
    int attente_max = 0, file_max = 0, refus = 0;
    graine = 58;
    for (int t = 0; t < minutes; t++) {
        if (hasard() < proba_arrivee && !enfiler(&file, t)) refus++;
        for (int g = 0; g < guichets; g++) {
            if (occupe[g] > 0) occupe[g]--;
            if (occupe[g] == 0 && file.taille > 0) {
                int attente = t - defiler(&file);
                attente_totale += attente;
                if (attente > attente_max) attente_max = attente;
                servis++;
                occupe[g] = 2 + (int)(hasard() * (duree_service_max - 1));
            }
        }
        if (file.taille > file_max) file_max = file.taille;
    }
    printf("%d guichet(s) : %4ld clients servis | attente moyenne %5.1f min | max %3d min | "
           "file max %3d | restants %d\n", guichets, servis,
           servis ? (double)attente_totale / servis : 0.0, attente_max, file_max, file.taille + refus);
}

int main(int argc, char *argv[]) {
    double proba = 0.45;   /* ~27 clients par heure */
    if (argc == 2) {
        char *fin;
        proba = strtod(argv[1], &fin);
        if (*fin || proba <= 0 || proba > 1) {
            fprintf(stderr, "Usage : %s [probabilité d'arrivée par minute, 0..1]\n", argv[0]);
            return 1;
        }
    } else if (argc > 2) {
        fprintf(stderr, "Usage : %s [probabilité]\n", argv[0]);
        return 1;
    }
    printf("Journée de 8 h, %.0f arrivées/heure en moyenne, service de 2 à 6 min\n\n", proba * 60);
    for (int g = 1; g <= 4; g++) simuler(g, proba, 6, 8 * 60);
    return 0;
}
