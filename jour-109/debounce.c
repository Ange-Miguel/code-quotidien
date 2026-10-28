/*
 * Jour 109 — Anti-rebond (debounce) logiciel d'un bouton
 *
 * Problème : quand on appuie sur un bouton mécanique, le contact « rebondit »
 * pendant quelques millisecondes : le microcontrôleur voit une rafale
 * 0-1-0-1-0-1 au lieu d'un seul appui. Un compteur d'appuis compte alors 5 ou
 * 10 appuis pour un seul.
 * Solution : on échantillonne l'entrée toutes les 1 ms et on ne valide un
 * changement d'état que s'il reste stable pendant N échantillons consécutifs
 * (compteur de stabilité). Variante très compacte : un registre à décalage de
 * 8 bits où l'état n'est accepté que quand les 8 derniers échantillons sont
 * identiques (0x00 ou 0xFF). On compare avec la méthode naïve sur un signal
 * simulé avec rebonds.
 * On apprend : le phénomène de rebond, l'échantillonnage périodique, les
 * compteurs de stabilité, le registre à décalage et la détection de front.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define STABLE_MS 5

/* Signal simulé : 3 appuis avec rebonds, échantillonné à 1 ms. */
static int signal_bouton(int t, unsigned *graine) {
    static const int appuis[][2] = {{20, 120}, {200, 260}, {330, 450}};   /* début, fin (ms) */
    for (int i = 0; i < 3; i++) {
        int d = appuis[i][0], f = appuis[i][1];
        if ((t >= d && t < d + 4) || (t >= f && t < f + 4)) {            /* zone de rebond */
            *graine = *graine * 1103515245u + 12345u;
            return (*graine >> 16) & 1;
        }
        if (t >= d + 4 && t < f) return 1;
    }
    return 0;
}

typedef struct { int etat; int compteur; } Stabilite;

static int filtre_compteur(Stabilite *s, int brut) {
    if (brut == s->etat) { s->compteur = 0; return s->etat; }
    if (++s->compteur >= STABLE_MS) { s->etat = brut; s->compteur = 0; }
    return s->etat;
}

static int filtre_registre(uint8_t *historique, int *etat, int brut) {
    *historique = (uint8_t)((*historique << 1) | (brut & 1));
    if (*historique == 0xFF) *etat = 1;
    else if (*historique == 0x00) *etat = 0;
    return *etat;
}

int main(int argc, char *argv[]) {
    unsigned graine = 109;
    if (argc == 2) {
        char *fin;
        graine = (unsigned)strtoul(argv[1], &fin, 10);
        if (*fin) { fprintf(stderr, "Usage : %s [graine du bruit]\n", argv[0]); return 1; }
    } else if (argc > 2) { fprintf(stderr, "Usage : %s [graine]\n", argv[0]); return 1; }
    Stabilite s = {0, 0};
    uint8_t hist = 0;
    int etat_reg = 0, prec_brut = 0, prec_cpt = 0, prec_reg = 0;
    int fronts_brut = 0, fronts_cpt = 0, fronts_reg = 0;
    printf("t(ms)  brut  compteur  registre\n");
    for (int t = 0; t < 500; t++) {
        int brut = signal_bouton(t, &graine);
        int c = filtre_compteur(&s, brut), r = filtre_registre(&hist, &etat_reg, brut);
        fronts_brut += brut && !prec_brut;          /* fronts montants = appuis détectés */
        fronts_cpt += c && !prec_cpt;
        fronts_reg += r && !prec_reg;
        if (t >= 198 && t <= 212) printf("%4d    %d       %d         %d\n", t, brut, c, r);
        prec_brut = brut; prec_cpt = c; prec_reg = r;
    }
    printf("...\nAppuis réels : 3\n");
    printf("Détectés sans filtre        : %d\n", fronts_brut);
    printf("Détectés avec compteur (%d ms): %d\n", STABLE_MS, fronts_cpt);
    printf("Détectés avec registre 8 bits: %d\n", fronts_reg);
    return 0;
}
