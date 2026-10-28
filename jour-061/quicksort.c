/*
 * Jour 061 — Tri rapide (quicksort) sur des prix de produits
 *
 * Problème : trier des milliers de prix (catalogue, comparateur) avec un tri
 * en O(n²) devient vite trop lent. Le tri rapide fait en moyenne O(n log n).
 * Solution : on choisit un pivot (médiane de trois pour éviter le pire cas sur
 * des données déjà triées), on partitionne le tableau (schéma de Hoare) en
 * plus petits / plus grands que le pivot, puis on trie récursivement chaque
 * partie, en récursant d'abord sur la plus petite pour limiter la pile. On
 * compare le temps avec le qsort de la bibliothèque standard.
 * On apprend : la stratégie « diviser pour régner », la partition de Hoare,
 * le choix du pivot, la profondeur de récursion et la mesure de temps.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void echanger(long *a, long *b) { long t = *a; *a = *b; *b = t; }

static long mediane_de_trois(long *t, long g, long d) {
    long m = g + (d - g) / 2;
    if (t[m] < t[g]) echanger(&t[m], &t[g]);
    if (t[d] < t[g]) echanger(&t[d], &t[g]);
    if (t[d] < t[m]) echanger(&t[d], &t[m]);
    return t[m];
}

static void tri_rapide(long *t, long g, long d) {
    while (g < d) {
        long pivot = mediane_de_trois(t, g, d);
        long i = g - 1, j = d + 1;
        for (;;) {                                  /* partition de Hoare */
            do i++; while (t[i] < pivot);
            do j--; while (t[j] > pivot);
            if (i >= j) break;
            echanger(&t[i], &t[j]);
        }
        if (j - g < d - j) { tri_rapide(t, g, j); g = j + 1; }   /* plus petite partie d'abord */
        else { tri_rapide(t, j + 1, d); d = j; }
    }
}

static int comparer(const void *a, const void *b) {
    long x = *(const long *)a, y = *(const long *)b;
    return (x > y) - (x < y);
}

static double secondes(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char *argv[]) {
    long n = 1000000;
    if (argc == 2) {
        char *fin;
        n = strtol(argv[1], &fin, 10);
        if (*fin || n < 10 || n > 50000000) {
            fprintf(stderr, "Usage : %s [nombre de prix 10..50000000]\n", argv[0]);
            return 1;
        }
    }
    long *a = malloc((size_t)n * sizeof *a), *b = malloc((size_t)n * sizeof *b);
    if (!a || !b) { fprintf(stderr, "Mémoire insuffisante\n"); return 2; }
    srand(61);
    for (long i = 0; i < n; i++) a[i] = b[i] = 100 + (rand() % 200000) * 5L;   /* prix en FCFA */

    double t0 = secondes();
    tri_rapide(a, 0, n - 1);
    double t1 = secondes();
    qsort(b, (size_t)n, sizeof *b, comparer);
    double t2 = secondes();

    for (long i = 0; i < n; i++)
        if (a[i] != b[i]) { fprintf(stderr, "Erreur de tri à l'indice %ld\n", i); return 3; }
    printf("%ld prix triés\n", n);
    printf("  Notre tri rapide : %.3f s\n  qsort standard   : %.3f s\n", t1 - t0, t2 - t1);
    printf("  Moins cher : %ld FCFA | plus cher : %ld FCFA | médian : %ld FCFA\n", a[0], a[n - 1], a[n / 2]);

    t0 = secondes();
    tri_rapide(a, 0, n - 1);                    /* données déjà triées : pas de pire cas */
    printf("  Tri d'un tableau déjà trié : %.3f s (grâce à la médiane de trois)\n", secondes() - t0);
    free(a);
    free(b);
    return 0;
}
