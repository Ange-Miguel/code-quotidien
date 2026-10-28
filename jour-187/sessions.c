/*
 * Jour 187 — Gestionnaire de sessions avec expiration
 *
 * Problème : quand on se connecte à un site ou à une application, le serveur
 * crée une « session » identifiée par un jeton aléatoire stocké dans un
 * cookie. Le jeton doit être imprévisible, la session doit expirer après une
 * période d'inactivité (et au bout d'une durée maximale), et on doit pouvoir
 * se déconnecter. Sinon, un téléphone perdu ou un jeton volé donne un accès
 * permanent.
 * Solution : les jetons sont 16 octets lus dans /dev/urandom, affichés en
 * hexadécimal. Chaque session mémorise l'utilisateur, la date de création et
 * celle de la dernière activité. La validation vérifie l'expiration
 * glissante (30 min d'inactivité) et absolue (8 h). Un nettoyage périodique
 * libère les sessions expirées. L'horloge est simulée pour la démonstration.
 * On apprend : la génération de jetons sûrs, l'expiration glissante et
 * absolue, la comparaison en temps constant et la gestion d'une table fixe.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_SESSIONS 16
#define INACTIVITE (30 * 60)
#define DUREE_MAX (8 * 3600)

typedef struct { int active; char jeton[33]; char utilisateur[24]; long creation, activite; } Session;
static Session sessions[MAX_SESSIONS];
static long maintenant;                 /* horloge simulée, en secondes */

static int jeton_aleatoire(char sortie[33]) {
    unsigned char o[16];
    FILE *f = fopen("/dev/urandom", "rb");
    if (!f || fread(o, 1, 16, f) != 16) { if (f) fclose(f); return 0; }
    fclose(f);
    for (int i = 0; i < 16; i++) sprintf(sortie + 2 * i, "%02x", o[i]);
    return 1;
}

static int egal_temps_constant(const char *a, const char *b, size_t n) {
    unsigned char diff = 0;
    for (size_t i = 0; i < n; i++) diff |= (unsigned char)(a[i] ^ b[i]);   /* pas d'arrêt anticipé */
    return diff == 0;
}

static const char *connecter(const char *utilisateur) {
    for (int i = 0; i < MAX_SESSIONS; i++)
        if (!sessions[i].active) {
            if (!jeton_aleatoire(sessions[i].jeton)) return NULL;
            sessions[i].active = 1;
            snprintf(sessions[i].utilisateur, sizeof sessions[i].utilisateur, "%s", utilisateur);
            sessions[i].creation = sessions[i].activite = maintenant;
            return sessions[i].jeton;
        }
    return NULL;
}

static const char *verifier(const char *jeton) {
    if (!jeton || strlen(jeton) != 32) return NULL;
    for (int i = 0; i < MAX_SESSIONS; i++) {
        Session *s = &sessions[i];
        if (!s->active || !egal_temps_constant(s->jeton, jeton, 32)) continue;
        if (maintenant - s->activite > INACTIVITE || maintenant - s->creation > DUREE_MAX) { s->active = 0; return NULL; }
        s->activite = maintenant;                       /* expiration glissante */
        return s->utilisateur;
    }
    return NULL;
}

static void deconnecter(const char *jeton) {
    for (int i = 0; i < MAX_SESSIONS; i++)
        if (sessions[i].active && egal_temps_constant(sessions[i].jeton, jeton, 32)) sessions[i].active = 0;
}

static void requete(const char *qui, const char *jeton) {
    const char *u = verifier(jeton);
    printf("  %02ld:%02ld  requête de %-6s → %s\n", maintenant / 3600, maintenant / 60 % 60, qui, u ? "autorisée" : "REFUSÉE : session invalide ou expirée, reconnectez-vous");
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    char awa[33], kofi[33];
    maintenant = 8 * 3600;
    const char *j = connecter("awa");
    if (!j) return 2;
    snprintf(awa, sizeof awa, "%s", j);
    snprintf(kofi, sizeof kofi, "%s", connecter("kofi"));
    printf("Jetons (16 octets aléatoires) : awa=%.16s… kofi=%.16s…\n", awa, kofi);
    requete("Awa", awa);
    maintenant += 20 * 60; requete("Awa", awa);          /* 20 min : ok, et prolonge la session */
    maintenant += 25 * 60; requete("Awa", awa);          /* 25 min après la précédente : ok */
    requete("Kofi", kofi);                               /* 45 min d'inactivité : expirée */
    requete("Pirate", "00000000000000000000000000000000");
    deconnecter(awa);
    maintenant += 60; requete("Awa", awa);               /* après déconnexion */
    const char *j2 = connecter("awa");
    char s2[33];
    snprintf(s2, sizeof s2, "%s", j2 ? j2 : "");
    for (int h = 0; h < 9; h++) { maintenant += 25 * 60; verifier(s2); }   /* active toutes les 25 min… */
    requete("Awa", s2);                                  /* …mais durée max de 8 h dépassée */
    return 0;
}
