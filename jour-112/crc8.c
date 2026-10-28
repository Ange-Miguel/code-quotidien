/*
 * Jour 112 — Calcul CRC-8 pour trame de capteur
 *
 * Problème : des capteurs comme le DS18B20 (bus 1-Wire) ou le SHT31 (I2C)
 * ajoutent un octet CRC-8 à chaque mesure. Si le câble est long ou bruité, un
 * bit peut s'inverser : il faut recalculer le CRC pour rejeter les mesures
 * corrompues au lieu d'afficher une température absurde.
 * Solution : le CRC-8 est le reste de la division polynomiale des données par
 * un polynôme générateur (0x31 pour Sensirion, 0x31 réfléchi = 0x8C pour
 * Dallas/Maxim). On l'implémente bit par bit, puis avec une table de 256
 * entrées (plus rapide), et on vérifie avec les valeurs de référence des
 * fiches techniques. On montre qu'il détecte toutes les erreurs d'un bit.
 * On apprend : le principe du CRC, les décalages et XOR, les variantes
 * (polynôme, valeur initiale, réflexion) et la vérification d'une trame.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* CRC-8 Sensirion (SHT3x) : polynôme 0x31, init 0xFF, non réfléchi. */
static uint8_t crc8_sensirion(const uint8_t *d, size_t n) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < n; i++) {
        crc ^= d[i];
        for (int b = 0; b < 8; b++) crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
    }
    return crc;
}

/* CRC-8 Dallas/Maxim (DS18B20) : polynôme 0x31 réfléchi (0x8C), init 0x00. */
static uint8_t table_maxim[256];
static void init_table(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t c = (uint8_t)i;
        for (int b = 0; b < 8; b++) c = (c & 1) ? (uint8_t)((c >> 1) ^ 0x8C) : (uint8_t)(c >> 1);
        table_maxim[i] = c;
    }
}
static uint8_t crc8_maxim(const uint8_t *d, size_t n) {
    uint8_t crc = 0;
    for (size_t i = 0; i < n; i++) crc = table_maxim[crc ^ d[i]];
    return crc;
}

static void trame_sht31(uint8_t msb, uint8_t lsb, uint8_t crc_recu) {
    uint8_t d[2] = {msb, lsb}, crc = crc8_sensirion(d, 2);
    uint16_t brut = (uint16_t)(msb << 8 | lsb);
    double temp = -45 + 175.0 * brut / 65535;
    printf("  SHT31 %02X %02X CRC %02X → calculé %02X : %s", msb, lsb, crc_recu, crc, crc == crc_recu ? "OK" : "CORROMPU");
    if (crc == crc_recu) printf(", température %.2f °C", temp);
    printf("\n");
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    init_table();
    const uint8_t ref[] = {0xBE, 0xEF};
    const uint8_t chk[] = "123456789";
    printf("Vérifications de référence :\n");
    printf("  Sensirion(BE EF) = 0x%02X (fiche technique : 0x92)\n", crc8_sensirion(ref, 2));
    printf("  Maxim(\"123456789\") = 0x%02X (valeur de contrôle : 0xA1)\n", crc8_maxim(chk, 9));
    if (crc8_sensirion(ref, 2) != 0x92 || crc8_maxim(chk, 9) != 0xA1) { fprintf(stderr, "CRC incorrect\n"); return 2; }

    printf("\nTrames reçues d'un SHT31 :\n");
    trame_sht31(0x66, 0x26, crc8_sensirion((const uint8_t[]){0x66, 0x26}, 2));
    trame_sht31(0x66, 0x2E, crc8_sensirion((const uint8_t[]){0x66, 0x26}, 2));   /* 1 bit inversé */

    /* Toutes les erreurs d'un bit sont détectées sur une trame DS18B20 de 8 octets */
    uint8_t rom[8] = {0x28, 0xFF, 0x4C, 0x1A, 0x6E, 0x18, 0x01, 0x00};
    rom[7] = crc8_maxim(rom, 7);
    int detectees = 0;
    for (int bit = 0; bit < 56; bit++) {
        uint8_t copie[8];
        memcpy(copie, rom, 8);
        copie[bit / 8] ^= (uint8_t)(1u << (bit % 8));
        detectees += crc8_maxim(copie, 7) != copie[7];
    }
    printf("\nAdresse DS18B20 avec CRC : ");
    for (int i = 0; i < 8; i++) printf("%02X ", rom[i]);
    printf("\nErreurs d'un bit détectées : %d/56\n", detectees);
    return 0;
}
