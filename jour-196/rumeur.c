/*
 * Jour 196 — Simulation de propagation d'une rumeur dans un réseau
 *
 * Problème : comment une information (vraie ou fausse) se répand-elle dans un
 * groupe WhatsApp, un quartier ou une école ? Pourquoi certaines rumeurs
 * s'éteignent-elles vite alors que d'autres touchent tout le monde ? Et
 * qu'est-ce qui freine le plus une fausse information ?
 * Solution : un réseau social simulé (graphe « petit monde » : chacun connaît
 * ses voisins proches plus quelques personnes au hasard) et le modèle de
 * propagation à trois états : ignorant, propagateur, sceptique. Chaque jour,
 * un propagateur transmet la rumeur à chacun de ses contacts avec une
 * probabilité p ; il cesse de la propager quand il rencontre quelqu'un qui la
 * connaît déjà (perte d'intérêt) ou après vérification. On compare plusieurs
 * scénarios : rumeur peu partagée, très partagée, et avec démenti précoce.
 * On apprend : les graphes en listes d'adjacence, le modèle de propagation
 * (proche des modèles d'épidémie), la simulation stochastique et la moyenne
 * sur plusieurs essais.
 */
#include <stdio.h>
#include <stdlib.h>

#define N 500
#define K 6           /* contacts proches : K/2 de chaque côté sur l'anneau */
#define MAX_DEG 32

static int voisins[N][MAX_DEG], degre[N];
static unsigned graine;

static double hasard(void) { graine = graine * 1103515245u + 12345u; return (graine >> 8) / 16777216.0; }

static void ajouter(int a, int b) {
    if (a == b || degre[a] >= MAX_DEG || degre[b] >= MAX_DEG) return;
    for (int i = 0; i < degre[a]; i++) if (voisins[a][i] == b) return;
    voisins[a][degre[a]++] = b;
    voisins[b][degre[b]++] = a;
}

static void construire_reseau(void) {   /* petit monde : anneau + raccourcis aléatoires */
    for (int i = 0; i < N; i++) degre[i] = 0;
    for (int i = 0; i < N; i++) for (int k = 1; k <= K / 2; k++) ajouter(i, (i + k) % N);
    for (int r = 0; r < N / 5; r++) ajouter((int)(hasard() * N), (int)(hasard() * N));
}

typedef enum { IGNORANT, PROPAGATEUR, SCEPTIQUE } Etat;

/* Retourne la part de la population touchée. */
static double simuler(double p_transmission, double p_verification, int jour_dementi, int *duree) {
    Etat e[N], suivant[N];
    for (int i = 0; i < N; i++) e[i] = IGNORANT;
    e[0] = PROPAGATEUR;
    int jour = 0, actifs = 1;
    while (actifs > 0 && jour < 200) {
        jour++;
        for (int i = 0; i < N; i++) suivant[i] = e[i];
        double verif = jour >= jour_dementi ? 0.5 : p_verification;   /* un démenti officiel accroît la vérification */
        for (int i = 0; i < N; i++) {
            if (e[i] != PROPAGATEUR) continue;
            for (int k = 0; k < degre[i]; k++) {
                int v = voisins[i][k];
                if (hasard() >= p_transmission) continue;
                if (e[v] == IGNORANT && suivant[v] == IGNORANT) suivant[v] = hasard() < verif ? SCEPTIQUE : PROPAGATEUR;
                else if (hasard() < 0.5) suivant[i] = SCEPTIQUE;   /* « tout le monde le sait déjà » : il se lasse */
            }
            if (hasard() < verif) suivant[i] = SCEPTIQUE;
        }
        actifs = 0;
        for (int i = 0; i < N; i++) { e[i] = suivant[i]; actifs += e[i] == PROPAGATEUR; }
    }
    int touches = 0;
    for (int i = 0; i < N; i++) touches += e[i] != IGNORANT;
    *duree = jour;
    return (double)touches / N;
}

int main(int argc, char *argv[]) {
    int essais = 30;
    if (argc == 2) {
        char *fin;
        essais = (int)strtol(argv[1], &fin, 10);
        if (*fin || essais < 1 || essais > 1000) { fprintf(stderr, "Usage : %s [nombre d'essais 1..1000]\n", argv[0]); return 1; }
    } else if (argc > 2) { fprintf(stderr, "Usage : %s [essais]\n", argv[0]); return 1; }
    struct { const char *nom; double p, verif; int dementi; } scenarios[] = {
        {"Rumeur peu partagée (p=0,10)", 0.10, 0.05, 999}, {"Rumeur virale (p=0,40)", 0.40, 0.05, 999},
        {"Virale, démenti au jour 3", 0.40, 0.05, 3}, {"Virale, démenti au jour 8", 0.40, 0.05, 8},
        {"Virale, public prudent (vérif. 25 %)", 0.40, 0.25, 999},
    };
    graine = 196;
    construire_reseau();
    double deg = 0;
    for (int i = 0; i < N; i++) deg += degre[i];
    printf("Réseau de %d personnes (%.1f contacts en moyenne), %d essais par scénario\n\n", N, deg / N, essais);
    printf("%-38s %10s %8s\n", "Scénario", "Touchés", "Durée");
    for (int s = 0; s < 5; s++) {
        double total = 0;
        long duree_tot = 0;
        for (int k = 0; k < essais; k++) {
            graine = (unsigned)(196 + k * 7919);
            construire_reseau();
            int d;
            total += simuler(scenarios[s].p, scenarios[s].verif, scenarios[s].dementi, &d);
            duree_tot += d;
        }
        double part = total / essais;
        printf("%-38s %9.0f%% %6.0f j  ", scenarios[s].nom, 100 * part, (double)duree_tot / essais);
        for (int b = 0; b < (int)(part * 40); b++) putchar('#');
        putchar('\n');
    }
    printf("\nUn démenti rapide et un public qui vérifie freinent bien plus la rumeur qu'un démenti tardif.\n");
    return 0;
}
