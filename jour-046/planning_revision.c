/*
 * Jour 046 — Planning de révision réparti sur N jours
 *
 * Problème : à l'approche des examens, on a plusieurs matières de difficultés
 * différentes et un nombre limité de jours. Comment répartir les heures sans
 * oublier de matière ni surcharger une journée ?
 * Solution : chaque matière reçoit un nombre d'heures proportionnel à son
 * coefficient × sa difficulté. On distribue ensuite les heures jour par jour
 * en donnant toujours la prochaine heure à la matière qui a le plus grand
 * « reste à faire » (méthode du plus fort reste), avec un maximum d'heures
 * par jour et sans mettre deux fois de suite la même matière si possible.
 * On apprend : la répartition proportionnelle, les algorithmes gloutons, les
 * tableaux à deux dimensions et l'affichage d'un emploi du temps.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_MATIERES 8
#define MAX_JOURS 30

typedef struct {
    const char *nom;
    int coef, difficulte;    /* difficulté de 1 (facile) à 3 (dur) */
    double quota, fait;
} Matiere;

static void planifier(Matiere *m, int nb, int jours, int heures_par_jour) {
    int poids_total = 0;
    for (int i = 0; i < nb; i++) poids_total += m[i].coef * m[i].difficulte;
    int total = jours * heures_par_jour;
    for (int i = 0; i < nb; i++) {
        m[i].quota = (double)total * m[i].coef * m[i].difficulte / poids_total;
        m[i].fait = 0;
    }
    printf("%d jours × %d h = %d h de révision\n\n", jours, heures_par_jour, total);
    for (int j = 1; j <= jours; j++) {
        printf("Jour %2d :", j);
        int precedente = -1;
        for (int h = 0; h < heures_par_jour; h++) {
            int choix = -1;
            double meilleur = -1e9;
            for (int i = 0; i < nb; i++) {
                if (i == precedente && nb > 1) continue;   /* pas deux fois de suite */
                double reste = m[i].quota - m[i].fait;
                if (reste > meilleur) { meilleur = reste; choix = i; }
            }
            m[choix].fait += 1;
            precedente = choix;
            printf(" %-12s", m[choix].nom);
        }
        printf("\n");
    }
    printf("\nBilan :\n");
    for (int i = 0; i < nb; i++)
        printf("  %-12s coef %d, difficulté %d → %2.0f h (quota %.1f h)\n",
               m[i].nom, m[i].coef, m[i].difficulte, m[i].fait, m[i].quota);
}

int main(int argc, char *argv[]) {
    int jours = 6, heures = 4;
    if (argc == 3) {
        char *f1, *f2;
        jours = (int)strtol(argv[1], &f1, 10);
        heures = (int)strtol(argv[2], &f2, 10);
        if (*f1 || *f2 || jours < 1 || jours > MAX_JOURS || heures < 1 || heures > 10) {
            fprintf(stderr, "Usage : %s [jours 1-30] [heures par jour 1-10]\n", argv[0]);
            return 1;
        }
    } else if (argc != 1) {
        fprintf(stderr, "Usage : %s [jours] [heures par jour]\n", argv[0]);
        return 1;
    }
    Matiere matieres[MAX_MATIERES] = {
        {"Électronique", 4, 3, 0, 0}, {"Prog. C", 3, 2, 0, 0}, {"Maths", 3, 3, 0, 0},
        {"Réseaux", 2, 2, 0, 0}, {"Anglais", 1, 1, 0, 0}
    };
    planifier(matieres, 5, jours, heures);
    return 0;
}
