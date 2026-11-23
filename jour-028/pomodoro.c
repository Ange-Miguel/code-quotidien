/*
 * Jour 028 — Minuteur Pomodoro en console
 *
 * Problème : réviser ou coder pendant des heures sans pause fait chuter la
 * concentration. La méthode Pomodoro alterne 25 min de travail et 5 min de
 * pause, avec une longue pause de 15 min tous les 4 cycles.
 * Solution : une machine à états (TRAVAIL, PAUSE_COURTE, PAUSE_LONGUE) et une
 * barre de progression mise à jour chaque seconde. Un facteur d'accélération
 * permet de tester le programme en quelques secondes (mode démo).
 * On apprend : les machines à états, nanosleep pour attendre sans bloquer le
 * processeur, \r pour réécrire une ligne et fflush pour forcer l'affichage.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { TRAVAIL, PAUSE_COURTE, PAUSE_LONGUE } Etat;
static const char *NOMS[] = {"Travail", "Pause courte", "Pause longue"};
static const int DUREES_MIN[] = {25, 5, 15};

static void attendre_ms(long ms) {
    struct timespec t = {ms / 1000, (ms % 1000) * 1000000L};
    nanosleep(&t, NULL);
}

static void phase(Etat e, int numero, long ms_par_seconde) {
    int total = DUREES_MIN[e] * 60;
    const int largeur = 30;
    for (int s = 0; s <= total; s += 15) {           /* rafraîchissement par pas de 15 s */
        int rempli = s * largeur / total;
        printf("\r[%d] %-13s |", numero, NOMS[e]);
        for (int i = 0; i < largeur; i++) putchar(i < rempli ? '#' : '.');
        printf("| %02d:%02d restantes ", (total - s) / 60, (total - s) % 60);
        fflush(stdout);
        attendre_ms(ms_par_seconde * 15);
    }
    printf("\n");
}

