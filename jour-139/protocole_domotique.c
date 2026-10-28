/*
 * Jour 139 — Protocole de commande domotique texte (ON/OFF/STATUS)
 *
 * Problème : une carte domotique (Arduino, ESP32) reçoit des ordres par liaison
 * série, Bluetooth ou Wi-Fi. Un protocole texte simple, que l'on peut taper à
 * la main dans un terminal, facilite énormément les tests et le dépannage :
 * « ON LAMPE1 », « DIM SALON 40 », « STATUS ».
 * Solution : chaque ligne est découpée en mots ; le premier est la commande,
 * cherchée dans une table (nom, nombre d'arguments, fonction). Chaque commande
 * vérifie ses arguments et répond toujours par « OK … » ou « ERR code
 * message », ce qui permet à un programme de lire la réponse facilement.
 * Les sorties (relais, variateur) sont simulées dans un tableau d'état. Une
 * commande HELP liste les commandes disponibles.
 * On apprend : la conception d'un protocole texte, les tables de dispatch
 * (pointeurs de fonction), la validation des arguments et les codes d'erreur.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

typedef struct { const char *nom; int relais; int niveau; } Sortie;
static Sortie sorties[] = {{"LAMPE1", 0, 0}, {"LAMPE2", 0, 0}, {"SALON", 0, 0}, {"POMPE", 0, 0}, {"PORTAIL", 0, 0}};
#define NB_SORTIES (int)(sizeof sorties / sizeof sorties[0])

static Sortie *trouver(const char *nom) {
    for (int i = 0; i < NB_SORTIES; i++) if (!strcasecmp(sorties[i].nom, nom)) return &sorties[i];
    return NULL;
}

static void cmd_on(char **a)  { Sortie *s = trouver(a[0]); if (!s) { printf("ERR 404 sortie inconnue\n"); return; } s->relais = 1; s->niveau = 100; printf("OK %s=ON\n", s->nom); }
static void cmd_off(char **a) { Sortie *s = trouver(a[0]); if (!s) { printf("ERR 404 sortie inconnue\n"); return; } s->relais = 0; s->niveau = 0; printf("OK %s=OFF\n", s->nom); }
static void cmd_dim(char **a) {
    Sortie *s = trouver(a[0]);
    char *fin;
    long n = strtol(a[1], &fin, 10);
    if (!s) { printf("ERR 404 sortie inconnue\n"); return; }
    if (*fin || n < 0 || n > 100) { printf("ERR 422 niveau 0-100 attendu\n"); return; }
    s->niveau = (int)n; s->relais = n > 0;
    printf("OK %s=%ld%%\n", s->nom, n);
}
static void cmd_status(char **a) {
    (void)a;
    printf("OK");
    for (int i = 0; i < NB_SORTIES; i++) printf(" %s=%s", sorties[i].nom, sorties[i].relais ? (sorties[i].niveau < 100 ? "DIM" : "ON") : "OFF");
    printf("\n");
}
static void cmd_help(char **a);

typedef struct { const char *nom; int nb_args; void (*f)(char **); const char *aide; } Commande;
static const Commande COMMANDES[] = {
    {"ON", 1, cmd_on, "ON <sortie>"}, {"OFF", 1, cmd_off, "OFF <sortie>"}, {"DIM", 2, cmd_dim, "DIM <sortie> <0-100>"},
    {"STATUS", 0, cmd_status, "STATUS"}, {"HELP", 0, cmd_help, "HELP"},
};
#define NB_CMD (int)(sizeof COMMANDES / sizeof COMMANDES[0])

static void cmd_help(char **a) { (void)a; printf("OK"); for (int i = 0; i < NB_CMD; i++) printf(" | %s", COMMANDES[i].aide); printf("\n"); }

static void traiter(char *ligne) {
    char *mots[4];
    int n = 0;
    for (char *m = strtok(ligne, " \t\r\n"); m && n < 4; m = strtok(NULL, " \t\r\n")) mots[n++] = m;
    if (n == 0) return;
    for (int i = 0; i < NB_CMD; i++)
        if (!strcasecmp(mots[0], COMMANDES[i].nom)) {
            if (n - 1 != COMMANDES[i].nb_args) { printf("ERR 400 usage : %s\n", COMMANDES[i].aide); return; }
            COMMANDES[i].f(mots + 1);
            return;
        }
    printf("ERR 405 commande inconnue (HELP pour la liste)\n");
}

int main(int argc, char *argv[]) {
    char ligne[128];
    if (argc == 2 && !strcmp(argv[1], "-")) {        /* mode terminal : tapez les commandes */
        while (printf("> "), fflush(stdout), fgets(ligne, sizeof ligne, stdin)) traiter(ligne);
        return 0;
    }
    if (argc > 1) { fprintf(stderr, "Usage : %s [-]   (« - » : saisie des commandes au clavier)\n", argv[0]); return 1; }
    const char *script[] = {"HELP", "ON lampe1", "DIM salon 40", "dim SALON 140", "ON GARAGE", "OFF", "ALLUMER LAMPE2",
                            "on pompe", "STATUS", "OFF POMPE", "STATUS"};
    for (size_t i = 0; i < sizeof script / sizeof script[0]; i++) {
        snprintf(ligne, sizeof ligne, "%s", script[i]);
        printf("> %s\n", script[i]);
        traiter(ligne);
    }
    return 0;
}
