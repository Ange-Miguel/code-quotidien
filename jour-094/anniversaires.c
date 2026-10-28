/*
 * Jour 094 — Rappel d'anniversaires à partir d'un fichier
 *
 * Problème : on oublie l'anniversaire d'un ami, d'un parent ou d'un client
 * fidèle. Un petit programme lancé chaque matin pourrait prévenir à l'avance.
 * Solution : un fichier texte contient une ligne par personne :
 * « JJ/MM/AAAA;Nom ». On lit le fichier, on calcule pour chacun le nombre de
 * jours avant le prochain anniversaire (en tenant compte du passage d'année
 * et du 29 février) et l'âge qu'il aura. On trie par proximité et on affiche
 * ceux des 30 prochains jours. La date du jour peut être imposée pour tester.
 * On apprend : sscanf pour analyser des lignes, la structure tm et mktime de
 * la bibliothèque C pour les calculs de dates, et qsort sur des structures.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { char nom[48]; int j, m, a; int dans; int age; } Personne;

static time_t date_vers_temps(int j, int m, int a) {
    struct tm t = {0};
    t.tm_mday = j; t.tm_mon = m - 1; t.tm_year = a - 1900; t.tm_hour = 12;
    return mktime(&t);
}

static void calculer(Personne *p, int jj, int mm, int aa) {
    time_t aujourd_hui = date_vers_temps(jj, mm, aa);
    int annee = aa;
    for (;;) {
        int jour = p->j;
        if (p->m == 2 && p->j == 29 && !((annee % 4 == 0 && annee % 100) || annee % 400 == 0)) jour = 28;
        time_t prochain = date_vers_temps(jour, p->m, annee);
        if (prochain >= aujourd_hui) {
            p->dans = (int)((prochain - aujourd_hui) / 86400);
            p->age = annee - p->a;
            return;
        }
        annee++;
    }
}

static int comparer(const void *x, const void *y) {
    return ((const Personne *)x)->dans - ((const Personne *)y)->dans;
}

static int lire(FILE *f, Personne *liste, int max) {
    char ligne[128];
    int n = 0, numero = 0;
    while (n < max && fgets(ligne, sizeof ligne, f)) {
        numero++;
        Personne p;
        if (sscanf(ligne, "%d/%d/%d;%47[^\n]", &p.j, &p.m, &p.a, p.nom) != 4 || p.m < 1 || p.m > 12 ||
            p.j < 1 || p.j > 31 || p.a < 1900) {
            fprintf(stderr, "  ligne %d ignorée : %s", numero, ligne);
            continue;
        }
        liste[n++] = p;
    }
    return n;
}

int main(int argc, char *argv[]) {
    Personne liste[200];
    int n, jj = 28, mm = 10, aa = 2026;
    FILE *f;
    if (argc >= 2) {
        f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
        time_t t = time(NULL);
        struct tm *now = localtime(&t);
        jj = now->tm_mday; mm = now->tm_mon + 1; aa = now->tm_year + 1900;
    } else {
        f = tmpfile();
        if (!f) return 2;
        fputs("14/03/2005;Ange-Miguel\n02/11/1972;Maman\n29/02/2004;Kofi (né un 29 février)\n"
              "25/12/1999;Awa\n15/11/2001;Serge\n05/10/1998;Carine\nmauvaise ligne\n10/01/2003;Inès\n", f);
        rewind(f);
        printf("Démo avec la date du 28/10/2026\n");
    }
    n = lire(f, liste, 200);
    fclose(f);
    for (int i = 0; i < n; i++) calculer(&liste[i], jj, mm, aa);
    qsort(liste, (size_t)n, sizeof liste[0], comparer);
    printf("Anniversaires dans les 30 prochains jours :\n");
    for (int i = 0; i < n && liste[i].dans <= 30; i++)
        printf("  %-26s %02d/%02d  %s (%d ans)\n", liste[i].nom, liste[i].j, liste[i].m,
               liste[i].dans == 0 ? "AUJOURD'HUI !" : liste[i].dans == 1 ? "demain" : "bientôt", liste[i].age);
    printf("Prochains ensuite :\n");
    for (int i = 0; i < n; i++)
        if (liste[i].dans > 30) printf("  %-26s dans %3d jours\n", liste[i].nom, liste[i].dans);
    return 0;
}
