/*
 * Jour 019 — Conversion de durée : secondes → jours, heures, minutes, secondes
 *
 * Problème : un compteur de microcontrôleur, un chronomètre ou un journal de
 * serveur donne souvent une durée en secondes (« uptime 1 234 567 s »), peu
 * lisible pour un humain.
 * Solution : divisions entières successives par 86 400, 3 600 et 60, avec le
 * reste à chaque étape. On fait aussi l'opération inverse : « 2j3h15m » vers
 * des secondes, avec un petit analyseur de texte.
 * On apprend : la division entière et le modulo, les types entiers longs, et
 * l'écriture d'un mini-analyseur caractère par caractère.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void afficher_duree(long long s) {
    long long j = s / 86400, h = s % 86400 / 3600, m = s % 3600 / 60, r = s % 60;
    printf("%12lld s = ", s);
    if (j) printf("%lld j ", j);
    printf("%02lld h %02lld min %02lld s\n", h, m, r);
}

/* Analyse un texte comme "2j3h15m10s" ; retourne -1 si invalide. */
static long long analyser(const char *texte) {
    long long total = 0, nombre = 0;
    int chiffres = 0;
    for (const char *p = texte; *p; p++) {
        if (isdigit((unsigned char)*p)) {
            nombre = nombre * 10 + (*p - '0');
            chiffres++;
            if (nombre > 1000000000LL) return -1;
            continue;
        }
        if (!chiffres) return -1;
        switch (tolower((unsigned char)*p)) {
            case 'j': total += nombre * 86400; break;
            case 'h': total += nombre * 3600; break;
            case 'm': total += nombre * 60; break;
            case 's': total += nombre; break;
            default: return -1;
        }
        nombre = 0;
        chiffres = 0;
    }
    if (chiffres) total += nombre;   /* nombre final sans unité = secondes */
    return total;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        long long exemples[] = {59, 3600, 86399, 1234567, 31536000};
        for (int i = 0; i < 5; i++) afficher_duree(exemples[i]);
        const char *textes[] = {"2j3h15m", "90m", "1h30m45s"};
        for (int i = 0; i < 3; i++) {
            printf("%-10s → ", textes[i]);
            afficher_duree(analyser(textes[i]));
        }
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        long long s = analyser(argv[i]);
        if (s < 0) {
            fprintf(stderr, "Durée invalide : %s (ex. 3600, 2j3h, 45m10s)\n", argv[i]);
            return 1;
        }
        afficher_duree(s);
    }
    return 0;
}
