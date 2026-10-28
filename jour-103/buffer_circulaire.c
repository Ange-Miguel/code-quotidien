/*
 * Jour 103 — Buffer circulaire pour données de capteur
 *
 * Problème : un capteur produit des mesures à intervalle régulier (souvent
 * dans une interruption), tandis que la boucle principale les traite ou les
 * envoie quand elle peut. Il faut un tampon de taille fixe, sans malloc, qui
 * ne bloque jamais le producteur.
 * Solution : un buffer circulaire (ring buffer) : un tableau, un indice
 * d'écriture et un indice de lecture qui reviennent à zéro en fin de tableau.
 * Avec une taille puissance de 2, le modulo devient un simple masque
 * (& (TAILLE − 1)), très rapide sur microcontrôleur. Quand il est plein, on
 * écrase la plus ancienne mesure et on compte les pertes.
 * On apprend : le buffer circulaire, l'astuce du masque pour les puissances de
 * 2, les types entiers de taille fixe et la notion producteur / consommateur.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define TAILLE 8u                 /* doit être une puissance de 2 */
#define MASQUE (TAILLE - 1u)
_Static_assert((TAILLE & MASQUE) == 0, "TAILLE doit être une puissance de 2");

typedef struct {
    int16_t donnees[TAILLE];
    uint32_t ecriture, lecture;   /* compteurs libres : la différence donne le remplissage */
    uint32_t pertes;
} Anneau;

static uint32_t remplissage(const Anneau *a) { return a->ecriture - a->lecture; }

static void ecrire(Anneau *a, int16_t valeur) {
    if (remplissage(a) == TAILLE) { a->lecture++; a->pertes++; }   /* plein : on écrase la plus ancienne */
    a->donnees[a->ecriture & MASQUE] = valeur;
    a->ecriture++;
}

static bool lire(Anneau *a, int16_t *valeur) {
    if (remplissage(a) == 0) return false;
    *valeur = a->donnees[a->lecture & MASQUE];
    a->lecture++;
    return true;
}

static void etat(const Anneau *a) {
    printf("   [");
    for (uint32_t i = 0; i < TAILLE; i++) {
        bool occupe = (i - (a->lecture & MASQUE)) % TAILLE < remplissage(a);
        if (occupe) printf("%5d", a->donnees[i]);
        else printf("    .");
    }
    printf(" ]  rempli %u/%u, pertes %u\n", remplissage(a), TAILLE, a->pertes);
}

/* Température simulée en dixièmes de °C (capteur un peu bruité). */
static int16_t mesure(int t) { return (int16_t)(275 + (t * 7) % 13 - 6); }

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    Anneau a = {{0}, 0, 0, 0};
    int16_t v;
    puts("Le capteur écrit 5 mesures :");
    for (int t = 0; t < 5; t++) ecrire(&a, mesure(t));
    etat(&a);
    puts("La boucle principale en lit 3 :");
    for (int k = 0; k < 3 && lire(&a, &v); k++) printf("   lu %d (%.1f °C)\n", v, v / 10.0);
    etat(&a);
    puts("Le capteur écrit 10 mesures pendant que la boucle est occupée (débordement) :");
    for (int t = 5; t < 15; t++) ecrire(&a, mesure(t));
    etat(&a);
    puts("La boucle vide le tampon (les plus anciennes ont été écrasées) :");
    printf("   ");
    while (lire(&a, &v)) printf("%d ", v);
    printf("\n");
    etat(&a);
    a.ecriture = a.lecture = UINT32_MAX - 2;          /* les compteurs débordent sans problème */
    for (int t = 0; t < 5; t++) ecrire(&a, (int16_t)t);
    printf("Après débordement des compteurs 32 bits : ");
    while (lire(&a, &v)) printf("%d ", v);
    printf("(ordre conservé)\n");
    return 0;
}
