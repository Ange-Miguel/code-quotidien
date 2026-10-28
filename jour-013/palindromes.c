/*
 * Jour 013 — Détecteur de palindromes (mots et phrases)
 *
 * Problème : vérifier si un mot ou une phrase se lit de la même façon dans les
 * deux sens (« kayak », « Ésope reste ici et se repose »), en ignorant espaces,
 * ponctuation, majuscules et accents.
 * Solution : on normalise d'abord le texte : on garde uniquement les lettres et
 * chiffres, en minuscules, et on remplace les lettres accentuées UTF-8 les plus
 * courantes par leur lettre de base. Puis deux indices partent des deux bouts
 * et se rapprochent en comparant les caractères.
 * On apprend : le parcours avec deux pointeurs, l'encodage UTF-8 sur plusieurs
 * octets et la normalisation de texte en C.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Correspondance des lettres accentuées (2 octets UTF-8) vers l'ASCII. */
static const char *ACCENTS[][2] = {
    {"é", "e"}, {"è", "e"}, {"ê", "e"}, {"ë", "e"}, {"É", "e"}, {"È", "e"},
    {"à", "a"}, {"â", "a"}, {"À", "a"}, {"î", "i"}, {"ï", "i"}, {"ô", "o"},
    {"ù", "u"}, {"û", "u"}, {"ü", "u"}, {"ç", "c"}, {"Ç", "c"}
};
#define NB_ACCENTS (sizeof ACCENTS / sizeof ACCENTS[0])

static size_t normaliser(const char *src, char *dst, size_t taille) {
    size_t n = 0;
    while (*src && n + 1 < taille) {
        int remplace = 0;
        for (size_t k = 0; k < NB_ACCENTS; k++) {
            size_t l = strlen(ACCENTS[k][0]);
            if (strncmp(src, ACCENTS[k][0], l) == 0) {
                dst[n++] = ACCENTS[k][1][0];
                src += l;
                remplace = 1;
                break;
            }
        }
        if (remplace) continue;
        unsigned char c = (unsigned char)*src++;
        if (isalnum(c)) dst[n++] = (char)tolower(c);
    }
    dst[n] = '\0';
    return n;
}

static int est_palindrome(const char *texte) {
    char propre[512];
    size_t n = normaliser(texte, propre, sizeof propre);
    if (n == 0) return 0;
    for (size_t i = 0, j = n - 1; i < j; i++, j--)
        if (propre[i] != propre[j]) return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    const char *demo[] = {"kayak", "Ésope reste ici et se repose", "Engage le jeu que je le gagne",
                          "Calavi", "Été", "12321", "!!!"};
    const char **textes = argc > 1 ? (const char **)(argv + 1) : demo;
    int nb = argc > 1 ? argc - 1 : (int)(sizeof demo / sizeof demo[0]);
    for (int i = 0; i < nb; i++)
        printf("%-34s → %s\n", textes[i], est_palindrome(textes[i]) ? "palindrome" : "pas un palindrome");
    return 0;
}
