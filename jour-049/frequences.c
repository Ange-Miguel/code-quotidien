/*
 * Jour 049 — Compteur de fréquence des lettres d'un texte
 *
 * Problème : quelles lettres reviennent le plus dans un texte français ? C'est
 * la base de l'analyse de fréquences qui permet de casser les chiffrements
 * simples (voir le jour 014) et de comprendre la compression de données.
 * Solution : on lit le texte depuis un fichier (ou un texte d'exemple), on
 * compte les 26 lettres dans un tableau indexé par c − 'a' (les lettres
 * accentuées UTF-8 courantes sont ramenées à leur base), puis on trie par
 * fréquence et on affiche un histogramme.
 * On apprend : la lecture de fichier caractère par caractère avec fgetc, les
 * tableaux de compteurs indexés, le tri d'indices et l'histogramme texte.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static const char *EXEMPLE =
    "Le Bénin est un pays d'Afrique de l'Ouest. Sa capitale officielle est Porto-Novo, "
    "mais Cotonou est la plus grande ville et le centre économique. Abomey-Calavi accueille "
    "la principale université du pays, où des milliers d'étudiants apprennent chaque année "
    "l'informatique, l'électronique et les systèmes embarqués.";

static long compte[26];
static long total;

/* Octet suivant un 0xC3 en UTF-8 : lettres latines accentuées → base. */
static int base_accent(unsigned char c) {
    const char *table = "aaaaaaaceeeeiiii";   /* à..ï : 0xA0..0xAF */
    if (c >= 0xA0 && c <= 0xAF) return table[c - 0xA0];
    if (c >= 0xB2 && c <= 0xB6) return 'o';
    if (c >= 0xB9 && c <= 0xBC) return 'u';
    if (c >= 0x80 && c <= 0x8F) return table[c - 0x80];   /* majuscules */
    return 0;
}

static void compter(int (*suivant)(void *), void *source) {
    int c, accent = 0;
    while ((c = suivant(source)) != EOF) {
        if (accent) { c = base_accent((unsigned char)c); accent = 0; }
        else if (c == 0xC3) { accent = 1; continue; }
        if (c && isalpha(c)) { compte[tolower(c) - 'a']++; total++; }
    }
}

static int depuis_fichier(void *f) { return fgetc((FILE *)f); }
static int depuis_chaine(void *p) {
    const unsigned char **s = p;
    return **s ? *(*s)++ : EOF;
}

static int comparer(const void *a, const void *b) {
    long x = compte[*(const int *)a], y = compte[*(const int *)b];
    return (y > x) - (y < x);
}

int main(int argc, char *argv[]) {
    if (argc > 2) { fprintf(stderr, "Usage : %s [fichier.txt]\n", argv[0]); return 1; }
    if (argc == 2) {
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
        compter(depuis_fichier, f);
        fclose(f);
    } else {
        const unsigned char *p = (const unsigned char *)EXEMPLE;
        compter(depuis_chaine, &p);
    }
    if (total == 0) { puts("Aucune lettre trouvée."); return 0; }
    int ordre[26];
    for (int i = 0; i < 26; i++) ordre[i] = i;
    qsort(ordre, 26, sizeof ordre[0], comparer);
    printf("%ld lettres analysées\n", total);
    for (int k = 0; k < 26 && compte[ordre[k]]; k++) {
        double pct = 100.0 * compte[ordre[k]] / total;
        printf("  %c %5.2f %% ", 'a' + ordre[k], pct);
        for (int j = 0; j < (int)(pct * 2); j++) putchar('#');
        putchar('\n');
    }
    return 0;
}
