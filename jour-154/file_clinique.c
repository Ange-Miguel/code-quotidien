/*
 * Jour 154 — Gestion de file d'attente d'une clinique avec priorités
 *
 * Problème : dans une clinique, le premier arrivé n'est pas toujours le
 * premier soigné : une urgence passe avant une consultation de routine. Mais
 * il ne faut pas non plus qu'un patient « peu urgent » attende indéfiniment.
 * Solution : un tas binaire (file de priorité) implémenté dans un tableau. La
 * priorité dépend du niveau de triage (1 = urgence vitale … 4 = non urgent)
 * et du temps d'attente : chaque tranche de 30 minutes d'attente améliore la
 * priorité (vieillissement), et l'ordre d'arrivée départage les égalités. On
 * simule une matinée avec deux médecins et on mesure l'attente par niveau.
 * Les niveaux de triage sont simplifiés, pour l'exemple.
 * On apprend : le tas binaire (insertion et extraction en O(log n)), les
 * fonctions de comparaison à plusieurs critères et le vieillissement des
 * priorités pour éviter la famine.
 */
#include <stdio.h>
#include <string.h>

#define MAX 64
typedef struct { char nom[24]; int niveau, arrivee, numero; } Patient;

static Patient tas[MAX];
static int taille, maintenant;

/* Plus petit = plus prioritaire. Vieillissement : -1 niveau par tranche de 30 min d'attente. */
static int score(const Patient *p) {
    int niveau = p->niveau - (maintenant - p->arrivee) / 30;
    return (niveau < 1 ? 1 : niveau) * 10000 + p->numero;
}

static void echanger(int a, int b) { Patient t = tas[a]; tas[a] = tas[b]; tas[b] = t; }

static void tamiser_bas(int i) {
    for (;;) {
        int g = 2 * i + 1, d = g + 1, m = i;
        if (g < taille && score(&tas[g]) < score(&tas[m])) m = g;
        if (d < taille && score(&tas[d]) < score(&tas[m])) m = d;
        if (m == i) return;
        echanger(i, m);
        i = m;
    }
}

static void inserer(Patient p) {
    if (taille == MAX) return;
    int i = taille++;
    tas[i] = p;
    while (i > 0 && score(&tas[i]) < score(&tas[(i - 1) / 2])) { echanger(i, (i - 1) / 2); i = (i - 1) / 2; }
}

static Patient extraire(void) {
    for (int i = taille / 2 - 1; i >= 0; i--) tamiser_bas(i);   /* les scores ont vieilli : on reconstruit */
    Patient p = tas[0];
    tas[0] = tas[--taille];
    tamiser_bas(0);
    return p;
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    struct { int minute; const char *nom; int niveau; } arrivees[] = {
        {0, "M. Adjovi", 4}, {2, "Mme Dossou", 3}, {5, "Bébé Kpade", 2}, {8, "M. Zinsou", 4}, {10, "Mme Agossa", 4},
        {12, "M. Houngbo", 1}, {20, "Mme Tchibozo", 3}, {25, "M. Mensah", 4}, {31, "Mlle Sossou", 2}, {40, "M. Gbaguidi", 4},
        {45, "Mme Ahouandjinou", 3}, {50, "M. Dah", 4}};
    const char *NIVEAUX[] = {"", "vital", "urgent", "semi-urgent", "non urgent"};
    int nb = (int)(sizeof arrivees / sizeof arrivees[0]), prochain = 0, numero = 0;
    int libre_a[2] = {0, 0};
    double attente_totale[5] = {0};
    int compte[5] = {0};
    printf("Heure   Médecin  Patient            Triage        Attente\n");
    for (maintenant = 0; maintenant < 300 && (prochain < nb || taille > 0); maintenant++) {
        while (prochain < nb && arrivees[prochain].minute == maintenant) {
            Patient p = {"", arrivees[prochain].niveau, maintenant, numero++};
            snprintf(p.nom, sizeof p.nom, "%s", arrivees[prochain].nom);
            inserer(p);
            prochain++;
        }
        for (int m = 0; m < 2; m++)
            if (libre_a[m] <= maintenant && taille > 0) {
                Patient p = extraire();
                int attente = maintenant - p.arrivee;
                attente_totale[p.niveau] += attente;
                compte[p.niveau]++;
                libre_a[m] = maintenant + (p.niveau <= 2 ? 25 : 15);
                printf("%02dh%02d   Dr %c     %-18s %-12s %4d min\n", 8 + maintenant / 60, maintenant % 60, 'A' + m, p.nom, NIVEAUX[p.niveau], attente);
            }
    }
    printf("\nAttente moyenne par niveau :");
    for (int n = 1; n <= 4; n++) if (compte[n]) printf("  %s %.0f min", NIVEAUX[n], attente_totale[n] / compte[n]);
    printf("\n");
    return 0;
}
