/*
 * Jour 133 — Simulation d'un client MQTT : publier/s'abonner (en mémoire)
 *
 * Problème : en IoT, des dizaines de capteurs et d'applications échangent des
 * messages sans se connaître directement. MQTT, le protocole le plus utilisé,
 * repose sur le modèle « publier / s'abonner » : un capteur publie sur un
 * sujet (topic) comme maison/salon/temperature, et tous les abonnés à ce
 * sujet reçoivent le message.
 * Solution : on simule en mémoire, en C, le cœur du modèle : une table
 * d'abonnements (sujet → fonction de rappel), une file de messages, la
 * publication qui distribue aux abonnés concernés, le désabonnement et les
 * messages « retenus » (retain) livrés immédiatement aux nouveaux abonnés.
 * Le vrai réseau sera ajouté plus tard ; ici on comprend le principe.
 * On apprend : le modèle publier/s'abonner, les pointeurs de fonction, les
 * tables de structures avec chaînes de caractères et le découplage entre
 * producteurs et consommateurs de données.
 */
#include <stdio.h>
#include <string.h>

#define MAX_ABONNEMENTS 16
#define MAX_RETENUS 8

typedef void (*Rappel)(const char *client, const char *sujet, const char *message);

typedef struct { int actif; char client[24]; char sujet[64]; Rappel rappel; } Abonnement;
typedef struct { char sujet[64]; char message[64]; } Retenu;

static Abonnement abonnements[MAX_ABONNEMENTS];
static Retenu retenus[MAX_RETENUS];
static int nb_retenus, livres;

static void livrer(Abonnement *a, const char *sujet, const char *msg) { a->rappel(a->client, sujet, msg); livres++; }

static int abonner(const char *client, const char *sujet, Rappel r) {
    for (int i = 0; i < MAX_ABONNEMENTS; i++)
        if (!abonnements[i].actif) {
            abonnements[i].actif = 1;
            snprintf(abonnements[i].client, sizeof abonnements[i].client, "%s", client);
            snprintf(abonnements[i].sujet, sizeof abonnements[i].sujet, "%s", sujet);
            abonnements[i].rappel = r;
            for (int k = 0; k < nb_retenus; k++)          /* message retenu : livré tout de suite */
                if (!strcmp(retenus[k].sujet, sujet)) livrer(&abonnements[i], sujet, retenus[k].message);
            return i;
        }
    return -1;
}

static void desabonner(const char *client, const char *sujet) {
    for (int i = 0; i < MAX_ABONNEMENTS; i++)
        if (abonnements[i].actif && !strcmp(abonnements[i].client, client) && !strcmp(abonnements[i].sujet, sujet))
            abonnements[i].actif = 0;
}

static int publier(const char *sujet, const char *message, int retenir) {
    printf("PUBLICATION %-28s « %s »%s\n", sujet, message, retenir ? " [retenu]" : "");
    if (retenir) {
        int k = 0;
        while (k < nb_retenus && strcmp(retenus[k].sujet, sujet)) k++;
        if (k == nb_retenus && nb_retenus < MAX_RETENUS) nb_retenus++;
        if (k < MAX_RETENUS) {
            snprintf(retenus[k].sujet, sizeof retenus[k].sujet, "%s", sujet);
            snprintf(retenus[k].message, sizeof retenus[k].message, "%s", message);
        }
    }
    int n = 0;
    for (int i = 0; i < MAX_ABONNEMENTS; i++)
        if (abonnements[i].actif && !strcmp(abonnements[i].sujet, sujet)) { livrer(&abonnements[i], sujet, message); n++; }
    if (!n) printf("   (aucun abonné)\n");
    return n;
}

static void appli_mobile(const char *c, const char *s, const char *m) { printf("   → [%s] affiche %s = %s\n", c, s, m); }
static void climatiseur(const char *c, const char *s, const char *m) {
    double t;
    if (sscanf(m, "%lf", &t) == 1) printf("   → [%s] %s : %s\n", c, s, t > 27 ? "démarrage du refroidissement" : "rien à faire");
}
static void historique(const char *c, const char *s, const char *m) { printf("   → [%s] enregistre (%s, %s)\n", c, s, m); }

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    abonner("appli-mobile", "maison/salon/temperature", appli_mobile);
    abonner("climatiseur", "maison/salon/temperature", climatiseur);
    abonner("historique", "maison/salon/temperature", historique);
    abonner("appli-mobile", "maison/portail/etat", appli_mobile);

    publier("maison/salon/temperature", "26.4", 0);
    publier("maison/salon/temperature", "28.1", 1);
    publier("maison/cuisine/gaz", "0.02", 0);
    publier("maison/portail/etat", "fermé", 1);
    puts("\nL'historique se désabonne ; une tablette se connecte plus tard :");
    desabonner("historique", "maison/salon/temperature");
    abonner("tablette", "maison/portail/etat", appli_mobile);          /* reçoit l'état retenu */
    publier("maison/salon/temperature", "25.9", 0);
    printf("\nMessages livrés au total : %d\n", livres);
    return 0;
}
