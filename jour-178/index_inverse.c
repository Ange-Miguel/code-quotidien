/*
 * Jour 178 — Moteur de recherche plein texte sur des documents (index inversé)
 *
 * Problème : chercher un mot dans des centaines de cours, de fiches produits
 * ou d'articles en relisant chaque document à chaque recherche est trop lent.
 * Les moteurs de recherche utilisent un index inversé.
 * Solution : on construit une fois l'index : pour chaque mot (en minuscules,
 * sans les mots vides comme « le », « de »), la liste des documents où il
 * apparaît, avec le nombre d'occurrences. Une recherche à plusieurs mots
 * intersecte ces listes (tous les mots doivent être présents), puis classe
 * les résultats par score TF-IDF : un mot fréquent dans le document mais rare
 * dans la collection compte beaucoup. Les documents sont de petites fiches
 * d'exemple.
 * On apprend : l'index inversé, la tokenisation, les mots vides, le score
 * TF-IDF avec log, et l'intersection de listes.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NB_DOCS 6
#define MAX_MOTS 512
#define MAX_TERME 24

static const char *DOCS[NB_DOCS][2] = {
    {"Cours : capteurs de température", "Le capteur LM35 mesure la température. La température est convertie par l'ADC du microcontrôleur."},
    {"Cours : protocole MQTT", "MQTT est un protocole de messagerie pour l'IoT : un capteur publie, une application s'abonne."},
    {"Fiche : ESP32", "L'ESP32 est un microcontrôleur avec Wi-Fi et Bluetooth, idéal pour l'IoT et les capteurs connectés."},
    {"Cours : PWM et moteurs", "Le PWM règle la vitesse d'un moteur. Le microcontrôleur génère le PWM avec un timer."},
    {"Article : énergie solaire", "Un kit solaire alimente un capteur IoT isolé ; la batterie doit tenir la nuit."},
    {"Fiche : Arduino Uno", "L'Arduino Uno utilise un microcontrôleur AVR. On y branche facilement un capteur de température."},
};
static const char *MOTS_VIDES[] = {"le", "la", "les", "un", "une", "de", "des", "du", "et", "est", "pour", "par", "avec", "l", "d", "s", "y", "on", "a", "doit"};

typedef struct { char terme[MAX_TERME]; int occurrences[NB_DOCS]; int nb_docs; } Entree;
static Entree index_[MAX_MOTS];
static int nb_termes;

static int mot_vide(const char *m) {
    for (size_t i = 0; i < sizeof MOTS_VIDES / sizeof MOTS_VIDES[0]; i++) if (!strcmp(m, MOTS_VIDES[i])) return 1;
    return 0;
}

static Entree *trouver(const char *terme, int creer) {
    for (int i = 0; i < nb_termes; i++) if (!strcmp(index_[i].terme, terme)) return &index_[i];
    if (!creer || nb_termes == MAX_MOTS) return NULL;
    size_t k = 0;
    for (; terme[k] && k < MAX_TERME - 1; k++) index_[nb_termes].terme[k] = terme[k];
    index_[nb_termes].terme[k] = '\0';
    return &index_[nb_termes++];
}

/* Découpe un texte en termes ASCII minuscules (les octets accentués UTF-8 sont conservés tels quels). */
static int termes(const char *texte, char sortie[][MAX_TERME], int max) {
    int n = 0, l = 0;
    char courant[MAX_TERME];
    for (const char *p = texte;; p++) {
        unsigned char c = (unsigned char)*p;
        if (c && (isalnum(c) || c >= 0x80)) { if (l < MAX_TERME - 1) courant[l++] = (char)tolower(c); continue; }
        if (l) { courant[l] = '\0'; if (!mot_vide(courant) && n < max) memcpy(sortie[n++], courant, (size_t)l + 1); l = 0; }
        if (!c) break;
    }
    return n;
}

static void indexer(void) {
    char t[64][MAX_TERME];
    for (int d = 0; d < NB_DOCS; d++) {
        int n = termes(DOCS[d][1], t, 64);
        for (int i = 0; i < n; i++) {
            Entree *e = trouver(t[i], 1);
            if (e && e->occurrences[d]++ == 0) e->nb_docs++;
        }
    }
}

static void rechercher(const char *requete) {
    char t[8][MAX_TERME];
    int n = termes(requete, t, 8);
    double score[NB_DOCS];
    int present[NB_DOCS];
    for (int d = 0; d < NB_DOCS; d++) { score[d] = 0; present[d] = 1; }
    for (int i = 0; i < n; i++) {
        Entree *e = trouver(t[i], 0);
        for (int d = 0; d < NB_DOCS; d++) {
            if (!e || !e->occurrences[d]) { present[d] = 0; continue; }
            score[d] += (1 + log(e->occurrences[d])) * log((double)NB_DOCS / e->nb_docs);   /* TF-IDF */
        }
    }
    printf("Recherche « %s » :\n", requete);
    int trouves = 0;
    for (int k = 0; k < NB_DOCS; k++) {               /* affichage par score décroissant (sélection simple) */
        int best = -1;
        for (int d = 0; d < NB_DOCS; d++) if (present[d] && n && (best < 0 || score[d] > score[best])) best = d;
        if (best < 0) break;
        printf("   %.2f  %s\n", score[best], DOCS[best][0]);
        present[best] = 0;
        trouves++;
    }
    if (!trouves) printf("   aucun document ne contient tous ces mots\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    indexer();
    printf("Index construit : %d termes distincts dans %d documents\n\n", nb_termes, NB_DOCS);
    if (argc > 1) { for (int i = 1; i < argc; i++) rechercher(argv[i]); return 0; }
    rechercher("capteur");
    rechercher("température capteur");
    rechercher("microcontrôleur");
    rechercher("IoT batterie");
    rechercher("blockchain");
    return 0;
}
