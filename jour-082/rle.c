/*
 * Jour 082 — Compression RLE d'un texte
 *
 * Problème : un capteur envoie de longues séries de valeurs identiques
 * (« 0000000011111000 ») et une liaison radio LoRa ou série coûte cher en
 * octets. Une compression très simple, peu gourmande en calcul, est utile
 * sur microcontrôleur.
 * Solution : le codage par plages (Run-Length Encoding) remplace chaque série
 * de caractères identiques par le nombre de répétitions suivi du caractère :
 * « AAAABBB » → « 4A3B ». On écrit l'encodeur, le décodeur, on vérifie
 * l'aller-retour et on mesure le taux de compression, y compris le cas
 * défavorable d'un texte sans répétitions (qui grossit !).
 * On apprend : le parcours de chaînes, l'écriture dans un tampon avec
 * snprintf, le décodage d'un format et la notion de taux de compression.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Encode src dans dst (taille max) ; retourne la longueur ou -1. */
static int encoder(const char *src, char *dst, size_t max) {
    size_t n = 0;
    for (size_t i = 0; src[i];) {
        size_t j = i;
        while (src[j] == src[i] && j - i < 255) j++;
        if (isdigit((unsigned char)src[i])) return -1;     /* les chiffres seraient ambigus */
        int ecrit = snprintf(dst + n, max - n, "%zu%c", j - i, src[i]);
        if (ecrit < 0 || (size_t)ecrit >= max - n) return -1;
        n += (size_t)ecrit;
        i = j;
    }
    return (int)n;
}

static int decoder(const char *src, char *dst, size_t max) {
    size_t n = 0;
    while (*src) {
        char *fin;
        long compte = strtol(src, &fin, 10);
        if (fin == src || compte <= 0 || !*fin) return -1;
        if (n + (size_t)compte >= max) return -1;
        memset(dst + n, *fin, (size_t)compte);
        n += (size_t)compte;
        src = fin + 1;
    }
    dst[n] = '\0';
    return (int)n;
}

static int tester(const char *texte) {
    char code[1024], retour[1024];
    int lc = encoder(texte, code, sizeof code);
    if (lc < 0) { printf("%-36.36s → impossible (chiffres ou trop long)\n", texte); return 0; }
    int lr = decoder(code, retour, sizeof retour);
    if (lr < 0 || strcmp(retour, texte) != 0) { printf("Erreur d'aller-retour !\n"); return 0; }
    size_t l = strlen(texte);
    printf("%-36.36s (%3zu) → %-28.28s (%3d) : %+.0f %%\n", texte, l, code, lc, 100.0 * (lc - (double)l) / l);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int ok = 1;
        for (int i = 1; i < argc; i++) ok &= tester(argv[i]);
        return ok ? 0 : 1;
    }
    tester("AAAAAAAAAABBBBBCCCCCCCCCCCCCCCDDDDDD");
    tester("________....____________....________");
    tester("BBBBBBBBNNNNNNNBBBBBBBBNNNNNNN");
    tester("Calavi");                     /* sans répétition : le texte grossit */
    tester("Le 3 mai");                   /* contient un chiffre */
    printf("\nAstuce : RLE est efficace sur des images en noir et blanc ou des mesures stables,\n"
           "inefficace sur du texte ordinaire.\n");
    return 0;
}
