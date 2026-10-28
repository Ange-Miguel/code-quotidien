/*
 * Jour 181 — Planification de tournées de livraison (plus proche voisin)
 *
 * Problème : un livreur à moto doit déposer une douzaine de colis entre
 * Cotonou et Calavi, puis revenir au dépôt. Dans quel ordre passer pour faire
 * le moins de kilomètres possible ? C'est le célèbre problème du voyageur de
 * commerce : impossible d'essayer tous les ordres (12! = 479 millions).
 * Solution : l'heuristique du plus proche voisin (toujours aller au client le
 * plus proche non encore servi) donne vite une tournée correcte. On
 * l'améliore ensuite avec 2-opt : on inverse un morceau de la tournée tant
 * que cela raccourcit le trajet (cela supprime les croisements). On compare
 * avec l'ordre de saisie des commandes. Coordonnées fictives en kilomètres.
 * On apprend : les heuristiques pour les problèmes difficiles, l'algorithme
 * glouton du plus proche voisin, l'amélioration locale 2-opt et la mesure
 * du gain obtenu.
 */
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 13   /* dépôt + 12 clients */
static const char *NOMS[N] = {"Dépôt Akpakpa", "Ganhi", "Cadjèhoun", "Fidjrossè", "Godomey", "Calavi Kpota", "Zogbadjè",
                              "Akassato", "Vèdoko", "Agla", "Haie Vive", "Cococodji", "Gbégamey"};
static const double X[N] = {9.0, 7.2, 5.1, 3.0, 2.8, 4.0, 3.2, 4.6, 6.0, 4.4, 4.9, 0.9, 5.9};
static const double Y[N] = {2.0, 2.6, 2.1, 1.6, 5.2, 8.9, 7.4, 11.3, 3.4, 3.9, 2.9, 4.6, 2.7};

static double d(int a, int b) { return hypot(X[a] - X[b], Y[a] - Y[b]); }

static double longueur(const int *t) {
    double l = 0;
    for (int i = 0; i < N; i++) l += d(t[i], t[(i + 1) % N]);
    return l;
}

static void plus_proche_voisin(int *t) {
    int servi[N] = {0};
    t[0] = 0; servi[0] = 1;
    for (int k = 1; k < N; k++) {
        int meilleur = -1;
        for (int c = 1; c < N; c++)
            if (!servi[c] && (meilleur < 0 || d(t[k - 1], c) < d(t[k - 1], meilleur))) meilleur = c;
        t[k] = meilleur; servi[meilleur] = 1;
    }
}

static int deux_opt(int *t) {
    int ameliorations = 0, ameliore = 1;
    while (ameliore) {
        ameliore = 0;
        for (int i = 1; i < N - 1; i++)
            for (int j = i + 1; j < N; j++) {
                int a = t[i - 1], b = t[i], c = t[j], e = t[(j + 1) % N];
                if (d(a, c) + d(b, e) < d(a, b) + d(c, e) - 1e-9) {     /* inverser t[i..j] raccourcit */
                    for (int g = i, h = j; g < h; g++, h--) { int tmp = t[g]; t[g] = t[h]; t[h] = tmp; }
                    ameliore = 1;
                    ameliorations++;
                }
            }
    }
    return ameliorations;
}

static void afficher(const char *titre, const int *t) {
    printf("%s : %.1f km\n   ", titre, longueur(t));
    for (int i = 0; i < N; i++) printf("%s → ", NOMS[t[i]]);
    printf("%s\n\n", NOMS[t[0]]);
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    int saisie[N], ppv[N];
    for (int i = 0; i < N; i++) saisie[i] = i;
    afficher("Ordre de saisie des commandes", saisie);
    plus_proche_voisin(ppv);
    afficher("Plus proche voisin", ppv);
    double avant = longueur(ppv);
    int n = deux_opt(ppv);
    afficher("Après amélioration 2-opt", ppv);
    printf("2-opt : %d inversion(s), gain %.1f km ; gain total %.0f %% par rapport à l'ordre de saisie\n",
           n, avant - longueur(ppv), 100 * (1 - longueur(ppv) / longueur(saisie)));
    printf("Carburant économisé sur 20 tournées par mois : ~%.0f L (2,5 L/100 km)\n",
           (longueur(saisie) - longueur(ppv)) * 20 * 2.5 / 100);
    return 0;
}
