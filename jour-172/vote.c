/*
 * Jour 172 — Vote électronique simple et dépouillement
 *
 * Problème : une association ou une promotion vote pour élire son bureau. Il
 * faut garantir qu'une personne ne vote qu'une fois, que les bulletins nuls
 * sont comptés à part, et produire un dépouillement clair. Avec plusieurs
 * candidats, le vote à un tour peut élire quelqu'un avec 35 % des voix.
 * Solution : chaque électeur inscrit a un code unique ; le bureau de vote
 * vérifie le code et le marque comme utilisé (sans lier le code au bulletin,
 * pour garder le secret du vote). Les bulletins sont des classements de
 * préférence. On dépouille au scrutin majoritaire simple, puis au vote à
 * second tour instantané (élimination successive du dernier et report des
 * voix selon les préférences), et on compare. Données fictives ; un vrai
 * scrutin exige bien plus de garanties (audit, chiffrement, anonymat).
 * On apprend : la vérification d'unicité, la séparation identité / bulletin,
 * deux méthodes de dépouillement et l'algorithme du vote alternatif.
 */
#include <stdio.h>
#include <string.h>

#define NB_CANDIDATS 4
#define MAX_BULLETINS 64
static const char *CANDIDATS[NB_CANDIDATS] = {"Awa", "Kofi", "Inès", "Serge"};

typedef struct { int pref[NB_CANDIDATS]; int nb; } Bulletin;   /* indices par ordre de préférence */
typedef struct { const char *code; int a_vote; } Electeur;

static Electeur inscrits[] = {{"K7Q2", 0}, {"M3ZX", 0}, {"P9LA", 0}, {"T4RD", 0}, {"B8NW", 0}, {"C2HY", 0}, {"D5VE", 0},
                              {"F6JS", 0}, {"G1UK", 0}, {"H3MO", 0}, {"J8PB", 0}, {"L2TC", 0}, {"N4WD", 0}, {"Q7XF", 0}};
#define NB_INSCRITS (int)(sizeof inscrits / sizeof inscrits[0])
static Bulletin urne[MAX_BULLETINS];
static int nb_urne, nuls;

static int voter(const char *code, const char *classement) {
    int e = -1;
    for (int i = 0; i < NB_INSCRITS; i++) if (!strcmp(inscrits[i].code, code)) e = i;
    if (e < 0) { printf("  code %s : inconnu, vote refusé\n", code); return 0; }
    if (inscrits[e].a_vote) { printf("  code %s : a déjà voté, vote refusé\n", code); return 0; }
    inscrits[e].a_vote = 1;                         /* on ne garde PAS le lien code ↔ bulletin */
    Bulletin b = {{0}, 0};
    int vu[NB_CANDIDATS] = {0};
    for (const char *p = classement; *p; p++) {
        int c = *p - '1';
        if (c < 0 || c >= NB_CANDIDATS || vu[c]) { nuls++; return 1; }   /* bulletin nul */
        vu[c] = 1;
        b.pref[b.nb++] = c;
    }
    if (b.nb == 0) { nuls++; return 1; }
    urne[nb_urne++] = b;
    return 1;
}

static void majoritaire(void) {
    int voix[NB_CANDIDATS] = {0};
    for (int i = 0; i < nb_urne; i++) voix[urne[i].pref[0]]++;
    printf("Scrutin majoritaire à un tour (premier choix seulement) :\n");
    int gagnant = 0;
    for (int c = 0; c < NB_CANDIDATS; c++) {
        printf("   %-6s %2d voix (%4.1f %%)\n", CANDIDATS[c], voix[c], 100.0 * voix[c] / nb_urne);
        if (voix[c] > voix[gagnant]) gagnant = c;
    }
    printf("   → élu(e) : %s avec %.0f %% des voix exprimées\n\n", CANDIDATS[gagnant], 100.0 * voix[gagnant] / nb_urne);
}

static void vote_alternatif(void) {
    int elimine[NB_CANDIDATS] = {0};
    printf("Vote alternatif (report des voix des éliminés) :\n");
    for (int tour = 1; tour < NB_CANDIDATS; tour++) {
        int voix[NB_CANDIDATS] = {0}, exprimes = 0;
        for (int i = 0; i < nb_urne; i++)
            for (int k = 0; k < urne[i].nb; k++)
                if (!elimine[urne[i].pref[k]]) { voix[urne[i].pref[k]]++; exprimes++; break; }
        printf("   tour %d :", tour);
        int dernier = -1, premier = -1;
        for (int c = 0; c < NB_CANDIDATS; c++) {
            if (elimine[c]) continue;
            printf(" %s %d", CANDIDATS[c], voix[c]);
            if (dernier < 0 || voix[c] < voix[dernier]) dernier = c;
            if (premier < 0 || voix[c] > voix[premier]) premier = c;
        }
        if (2 * voix[premier] > exprimes) { printf("\n   → élu(e) : %s avec la majorité absolue (%d/%d)\n", CANDIDATS[premier], voix[premier], exprimes); return; }
        printf("  → %s éliminé(e)\n", CANDIDATS[dernier]);
        elimine[dernier] = 1;
    }
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    /* Classements : "213" = Kofi puis Awa puis Inès */
    const char *votes[][2] = {{"K7Q2", "1"}, {"M3ZX", "12"}, {"P9LA", "14"}, {"T4RD", "1"}, {"B8NW", "1"},
                              {"C2HY", "234"}, {"D5VE", "23"}, {"F6JS", "32"}, {"G1UK", "324"}, {"H3MO", "42"},
                              {"J8PB", "43"}, {"L2TC", "11"}, {"K7Q2", "2"}, {"ZZZZ", "1"}, {"N4WD", "2"}, {"Q7XF", "34"}};
    puts("Bureau de vote :");
    for (size_t i = 0; i < sizeof votes / sizeof votes[0]; i++) voter(votes[i][0], votes[i][1]);
    int votants = 0;
    for (int i = 0; i < NB_INSCRITS; i++) votants += inscrits[i].a_vote;
    printf("  %d inscrits, %d votants (participation %.0f %%), %d exprimés, %d nul(s)\n\n", NB_INSCRITS, votants,
           100.0 * votants / NB_INSCRITS, nb_urne, nuls);
    majoritaire();
    vote_alternatif();
    return 0;
}
