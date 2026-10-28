/*
 * Jour 115 — Parseur de trames NMEA GPS ($GPGGA)
 *
 * Problème : un module GPS (NEO-6M, NEO-M8N) envoie sur sa liaison série des
 * lignes de texte au format NMEA 0183, par exemple
 * « $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47 ».
 * Pour un traceur de moto ou de colis, il faut en extraire l'heure, la
 * position en degrés décimaux, l'altitude et la qualité du signal.
 * Solution : on vérifie la somme de contrôle (XOR de tous les caractères entre
 * « $ » et « * », comparé aux deux chiffres hexadécimaux finaux), on découpe
 * les champs en gérant les champs vides (strsep, contrairement à strtok), et
 * on convertit le format ddmm.mmmm en degrés décimaux.
 * On apprend : le format NMEA, les sommes de contrôle XOR, strsep et les
 * champs vides, et la conversion degrés-minutes → degrés décimaux.
 */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int h, m, s, qualite, satellites, valide;
    double latitude, longitude, altitude, hdop;
} PositionGps;

static int checksum_ok(const char *trame) {
    const char *etoile = strchr(trame, '*');
    if (trame[0] != '$' || !etoile || strlen(etoile) < 3) return 0;
    unsigned calc = 0;
    for (const char *p = trame + 1; p < etoile; p++) calc ^= (unsigned char)*p;
    return calc == (unsigned)strtoul(etoile + 1, NULL, 16);
}

/* ddmm.mmmm (ou dddmm.mmmm) → degrés décimaux, négatif au sud et à l'ouest. */
static double vers_degres(const char *valeur, const char *hemisphere) {
    double v = atof(valeur);
    int degres = (int)(v / 100);
    double d = degres + (v - degres * 100) / 60.0;
    return (*hemisphere == 'S' || *hemisphere == 'W') ? -d : d;
}

static int analyser_gga(const char *trame, PositionGps *p) {
    if (!checksum_ok(trame)) return -1;
    char copie[128];
    snprintf(copie, sizeof copie, "%s", trame);
    *strchr(copie, '*') = '\0';
    char *champs[16] = {0}, *curseur = copie;
    int n = 0;
    while (n < 16 && curseur) champs[n++] = strsep(&curseur, ",");   /* garde les champs vides */
    if (n < 10 || strcmp(champs[0] + 3, "GGA") != 0) return -2;
    memset(p, 0, sizeof *p);
    if (strlen(champs[1]) >= 6) sscanf(champs[1], "%2d%2d%2d", &p->h, &p->m, &p->s);
    p->qualite = atoi(champs[6]);
    p->valide = p->qualite > 0 && *champs[2];
    if (p->valide) {
        p->latitude = vers_degres(champs[2], champs[3]);
        p->longitude = vers_degres(champs[4], champs[5]);
    }
    p->satellites = atoi(champs[7]);
    p->hdop = atof(champs[8]);
    p->altitude = atof(champs[9]);
    return 0;
}

static void traiter(const char *trame) {
    PositionGps p;
    int r = analyser_gga(trame, &p);
    printf("%s\n", trame);
    if (r == -1) { printf("   ✘ somme de contrôle invalide : trame rejetée\n\n"); return; }
    if (r == -2) { printf("   ✘ ce n'est pas une trame GGA\n\n"); return; }
    if (!p.valide) { printf("   %02d:%02d:%02d UTC — pas de position (recherche de satellites, %d en vue)\n\n", p.h, p.m, p.s, p.satellites); return; }
    printf("   %02d:%02d:%02d UTC — %.6f, %.6f — altitude %.1f m — %d satellites, HDOP %.1f\n",
           p.h, p.m, p.s, p.latitude, p.longitude, p.altitude, p.satellites, p.hdop);
    printf("   carte : https://www.openstreetmap.org/?mlat=%.5f&mlon=%.5f\n\n", p.latitude, p.longitude);
}

static void fabriquer(char *dst, size_t taille, const char *corps) {   /* ajoute le checksum */
    unsigned c = 0;
    for (const char *p = corps; *p; p++) c ^= (unsigned char)*p;
    snprintf(dst, taille, "$%s*%02X", corps, c);
}

int main(int argc, char *argv[]) {
    if (argc == 2) { traiter(argv[1]); return checksum_ok(argv[1]) ? 0 : 1; }
    if (argc > 2) { fprintf(stderr, "Usage : %s [\"$GPGGA,...*CS\"]\n", argv[0]); return 1; }
    char t1[128], t2[128], t3[128];
    traiter("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");   /* exemple classique */
    fabriquer(t1, sizeof t1, "GPGGA,081532,0621.924,N,00225.098,E,1,09,1.1,12.3,M,24.1,M,,");   /* Cotonou */
    traiter(t1);
    fabriquer(t2, sizeof t2, "GPGGA,081533,,,,,0,03,,,M,,M,,");                              /* pas encore de fix */
    traiter(t2);
    snprintf(t3, sizeof t3, "%s", t1);
    t3[20] = '9';                                                                          /* corruption */
    traiter(t3);
    return 0;
}
