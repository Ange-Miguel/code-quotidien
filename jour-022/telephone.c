/*
 * Jour 022 — Validateur de numéro de téléphone béninois
 *
 * Problème : dans un formulaire (livraison, inscription), les clients écrivent
 * leur numéro de mille façons : « 0190123456 », « +229 01 90 12 34 56 »,
 * « 00229-01.66.54.61.44 ». Il faut vérifier et normaliser.
 * Solution : on retire espaces, points, tirets et parenthèses ; on accepte les
 * préfixes +229 ou 00229 ; le numéro national doit alors faire 10 chiffres et
 * commencer par 01 (format en vigueur depuis la migration de 2024). On affiche
 * le numéro au format international lisible.
 * On apprend : le parcours de chaînes, isdigit, strncmp, la normalisation de
 * données saisies par des humains et les codes de retour d'erreur.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum { OK, CARACTERE_INVALIDE, MAUVAISE_LONGUEUR, MAUVAIS_PREFIXE } Resultat;

static const char *MESSAGES[] = {
    "valide", "caractère non autorisé", "longueur incorrecte (10 chiffres attendus)",
    "doit commencer par 01"
};

static Resultat normaliser(const char *saisie, char national[11]) {
    char chiffres[32];
    size_t n = 0;
    for (const char *p = saisie; *p; p++) {
        if (isdigit((unsigned char)*p)) {
            if (n >= sizeof chiffres - 1) return MAUVAISE_LONGUEUR;
            chiffres[n++] = *p;
        } else if (*p == '+' && p == saisie) {
            continue;
        } else if (!strchr(" .-()", *p)) {
            return CARACTERE_INVALIDE;
        }
    }
    chiffres[n] = '\0';
    const char *debut = chiffres;
    if (n == 15 && strncmp(chiffres, "00229", 5) == 0) debut += 5;
    else if (n == 13 && strncmp(chiffres, "229", 3) == 0) debut += 3;
    if (strlen(debut) != 10) return MAUVAISE_LONGUEUR;
    if (strncmp(debut, "01", 2) != 0) return MAUVAIS_PREFIXE;
    strcpy(national, debut);
    return OK;
}

static int verifier(const char *saisie) {
    char nat[11];
    Resultat r = normaliser(saisie, nat);
    printf("%-26s → ", saisie);
    if (r == OK)
        printf("+229 %.2s %.2s %.2s %.2s %.2s\n", nat, nat + 2, nat + 4, nat + 6, nat + 8);
    else
        printf("invalide : %s\n", MESSAGES[r]);
    return r == OK;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        const char *exemples[] = {"+229 01 90 12 34 56", "0190123456", "00229-01.97.00.00.00",
                                  "97 00 00 00", "+229 01 66 54 61 4A", "(229) 0161000000"};
        for (int i = 0; i < 6; i++) verifier(exemples[i]);
        return 0;
    }
    int tous_valides = 1;
    for (int i = 1; i < argc; i++) tous_valides &= verifier(argv[i]);
    return tous_valides ? 0 : 1;
}
