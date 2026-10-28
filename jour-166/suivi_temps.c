/*
 * Jour 166 — Suivi du temps passé par projet freelance
 *
 * Problème : un freelance qui jongle entre plusieurs clients ne sait pas
 * combien de temps il a vraiment passé sur chaque projet. Résultat : il sous-
 * estime ses devis et certains projets « au forfait » deviennent non rentables.
 * Solution : un journal de sessions de travail (projet, début, fin) est lu
 * depuis un fichier au format « AAAA-MM-JJ HH:MM HH:MM projet ». Le programme
 * cumule le temps par projet (en gérant les sessions qui passent minuit),
 * compare au temps prévu dans le devis, calcule le taux horaire réel
 * (montant du forfait / heures passées) et signale les projets en
 * dépassement. Données d'exemple dans un fichier temporaire.
 * On apprend : la lecture de fichier ligne par ligne, sscanf avec plusieurs
 * champs, les calculs d'heures en minutes et l'analyse de rentabilité.
 */
#include <stdio.h>
#include <string.h>

#define MAX_PROJETS 16
typedef struct { char nom[32]; int minutes; int prevu_h; long forfait; } Projet;

static Projet projets[MAX_PROJETS] = {
    {"logo-boutique", 0, 8, 75000}, {"motion-asso", 0, 20, 150000}, {"site-restaurant", 0, 25, 200000}, {"app-zem", 0, 60, 600000},
};
static int nb_projets = 4;

static Projet *projet(const char *nom) {
    for (int i = 0; i < nb_projets; i++) if (!strcmp(projets[i].nom, nom)) return &projets[i];
    if (nb_projets == MAX_PROJETS) return NULL;
    snprintf(projets[nb_projets].nom, sizeof projets[0].nom, "%s", nom);
    return &projets[nb_projets++];
}

static int lire_journal(FILE *f) {
    char ligne[128], nom[32];
    int a, mo, j, h1, m1, h2, m2, numero = 0, erreurs = 0;
    while (fgets(ligne, sizeof ligne, f)) {
        numero++;
        if (ligne[0] == '#' || ligne[0] == '\n') continue;
        if (sscanf(ligne, "%d-%d-%d %d:%d %d:%d %31s", &a, &mo, &j, &h1, &m1, &h2, &m2, nom) != 8 ||
            h1 > 23 || h2 > 23 || m1 > 59 || m2 > 59) {
            fprintf(stderr, "  ligne %d ignorée : %s", numero, ligne);
            erreurs++;
            continue;
        }
        int duree = (h2 * 60 + m2) - (h1 * 60 + m1);
        if (duree <= 0) duree += 24 * 60;                  /* session qui passe minuit */
        Projet *p = projet(nom);
        if (p) p->minutes += duree;
    }
    return erreurs;
}

int main(int argc, char *argv[]) {
    FILE *f;
    if (argc == 2) {
        f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
    } else if (argc == 1) {
        f = tmpfile();
        if (!f) return 2;
        fputs("# date début fin projet\n2026-10-01 09:00 12:30 logo-boutique\n2026-10-01 14:00 17:45 motion-asso\n"
              "2026-10-02 08:30 11:00 logo-boutique\n2026-10-02 21:00 01:30 motion-asso\n2026-10-03 09:00 13:00 site-restaurant\n"
              "2026-10-04 10:00 12:00 logo-boutique\n2026-10-05 08:00 18:00 motion-asso\n2026-10-06 09:00 12:00 app-zem\n"
              "2026-10-06 14:00 15:30 logo-boutique\n2026-10-07 09:00 17:00 motion-asso\n2026-10-08 25:00 26:00 erreur\n"
              "2026-10-08 20:00 23:00 site-restaurant\n", f);
        rewind(f);
    } else { fprintf(stderr, "Usage : %s [journal.txt]\n", argv[0]); return 1; }
    lire_journal(f);
    fclose(f);
    printf("\n%-16s %8s %8s %10s %14s\n", "Projet", "Passé", "Prévu", "Avancement", "Taux réel/h");
    for (int i = 0; i < nb_projets; i++) {
        Projet *p = &projets[i];
        double h = p->minutes / 60.0;
        printf("%-16s %5dh%02d %6dh %9.0f %% ", p->nom, p->minutes / 60, p->minutes % 60, p->prevu_h, p->prevu_h ? 100 * h / p->prevu_h : 0);
        if (h > 0 && p->forfait) printf("%9.0f FCFA", p->forfait / h); else printf("%14s", "—");
        if (p->prevu_h && h > p->prevu_h) printf("  ⚠ dépassement de %.1f h", h - p->prevu_h);
        printf("\n");
    }
    printf("\nAstuce : comparez le taux réel à votre taux cible pour ajuster vos prochains devis.\n");
    return 0;
}
