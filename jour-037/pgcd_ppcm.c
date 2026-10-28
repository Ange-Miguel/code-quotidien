/*
 * Jour 037 — PGCD et PPCM (partage équitable de lots)
 *
 * Problème : un grossiste a 84 savons et 120 bouteilles d'huile ; il veut faire
 * le plus grand nombre de lots identiques sans rien laisser. Deux bus partent
 * toutes les 12 et 18 minutes : quand repartent-ils ensemble ?
 * Solution : le premier problème est un PGCD (algorithme d'Euclide :
 * pgcd(a, b) = pgcd(b, a mod b)), le second un PPCM (a × b / pgcd). On
 * généralise à plusieurs nombres et on affiche les étapes d'Euclide.
 * On apprend : l'algorithme d'Euclide (récursif et itératif), le lien entre
 * PGCD et PPCM, et le risque de dépassement de capacité des entiers.
 */
#include <stdio.h>
#include <stdlib.h>

static long long pgcd(long long a, long long b) {
    while (b) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static long long pgcd_etapes(long long a, long long b) {
    printf("  Euclide : ");
    while (b) {
        printf("pgcd(%lld, %lld) → ", a, b);
        long long r = a % b;
        a = b;
        b = r;
    }
    printf("%lld\n", a);
    return a;
}

static long long ppcm(long long a, long long b) {
    return a / pgcd(a, b) * b;   /* diviser d'abord limite le dépassement */
}

static void lots(long long a, const char *objet_a, long long b, const char *objet_b) {
    long long g = pgcd_etapes(a, b);
    printf("  %lld %s et %lld %s → %lld lots de %lld %s + %lld %s\n\n",
           a, objet_a, b, objet_b, g, a / g, objet_a, b / g, objet_b);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        puts("Lots identiques sans reste :");
        lots(84, "savons", 120, "bouteilles d'huile");
        lots(252, "cahiers", 198, "stylos");
        puts("Départs simultanés de bus (toutes les 12, 18 et 30 min) :");
        long long p = ppcm(ppcm(12, 18), 30);
        printf("  ils repartent ensemble toutes les %lld min (%lld h %02lld)\n", p, p / 60, p % 60);
        return 0;
    }
    if (argc < 3) {
        fprintf(stderr, "Usage : %s n1 n2 [n3 ...]\n", argv[0]);
        return 1;
    }
    long long g = 0, m = 1;
    for (int i = 1; i < argc; i++) {
        char *fin;
        long long v = strtoll(argv[i], &fin, 10);
        if (*fin || v <= 0 || v > 1000000000LL) {
            fprintf(stderr, "Valeur invalide : %s (entier de 1 à 10^9)\n", argv[i]);
            return 1;
        }
        g = pgcd(g, v);
        m = ppcm(m, v);
        if (m > 1000000000000000LL) { fprintf(stderr, "PPCM trop grand\n"); return 1; }
    }
    printf("PGCD = %lld | PPCM = %lld\n", g, m);
    return 0;
}
