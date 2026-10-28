/*
 * Jour 070 — Carte des quartiers de Calavi en graphe et parcours BFS
 *
 * Problème : un livreur ou un conducteur de zem veut savoir combien de
 * quartiers il doit traverser pour aller d'un point à un autre, et quels
 * quartiers sont à 1, 2 ou 3 « sauts » de sa base.
 * Solution : les quartiers sont les sommets d'un graphe non pondéré
 * représenté par une matrice d'adjacence (liaisons simplifiées pour
 * l'exemple). Le parcours en largeur (BFS) avec une file visite les quartiers
 * par distance croissante : il donne le plus court chemin en nombre de
 * quartiers et les « niveaux » autour du départ.
 * On apprend : la matrice d'adjacence, le parcours en largeur, la file
 * d'attente avec un simple tableau, et la reconstruction de chemin.
 */
#include <stdio.h>
#include <string.h>
#include <strings.h>

enum { N = 12 };
static const char *QUARTIERS[N] = {
    "Calavi Centre", "Zogbadjè", "Tankpè", "Kpota", "Akassato", "Togba",
    "Godomey", "Cococodji", "Arconville", "Womey", "Hêvié", "Ouèdo"
};
static int adj[N][N];

static void relier(int a, int b) { adj[a][b] = adj[b][a] = 1; }

static void construire(void) {   /* liaisons simplifiées, pour l'exemple */
    relier(0, 1); relier(0, 2); relier(0, 3); relier(1, 2); relier(3, 4); relier(4, 5);
    relier(2, 8); relier(8, 6); relier(6, 7); relier(6, 9); relier(9, 10); relier(10, 11);
    relier(5, 11); relier(1, 8);
}

static int trouver(const char *nom) {
    for (int i = 0; i < N; i++) if (strcasecmp(QUARTIERS[i], nom) == 0) return i;
    return -1;
}

static void bfs(int depart, int dist[N], int prec[N]) {
    int file[N], tete = 0, queue = 0;
    for (int i = 0; i < N; i++) { dist[i] = -1; prec[i] = -1; }
    dist[depart] = 0;
    file[queue++] = depart;
    while (tete < queue) {
        int u = file[tete++];
        for (int v = 0; v < N; v++)
            if (adj[u][v] && dist[v] < 0) {
                dist[v] = dist[u] + 1;
                prec[v] = u;
                file[queue++] = v;
            }
    }
}

static void chemin(int depart, int arrivee) {
    int dist[N], prec[N], etapes[N], n = 0;
    bfs(depart, dist, prec);
    if (dist[arrivee] < 0) { printf("Aucun chemin.\n"); return; }
    for (int v = arrivee; v != -1; v = prec[v]) etapes[n++] = v;
    printf("%s → %s : %d saut(s)\n   ", QUARTIERS[depart], QUARTIERS[arrivee], dist[arrivee]);
    for (int i = n - 1; i >= 0; i--) printf("%s%s", QUARTIERS[etapes[i]], i ? " → " : "\n");
}

static void niveaux(int depart) {
    int dist[N], prec[N], max = 0;
    bfs(depart, dist, prec);
    for (int i = 0; i < N; i++) if (dist[i] > max) max = dist[i];
    printf("Autour de %s :\n", QUARTIERS[depart]);
    for (int d = 1; d <= max; d++) {
        printf("   à %d saut(s) :", d);
        for (int i = 0; i < N; i++) if (dist[i] == d) printf(" %s;", QUARTIERS[i]);
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    construire();
    if (argc == 3) {
        int a = trouver(argv[1]), b = trouver(argv[2]);
        if (a < 0 || b < 0) {
            fprintf(stderr, "Quartier inconnu. Quartiers :");
            for (int i = 0; i < N; i++) fprintf(stderr, " %s;", QUARTIERS[i]);
            fprintf(stderr, "\n");
            return 1;
        }
        chemin(a, b);
        return 0;
    }
    if (argc != 1) { fprintf(stderr, "Usage : %s [départ arrivée]\n", argv[0]); return 1; }
    chemin(trouver("Kpota"), trouver("Cococodji"));
    chemin(trouver("Togba"), trouver("Arconville"));
    niveaux(trouver("Calavi Centre"));
    return 0;
}
