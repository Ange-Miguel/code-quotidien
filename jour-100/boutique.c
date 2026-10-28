/*
 * Jour 100 — Bilan de la phase 2 : mini-application de gestion de boutique
 *
 * Problème : réunir dans un seul programme ce qu'on a appris pendant les jours
 * 51 à 99 : structures, fichiers, recherche, tri, files et rapports, pour
 * gérer une vraie petite boutique (produits, ventes, tickets de caisse).
 * Solution : un interpréteur de commandes texte (lu depuis l'entrée standard
 * ou un script) : « produit », « vendre », « stock », « rapport », « sauver »,
 * « charger ». Le catalogue est trié et interrogé par recherche dichotomique,
 * chaque vente produit un ticket numéroté, le rapport classe les produits
 * par chiffre d'affaires, et l'état peut être sauvegardé dans un fichier CSV.
 * On apprend : l'architecture d'un programme complet en C (commandes,
 * données, persistance, rapports), bsearch/qsort et la robustesse face aux
 * commandes invalides.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct { char ref[12]; char nom[40]; long prix; int stock; long vendus; } Produit;
static Produit cat[MAX];
static int nb, tickets;

static int par_ref(const void *a, const void *b) { return strcmp(((const Produit *)a)->ref, ((const Produit *)b)->ref); }
static int par_ca(const void *a, const void *b) {
    const Produit *x = a, *y = b;
    long cx = x->prix * x->vendus, cy = y->prix * y->vendus;
    return (cy > cx) - (cy < cx);
}

static Produit *trouver(const char *ref) {
    Produit cle;
    snprintf(cle.ref, sizeof cle.ref, "%s", ref);
    return bsearch(&cle, cat, (size_t)nb, sizeof cat[0], par_ref);
}

static void cmd_produit(char *ref, char *nom, char *prix, char *stock) {
    if (!ref || !nom || !prix || !stock || nb == MAX) { puts("  usage : produit REF NOM PRIX STOCK"); return; }
    if (trouver(ref)) { printf("  %s existe déjà\n", ref); return; }
    Produit *p = &cat[nb++];
    snprintf(p->ref, sizeof p->ref, "%s", ref);
    snprintf(p->nom, sizeof p->nom, "%s", nom);
    p->prix = atol(prix); p->stock = atoi(stock); p->vendus = 0;
    qsort(cat, (size_t)nb, sizeof cat[0], par_ref);          /* garder le catalogue trié */
    printf("  ajouté : %s\n", ref);
}

static void cmd_vendre(char *ref, char *qte) {
    Produit *p = ref ? trouver(ref) : NULL;
    int q = qte ? atoi(qte) : 0;
    if (!p || q <= 0) { puts("  vente refusée : référence ou quantité invalide"); return; }
    if (q > p->stock) { printf("  vente refusée : stock %d\n", p->stock); return; }
    p->stock -= q; p->vendus += q;
    printf("  --- TICKET N°%04d ---\n  %d × %-20s %8ld FCFA\n  TOTAL %26ld FCFA\n", ++tickets, q, p->nom, p->prix * q, p->prix * q);
    if (p->stock <= 2) printf("  ⚠ stock bas pour %s (%d)\n", p->nom, p->stock);
}

static void cmd_stock(void) {
    for (int i = 0; i < nb; i++) printf("  %-6s %-20s %7ld FCFA  stock %3d\n", cat[i].ref, cat[i].nom, cat[i].prix, cat[i].stock);
}

static void cmd_rapport(void) {
    Produit copie[MAX];
    memcpy(copie, cat, sizeof(Produit) * (size_t)nb);
    qsort(copie, (size_t)nb, sizeof copie[0], par_ca);
    long total = 0;
    for (int i = 0; i < nb; i++) {
        long ca = copie[i].prix * copie[i].vendus;
        total += ca;
        printf("  %d. %-20s %3ld vendus  %9ld FCFA\n", i + 1, copie[i].nom, copie[i].vendus, ca);
    }
    printf("  Chiffre d'affaires : %ld FCFA, %d ticket(s)\n", total, tickets);
}

static void cmd_sauver(const char *fichier) {
    FILE *f = fichier ? fopen(fichier, "w") : NULL;
    if (!f) { puts("  impossible d'écrire le fichier"); return; }
    for (int i = 0; i < nb; i++) fprintf(f, "%s;%s;%ld;%d;%ld\n", cat[i].ref, cat[i].nom, cat[i].prix, cat[i].stock, cat[i].vendus);
    fclose(f);
    printf("  %d produit(s) sauvegardé(s)\n", nb);
}

static void cmd_charger(const char *fichier) {
    FILE *f = fichier ? fopen(fichier, "r") : NULL;
    if (!f) { puts("  fichier introuvable"); return; }
    nb = 0;
    while (nb < MAX && fscanf(f, "%11[^;];%39[^;];%ld;%d;%ld\n", cat[nb].ref, cat[nb].nom, &cat[nb].prix,
                              &cat[nb].stock, &cat[nb].vendus) == 5) nb++;
    fclose(f);
    qsort(cat, (size_t)nb, sizeof cat[0], par_ref);
    printf("  %d produit(s) chargé(s)\n", nb);
}

static void executer(char *ligne) {
    char *etat, *mot = strtok_r(ligne, " \t\n", &etat);
    if (!mot || mot[0] == '#') return;
    char *a = strtok_r(NULL, " \t\n", &etat), *b = strtok_r(NULL, " \t\n", &etat);
    char *c = strtok_r(NULL, " \t\n", &etat), *d = strtok_r(NULL, " \t\n", &etat);
    printf("> %s %s %s %s %s\n", mot, a ? a : "", b ? b : "", c ? c : "", d ? d : "");
    if (!strcmp(mot, "produit")) cmd_produit(a, b, c, d);
    else if (!strcmp(mot, "vendre")) cmd_vendre(a, b);
    else if (!strcmp(mot, "stock")) cmd_stock();
    else if (!strcmp(mot, "rapport")) cmd_rapport();
    else if (!strcmp(mot, "sauver")) cmd_sauver(a);
    else if (!strcmp(mot, "charger")) cmd_charger(a);
    else printf("  commande inconnue : %s\n", mot);
}

int main(int argc, char *argv[]) {
    char ligne[256];
    if (argc == 2 && !strcmp(argv[1], "-")) {             /* mode interactif : lit l'entrée standard */
        while (fgets(ligne, sizeof ligne, stdin)) executer(ligne);
        return 0;
    }
    if (argc > 1) { fprintf(stderr, "Usage : %s [-]   (« - » pour lire des commandes au clavier)\n", argv[0]); return 1; }
    char fichier[] = "/tmp/boutique_demo.csv";
    const char *script[] = {
        "produit ECO01 Écouteurs 4500 10", "produit CHG02 Chargeur 2500 20", "produit MON03 Montre 12000 3",
        "produit LMP04 Lampe_solaire 6000 5", "vendre ECO01 3", "vendre MON03 2", "vendre MON03 5",
        "vendre CHG02 4", "vendre XYZ 1", "stock", "rapport", "sauver /tmp/boutique_demo.csv",
        "charger /tmp/boutique_demo.csv", "inventer"};
    for (size_t i = 0; i < sizeof script / sizeof script[0]; i++) {
        snprintf(ligne, sizeof ligne, "%s", script[i]);
        executer(ligne);
    }
    remove(fichier);
    return 0;
}
