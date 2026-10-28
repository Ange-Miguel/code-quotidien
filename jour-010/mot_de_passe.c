/*
 * Jour 010 — Générateur de mot de passe aléatoire robuste
 *
 * Problème : on réutilise souvent le même mot de passe faible partout. Il faut
 * en créer de longs et variés pour chaque compte (e-mail, banque, GitHub).
 * Solution : on lit des octets aléatoires de qualité dans /dev/urandom et on
 * les transforme en caractères pris dans quatre familles (minuscules,
 * majuscules, chiffres, symboles). On garantit au moins un caractère de chaque
 * famille, puis on mélange avec l'algorithme de Fisher-Yates. Le rejet des
 * valeurs trop grandes évite un biais statistique.
 * On apprend : la lecture de fichiers spéciaux, le biais du modulo, le mélange
 * de Fisher-Yates et la manipulation de chaînes en C.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *FAMILLES[] = {
    "abcdefghijkmnopqrstuvwxyz",    /* sans l pour éviter la confusion avec 1 */
    "ABCDEFGHJKLMNPQRSTUVWXYZ",     /* sans I et O */
    "23456789",                     /* sans 0 et 1 */
    "!@#$%&*?-_+="
};
#define NB_FAMILLES 4

static FILE *source;

/* Entier aléatoire uniforme dans [0, n) sans biais du modulo. */
static unsigned aleatoire(unsigned n) {
    unsigned char octet;
    unsigned limite = 256 - (256 % n);
    do {
        if (fread(&octet, 1, 1, source) != 1) { perror("urandom"); exit(2); }
    } while (octet >= limite);
    return octet % n;
}

static void generer(char *mdp, int longueur) {
    char tous[128] = "";
    for (int f = 0; f < NB_FAMILLES; f++) strcat(tous, FAMILLES[f]);
    for (int i = 0; i < longueur; i++) {
        const char *jeu = i < NB_FAMILLES ? FAMILLES[i] : tous; /* une de chaque famille d'abord */
        mdp[i] = jeu[aleatoire((unsigned)strlen(jeu))];
    }
    mdp[longueur] = '\0';
    for (int i = longueur - 1; i > 0; i--) {           /* mélange de Fisher-Yates */
        int j = (int)aleatoire((unsigned)i + 1);
        char t = mdp[i]; mdp[i] = mdp[j]; mdp[j] = t;
    }
}

int main(int argc, char *argv[]) {
    int longueur = 16, nombre = argc == 1 ? 5 : 1;
    if (argc >= 2) {
        char *fin;
        long l = strtol(argv[1], &fin, 10);
        if (*fin != '\0' || l < 8 || l > 64) {
            fprintf(stderr, "Longueur invalide (8 à 64). Usage : %s [longueur]\n", argv[0]);
            return 1;
        }
        longueur = (int)l;
    }
    source = fopen("/dev/urandom", "rb");
    if (!source) { perror("/dev/urandom"); return 2; }
    char mdp[65];
    printf("%d mot(s) de passe de %d caractères :\n", nombre, longueur);
    for (int k = 0; k < nombre; k++) {
        generer(mdp, longueur);
        printf("  %s\n", mdp);
    }
    fclose(source);
    return 0;
}
