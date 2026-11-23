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

