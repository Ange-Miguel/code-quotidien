/*
 * Jour 199 — Mini-base de données clé-valeur persistante
 *
 * Problème : beaucoup d'applications (configuration d'un appareil, compteur,
 * panier d'un utilisateur) ont juste besoin d'enregistrer des paires
 * clé → valeur et de les retrouver après un redémarrage, sans installer une
 * vraie base de données. Une coupure de courant pendant l'écriture ne doit
 * pas tout corrompre.
 * Solution : un journal en ajout seul (append-only log) : chaque écriture
 * (« SET clé valeur » ou « DEL clé ») est ajoutée à la fin du fichier avec une
 * somme de contrôle, puis fflush/fsync. Au démarrage, on rejoue le journal
 * pour reconstruire la table en mémoire ; une ligne finale abîmée (coupure)
 * est ignorée. Comme le fichier grossit, la compaction réécrit uniquement
 * l'état actuel dans un nouveau fichier, puis le renomme de façon atomique.
 * C'est le principe de Redis (AOF) ou de nombreux moteurs de stockage.
 * On apprend : la persistance par journal, la reprise après panne, les
 * sommes de contrôle, rename atomique et la compaction.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 128
typedef struct { char cle[32]; char valeur[64]; int present; } Paire;
static Paire table[MAX];
static char chemin[256];

static unsigned somme(const char *s) { unsigned h = 2166136261u; while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; } return h; }

static Paire *trouver(const char *cle, int creer) {
    for (int i = 0; i < MAX; i++) if (table[i].present && !strcmp(table[i].cle, cle)) return &table[i];
    if (!creer) return NULL;
    for (int i = 0; i < MAX; i++) if (!table[i].present) { table[i].present = 1; snprintf(table[i].cle, sizeof table[i].cle, "%s", cle); return &table[i]; }
    return NULL;
}

static void appliquer(const char *op, const char *cle, const char *valeur) {
    if (!strcmp(op, "SET")) { Paire *p = trouver(cle, 1); if (p) snprintf(p->valeur, sizeof p->valeur, "%s", valeur); }
    else { Paire *p = trouver(cle, 0); if (p) p->present = 0; }
}

static int ecrire(const char *op, const char *cle, const char *valeur) {
    if (strchr(cle, ' ') || strchr(cle, '\n') || strchr(valeur, '\n')) return 0;
    char ligne[128];
    snprintf(ligne, sizeof ligne, "%.3s %.31s %.63s", op, cle, valeur);
    FILE *f = fopen(chemin, "a");
    if (!f) return 0;
    fprintf(f, "%s|%08x\n", ligne, somme(ligne));
    fflush(f);
    fsync(fileno(f));                                   /* sur disque avant de répondre « ok » */
    fclose(f);
    appliquer(op, cle, valeur);
    return 1;
}

static int charger(void) {
    memset(table, 0, sizeof table);
    FILE *f = fopen(chemin, "r");
    if (!f) return 0;
    char l[160];
    int lignes = 0, rejetees = 0;
    while (fgets(l, sizeof l, f)) {
        char *barre = strrchr(l, '|');
        unsigned attendu;
        if (!barre || sscanf(barre + 1, "%8x", &attendu) != 1) { rejetees++; continue; }
        *barre = '\0';
        if (somme(l) != attendu) { rejetees++; continue; }
        char op[4], cle[32], valeur[64] = "";
        if (sscanf(l, "%3s %31s %63[^\n]", op, cle, valeur) < 2) { rejetees++; continue; }
        appliquer(op, cle, valeur);
        lignes++;
    }
    fclose(f);
    if (rejetees) printf("   (%d ligne(s) abîmée(s) ignorée(s) au chargement)\n", rejetees);
    return lignes;
}

static void compacter(void) {
    char tmp[300];
    snprintf(tmp, sizeof tmp, "%s.tmp", chemin);
    FILE *f = fopen(tmp, "w");
    if (!f) return;
    for (int i = 0; i < MAX; i++)
        if (table[i].present) { char l[128]; snprintf(l, sizeof l, "SET %.31s %.63s", table[i].cle, table[i].valeur); fprintf(f, "%s|%08x\n", l, somme(l)); }
    fflush(f); fsync(fileno(f)); fclose(f);
    rename(tmp, chemin);                                /* remplacement atomique */
}

static long taille(void) { FILE *f = fopen(chemin, "r"); if (!f) return 0; fseek(f, 0, SEEK_END); long t = ftell(f); fclose(f); return t; }
static void lire(const char *cle) { Paire *p = trouver(cle, 0); printf("   GET %-12s → %s\n", cle, p ? p->valeur : "(absent)"); }

int main(int argc, char *argv[]) {
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); (void)argv; return 1; }
    snprintf(chemin, sizeof chemin, "/tmp/kv_demo_%d.log", (int)getpid());
    remove(chemin);
    puts("Écritures :");
    ecrire("SET", "wifi_ssid", "Maison-Calavi");
    ecrire("SET", "seuil_temp", "30");
    for (int i = 1; i <= 50; i++) { char v[16]; snprintf(v, sizeof v, "%d", i * 10); ecrire("SET", "compteur", v); }
    ecrire("SET", "mode", "auto");
    ecrire("DEL", "mode", "");
    lire("compteur"); lire("mode");
    printf("   journal : %ld octets\n\nCoupure de courant pendant une écriture (ligne finale tronquée) :\n", taille());
    FILE *f = fopen(chemin, "a");
    if (f) { fputs("SET seuil_temp 3", f); fclose(f); }
    printf("Redémarrage : %d opérations rejouées\n", charger());
    lire("wifi_ssid"); lire("seuil_temp"); lire("compteur");
    long avant = taille();
    compacter();
    printf("\nCompaction : %ld → %ld octets, puis relecture : %d entrées\n", avant, taille(), charger());
    lire("compteur");
    remove(chemin);
    return 0;
}
