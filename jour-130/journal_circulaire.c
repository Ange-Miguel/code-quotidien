/*
 * Jour 130 — Journal circulaire de données en mémoire limitée
 *
 * Problème : un enregistreur autonome (station météo, traceur de
 * température de vaccins) ne dispose que de quelques kilo-octets de mémoire,
 * mais doit garder un historique utile : chaque minute de la dernière heure,
 * puis des moyennes horaires sur plusieurs jours.
 * Solution : deux journaux circulaires à résolutions différentes. Les
 * mesures brutes entrent dans un anneau de 60 cases ; toutes les 60 mesures,
 * on en calcule le minimum, la moyenne et le maximum qu'on stocke dans un
 * second anneau de 72 cases (3 jours). Les valeurs sont compactées en entiers
 * 16 bits (dixièmes de degré) pour diviser la place par deux. On mesure la
 * mémoire utilisée et on affiche l'historique.
 * On apprend : le sous-échantillonnage (agrégation), la compaction des
 * données en types entiers de taille fixe, sizeof et l'économie de mémoire.
 */
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BRUTES 60
#define HORAIRES 72

typedef struct { int16_t min, moy, max; } Agregat;   /* dixièmes de °C */

typedef struct {
    int16_t brutes[BRUTES]; uint16_t nb_brutes, pos_brutes;
    Agregat horaires[HORAIRES]; uint16_t nb_horaires, pos_horaires;
    int32_t somme; int16_t mini, maxi; uint16_t dans_heure;
} Journal;

static void ajouter(Journal *j, double temperature) {
    int16_t v = (int16_t)lround(temperature * 10);
    j->brutes[j->pos_brutes] = v;
    j->pos_brutes = (uint16_t)((j->pos_brutes + 1) % BRUTES);
    if (j->nb_brutes < BRUTES) j->nb_brutes++;
    if (j->dans_heure == 0) { j->mini = j->maxi = v; j->somme = 0; }
    j->somme += v;
    if (v < j->mini) j->mini = v;
    if (v > j->maxi) j->maxi = v;
    if (++j->dans_heure == 60) {                          /* une heure complète : on agrège */
        j->horaires[j->pos_horaires] = (Agregat){j->mini, (int16_t)(j->somme / 60), j->maxi};
        j->pos_horaires = (uint16_t)((j->pos_horaires + 1) % HORAIRES);
        if (j->nb_horaires < HORAIRES) j->nb_horaires++;
        j->dans_heure = 0;
    }
}

static double temperature_simulee(int minute) {
    double heure = minute / 60.0;
    return 27 + 5 * sin((heure - 9) / 24 * 2 * 3.14159265) + (minute * 37 % 11 - 5) * 0.05;
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    static Journal j;
    int minutes = 5 * 24 * 60;                            /* 5 jours de mesures, une par minute */
    for (int m = 0; m < minutes; m++) ajouter(&j, temperature_simulee(m));

    printf("%d mesures reçues (5 jours). Mémoire du journal : %zu octets\n", minutes, sizeof j);
    printf("Stocker toutes les mesures en double aurait demandé %zu octets.\n\n", (size_t)minutes * sizeof(double));
    printf("Dernières 10 minutes (résolution 1 min) :\n   ");
    for (int k = 10; k >= 1; k--) printf("%.1f ", j.brutes[(j.pos_brutes + BRUTES - k) % BRUTES] / 10.0);
    printf("\n\nDernières 24 heures (min / moyenne / max par heure) :\n");
    for (int k = 24; k >= 1; k--) {
        Agregat a = j.horaires[(j.pos_horaires + HORAIRES - k) % HORAIRES];
        printf("   h-%-2d %5.1f %5.1f %5.1f  ", k, a.min / 10.0, a.moy / 10.0, a.max / 10.0);
        for (int b = 0; b < (a.moy - 200) / 5; b++) putchar('=');
        putchar('\n');
    }
    printf("Historique horaire disponible : %u heures (les plus anciennes ont été remplacées)\n", j.nb_horaires);
    return 0;
}
