/*
 * Jour 124 — Dimensionnement d'un kit solaire (panneaux, batterie, onduleur)
 *
 * Problème : avec les coupures fréquentes ou loin du réseau, beaucoup de
 * foyers et de boutiques s'équipent en solaire. Mais un kit mal dimensionné
 * (panneaux trop petits, batterie qui s'épuise la nuit) déçoit vite.
 * Solution : on fait le bilan des appareils (puissance × heures d'usage), on
 * en déduit l'énergie quotidienne, puis : puissance crête des panneaux =
 * énergie / (heures d'ensoleillement équivalent × rendement global) ;
 * capacité batterie = énergie de nuit × jours d'autonomie / (tension ×
 * profondeur de décharge) ; puissance de l'onduleur = somme des appareils
 * pouvant fonctionner ensemble × marge de démarrage. Valeurs indicatives :
 * faire valider par un installateur.
 * On apprend : le bilan énergétique (Wh), la différence entre puissance (W) et
 * énergie (Wh), l'arrondi au composant du commerce supérieur et les
 * hypothèses d'un dimensionnement.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct { const char *nom; double watts; double heures_jour; double heures_nuit; int quantite; } Appareil;

#define ENSOLEILLEMENT 5.0     /* heures équivalent plein soleil (sud Bénin, valeur indicative) */
#define RENDEMENT 0.75         /* pertes : chaleur, poussière, câbles, régulateur, onduleur */
#define TENSION_BATT 24.0
#define DOD 0.8                /* profondeur de décharge admissible (lithium) */
#define AUTONOMIE_JOURS 1.0
#define PANNEAU_W 400.0
#define BATTERIE_AH 100.0

int main(int argc, char *argv[]) {
    Appareil maison[] = {
        {"Ampoules LED", 9, 1, 5, 6}, {"Ventilateurs", 45, 4, 6, 2}, {"Téléviseur", 80, 2, 3, 1},
        {"Réfrigérateur (moyenne)", 60, 12, 12, 1}, {"Chargeurs téléphone", 10, 2, 1, 4},
        {"Ordinateur portable", 65, 5, 2, 1}, {"Box Internet", 12, 12, 12, 1},
    };
    int n = (int)(sizeof maison / sizeof maison[0]);
    double jours = AUTONOMIE_JOURS;
    if (argc == 2) {
        char *fin;
        jours = strtod(argv[1], &fin);
        if (*fin || jours < 0.5 || jours > 5) { fprintf(stderr, "Usage : %s [jours d'autonomie 0.5..5]\n", argv[0]); return 1; }
    } else if (argc > 2) { fprintf(stderr, "Usage : %s [jours d'autonomie]\n", argv[0]); return 1; }

    double jour_wh = 0, nuit_wh = 0, pointe_w = 0;
    printf("%-26s %6s %4s %10s\n", "Appareil", "W", "Qté", "Wh/jour");
    for (int i = 0; i < n; i++) {
        Appareil *a = &maison[i];
        double wh = a->watts * a->quantite * (a->heures_jour + a->heures_nuit);
        jour_wh += a->watts * a->quantite * a->heures_jour;
        nuit_wh += a->watts * a->quantite * a->heures_nuit;
        pointe_w += a->watts * a->quantite;
        printf("%-26s %6.0f %4d %10.0f\n", a->nom, a->watts, a->quantite, wh);
    }
    double total = jour_wh + nuit_wh;
    printf("Total : %.0f Wh/jour (dont %.0f Wh la nuit)\n\n", total, nuit_wh);

    double crete = total / (ENSOLEILLEMENT * RENDEMENT);
    int panneaux = (int)ceil(crete / PANNEAU_W);
    double ah = nuit_wh * jours / (TENSION_BATT * DOD);
    int batteries = (int)ceil(ah / BATTERIE_AH);
    double onduleur = pointe_w * 1.5;   /* marge pour les démarrages (moteur du frigo) */
    printf("Panneaux : %.0f Wc nécessaires → %d panneau(x) de %.0f W (%.0f Wc)\n", crete, panneaux, PANNEAU_W, panneaux * PANNEAU_W);
    printf("Batterie : %.0f Ah en %.0f V pour %.1f nuit(s) → %d batterie(s) de %.0f Ah en parallèle\n",
           ah, TENSION_BATT, jours, batteries, BATTERIE_AH);
    printf("Onduleur : appareils simultanés %.0f W × 1,5 → au moins %.0f W (ex. %d W)\n",
           pointe_w, onduleur, (int)(ceil(onduleur / 500) * 500));
    printf("Régulateur MPPT : courant ≈ %.0f A sous %.0f V\n", panneaux * PANNEAU_W / TENSION_BATT * 1.25, TENSION_BATT);
    printf("\nValeurs indicatives : faites valider l'installation par un professionnel.\n");
    return 0;
}
