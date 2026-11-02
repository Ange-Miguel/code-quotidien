/*
 * Jour 007 — Compte à rebours jusqu'à une date
 *
 * Problème : combien de jours reste-t-il avant les examens, une échéance de
 * paiement ou la livraison d'une commande ?
 * Solution : on convertit chaque date en « numéro de jour » absolu (nombre de
 * jours depuis une origine fixe) avec la formule des jours juliens ; la
 * différence donne le nombre de jours. On le convertit ensuite en semaines.
 * On apprend : les structures, la validation de dates, l'arithmétique entière
 * et la lecture d'une date au format AAAA-MM-JJ avec sscanf.
 */
#include <stdio.h>

typedef struct { int a, m, j; } Date;

static int bissextile(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }

static int date_valide(Date d) {
    static const int jours[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d.a < 1900 || d.a > 9999 || d.m < 1 || d.m > 12 || d.j < 1) return 0;
    int max = jours[d.m - 1] + (d.m == 2 && bissextile(d.a));
    return d.j <= max;
}

/* Numéro de jour julien (algorithme de Fliegel et Van Flandern). */
static long numero_jour(Date d) {
    long a = (14 - d.m) / 12, y = d.a + 4800 - a, m = d.m + 12 * a - 3;
    return d.j + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

static int lire_date(const char *s, Date *d) {
    char reste;
    return sscanf(s, "%d-%d-%d%c", &d->a, &d->m, &d->j, &reste) == 3 && date_valide(*d);
}

static void afficher(const char *evenement, Date depuis, Date vers) {
    long n = numero_jour(vers) - numero_jour(depuis);
    printf("%-22s %04d-%02d-%02d : ", evenement, vers.a, vers.m, vers.j);
    if (n > 0) printf("dans %ld jour(s), soit %ld semaine(s) et %ld jour(s)\n", n, n / 7, n % 7);
    else if (n == 0) printf("c'est aujourd'hui !\n");
    else printf("passé depuis %ld jour(s)\n", -n);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        Date ref = {2026, 10, 28};
        printf("Démo, à partir du 2026-10-28 :\n");
        afficher("Noël", ref, (Date){2026, 12, 25});
        afficher("Fête du Vodoun", ref, (Date){2027, 1, 10});
        afficher("Examens (exemple)", ref, (Date){2027, 2, 15});
        afficher("Fête de l'indépendance", ref, (Date){2026, 8, 1});
        return 0;
    }
    Date depuis, vers;
    if (argc != 3 || !lire_date(argv[1], &depuis) || !lire_date(argv[2], &vers)) {
        fprintf(stderr, "Usage : %s <date_du_jour AAAA-MM-JJ> <date_cible AAAA-MM-JJ>\n", argv[0]);
        return 1;
    }
    afficher("Date cible", depuis, vers);
    return 0;
}
