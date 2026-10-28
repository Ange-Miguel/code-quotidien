/*
 * Jour 085 — Calcul de somme de contrôle (checksum) d'un fichier
 *
 * Problème : un fichier téléchargé, un firmware envoyé à une carte ou une
 * trame reçue par radio a-t-il été corrompu en route ? Il faut une
 * « empreinte » qui change si un seul bit change.
 * Solution : on implémente trois sommes de contrôle de puissance croissante :
 * somme simple sur 8 bits, Fletcher-16 (sensible à l'ordre des octets) et
 * CRC-32 (celui des fichiers ZIP et d'Ethernet, avec une table précalculée).
 * On montre qu'en changeant un seul bit, la somme simple peut ne pas réagir
 * à une permutation alors que CRC-32 change complètement.
 * On apprend : la lecture binaire de fichiers (fread), les opérations sur les
 * bits, les entiers non signés de taille fixe (uint32_t) et les tables de
 * précalcul.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint32_t table_crc[256];

static void init_crc(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        table_crc[i] = c;
    }
}

typedef struct { uint8_t somme; uint16_t fletcher; uint32_t crc; size_t taille; } Empreintes;

static void ajouter(Empreintes *e, const uint8_t *d, size_t n, uint16_t *s1, uint16_t *s2) {
    for (size_t i = 0; i < n; i++) {
        e->somme = (uint8_t)(e->somme + d[i]);
        *s1 = (uint16_t)((*s1 + d[i]) % 255);
        *s2 = (uint16_t)((*s2 + *s1) % 255);
        e->crc = table_crc[(e->crc ^ d[i]) & 0xFF] ^ (e->crc >> 8);
    }
    e->taille += n;
}

static Empreintes calculer_flux(FILE *f) {
    Empreintes e = {0, 0, 0xFFFFFFFFu, 0};
    uint16_t s1 = 0, s2 = 0;
    uint8_t tampon[4096];
    size_t n;
    while ((n = fread(tampon, 1, sizeof tampon, f)) > 0) ajouter(&e, tampon, n, &s1, &s2);
    e.fletcher = (uint16_t)((s2 << 8) | s1);
    e.crc ^= 0xFFFFFFFFu;
    return e;
}

static Empreintes calculer_memoire(const char *texte) {
    Empreintes e = {0, 0, 0xFFFFFFFFu, 0};
    uint16_t s1 = 0, s2 = 0;
    ajouter(&e, (const uint8_t *)texte, strlen(texte), &s1, &s2);
    e.fletcher = (uint16_t)((s2 << 8) | s1);
    e.crc ^= 0xFFFFFFFFu;
    return e;
}

static void afficher(const char *nom, Empreintes e) {
    printf("%-30s %6zu octets | somme 0x%02X | Fletcher-16 0x%04X | CRC-32 0x%08X\n",
           nom, e.taille, e.somme, e.fletcher, e.crc);
}

int main(int argc, char *argv[]) {
    init_crc();
    if (argc > 1) {
        int erreur = 0;
        for (int i = 1; i < argc; i++) {
            FILE *f = fopen(argv[i], "rb");
            if (!f) { perror(argv[i]); erreur = 1; continue; }
            afficher(argv[i], calculer_flux(f));
            fclose(f);
        }
        return erreur;
    }
    if (calculer_memoire("123456789").crc != 0xCBF43926u) { fprintf(stderr, "CRC incorrect\n"); return 2; }
    afficher("\"123456789\" (référence)", calculer_memoire("123456789"));
    afficher("\"Montant: 5000 FCFA\"", calculer_memoire("Montant: 5000 FCFA"));
    afficher("\"Montant: 5001 FCFA\" (1 bit)", calculer_memoire("Montant: 5001 FCFA"));
    afficher("\"Montant: 0500 FCFA\" (permuté)", calculer_memoire("Montant: 0500 FCFA"));
    puts("\nLa somme simple ne voit pas la permutation 5000 → 0500 ; Fletcher et CRC-32 la détectent.");
    return 0;
}
