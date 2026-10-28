/*
 * Jour 073 — Matrice : rotation d'une image en niveaux de gris
 *
 * Problème : une caméra embarquée (ESP32-CAM, capteur d'image) est souvent
 * montée de travers : l'image arrive tournée de 90°. Il faut la redresser
 * avec peu de mémoire, comme sur un microcontrôleur.
 * Solution : une image en niveaux de gris est une matrice d'octets (0 = noir,
 * 255 = blanc). Rotation de 90° dans le sens horaire : le pixel (l, c) va en
 * (c, H − 1 − l). Pour une image carrée, on la tourne en place, sans
 * deuxième tableau, en échangeant les pixels quatre par quatre, couche par
 * couche. On affiche le résultat en caractères ASCII et on vérifie que quatre
 * rotations ramènent l'image d'origine.
 * On apprend : les tableaux 2D, les transformations d'indices, l'algorithme
 * en place (économie de mémoire) et l'affichage ASCII d'une image.
 */
#include <stdio.h>
#include <string.h>

#define N 12
typedef unsigned char Pixel;

static void afficher(Pixel img[N][N], const char *titre) {
    const char *nuances = " .:-=+*#%@";
    printf("%s\n", titre);
    for (int l = 0; l < N; l++) {
        printf("   ");
        for (int c = 0; c < N; c++) {
            char ch = nuances[img[l][c] * 9 / 255];
            putchar(ch); putchar(ch);
        }
        putchar('\n');
    }
}

/* Rotation de 90° horaire, en place, couche par couche. */
static void tourner_en_place(Pixel m[N][N]) {
    for (int couche = 0; couche < N / 2; couche++) {
        int premier = couche, dernier = N - 1 - couche;
        for (int i = premier; i < dernier; i++) {
            int decal = i - premier;
            Pixel haut = m[premier][i];
            m[premier][i] = m[dernier - decal][premier];       /* gauche → haut */
            m[dernier - decal][premier] = m[dernier][dernier - decal]; /* bas → gauche */
            m[dernier][dernier - decal] = m[i][dernier];       /* droite → bas */
            m[i][dernier] = haut;                              /* haut → droite */
        }
    }
}

/* Version avec copie, pour vérifier la version en place. */
static void tourner_copie(Pixel src[N][N], Pixel dst[N][N]) {
    for (int l = 0; l < N; l++)
        for (int c = 0; c < N; c++) dst[c][N - 1 - l] = src[l][c];
}

static void dessiner_fleche(Pixel img[N][N]) {
    memset(img, 0, sizeof(Pixel) * N * N);
    for (int c = 1; c < 9; c++) img[5][c] = img[6][c] = 255;    /* tige */
    for (int k = 0; k < 4; k++)                                  /* pointe vers la droite */
        for (int l = 2 + k; l <= 9 - k; l++) img[l][8 + k] = (Pixel)(255 - 40 * k);
    for (int l = 0; l < N; l++) img[l][0] = 120;                 /* bord gris à gauche */
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) {
        fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n");
        return 1;
    }
    Pixel img[N][N], origine[N][N], attendu[N][N];
    dessiner_fleche(img);
    memcpy(origine, img, sizeof img);
    afficher(img, "Image d'origine (flèche vers la droite) :");

    tourner_copie(img, attendu);
    tourner_en_place(img);
    afficher(img, "\nAprès rotation de 90° (en place) :");
    if (memcmp(img, attendu, sizeof img) != 0) { fprintf(stderr, "Erreur de rotation\n"); return 2; }

    for (int k = 0; k < 3; k++) tourner_en_place(img);
    printf("\nQuatre rotations ramènent l'image d'origine : %s\n", memcmp(img, origine, sizeof img) ? "NON" : "oui");
    printf("Mémoire utilisée en plus : 1 pixel (rotation en place) au lieu de %d octets\n", N * N);
    return 0;
}
