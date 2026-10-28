/*
 * Jour 148 — Mise à jour OTA simulée avec vérification d'intégrité
 *
 * Problème : mettre à jour à distance (Over The Air) le programme de cent
 * capteurs dispersés dans une ferme ou une ville évite de les démonter un par
 * un. Mais un téléchargement interrompu ou corrompu peut transformer chaque
 * capteur en brique inutilisable.
 * Solution : le schéma à deux partitions (A/B). L'appareil fonctionne sur la
 * partition active et télécharge le nouveau micrologiciel dans l'autre, par
 * blocs, chacun vérifié par CRC-32. À la fin, on vérifie la taille, la
 * version (pas de retour arrière) et l'empreinte globale annoncée par le
 * serveur. Seulement alors, on bascule. Au redémarrage, si le nouveau
 * programme ne confirme pas son bon fonctionnement, on revient à l'ancien.
 * On simule une mise à jour réussie, un bloc corrompu (re-téléchargé), une
 * coupure en plein téléchargement et un micrologiciel qui plante au démarrage.
 * On apprend : le principe A/B et du retour arrière, le téléchargement par
 * blocs vérifiés, les métadonnées de version et la robustesse aux pannes.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TAILLE_FW 4096
#define BLOC 512

static uint32_t crc32(const uint8_t *d, size_t n) {
    uint32_t c = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; i++) { c ^= d[i]; for (int k = 0; k < 8; k++) c = (c >> 1) ^ (0xEDB88320u & (0u - (c & 1u))); }
    return ~c;
}

typedef struct { uint8_t donnees[TAILLE_FW]; uint32_t version, crc; int valide; } Partition;
typedef struct { Partition p[2]; int active; int en_essai; } Appareil;

static void fabriquer_fw(uint8_t *fw, uint32_t version) {
    for (int i = 0; i < TAILLE_FW; i++) fw[i] = (uint8_t)((i * 31 + version * 7) & 0xFF);
}

/* Téléchargement : panne = -1 aucune, sinon numéro du bloc qui échoue (corruption ou coupure). */
static int mettre_a_jour(Appareil *a, const uint8_t *fw, uint32_t version, uint32_t crc_annonce,
                         int bloc_corrompu, int coupure_au_bloc) {
    Partition *cible = &a->p[1 - a->active];
    if (version <= a->p[a->active].version) { printf("   refus : version %u ≤ version actuelle %u\n", version, a->p[a->active].version); return 0; }
    cible->valide = 0;
    for (int b = 0; b < TAILLE_FW / BLOC; b++) {
        if (b == coupure_au_bloc) { printf("   coupure de courant au bloc %d : partition B incomplète, on reste sur A\n", b); return 0; }
        uint8_t recu[BLOC];
        memcpy(recu, fw + b * BLOC, BLOC);
        int tentatives = 0;
        if (b == bloc_corrompu) recu[100] ^= 0x20;               /* erreur de transmission */
        while (crc32(recu, BLOC) != crc32(fw + b * BLOC, BLOC)) {   /* CRC du bloc annoncé par le serveur */
            printf("   bloc %d corrompu, nouvelle demande (tentative %d)\n", b, ++tentatives + 1);
            memcpy(recu, fw + b * BLOC, BLOC);
        }
        memcpy(cible->donnees + b * BLOC, recu, BLOC);
    }
    if (crc32(cible->donnees, TAILLE_FW) != crc_annonce) { printf("   empreinte globale incorrecte : mise à jour annulée\n"); return 0; }
    cible->version = version; cible->crc = crc_annonce; cible->valide = 1;
    a->active = 1 - a->active;
    a->en_essai = 1;
    printf("   v%u écrite et vérifiée (CRC %08X) : redémarrage sur la nouvelle partition\n", version, crc_annonce);
    return 1;
}

static void redemarrer(Appareil *a, int programme_fonctionne) {
    if (!a->en_essai) return;
    if (programme_fonctionne) { a->en_essai = 0; printf("   le nouveau programme confirme son bon fonctionnement : v%u validée\n", a->p[a->active].version); }
    else { a->active = 1 - a->active; a->en_essai = 0; printf("   le nouveau programme plante (watchdog) : retour automatique à v%u\n", a->p[a->active].version); }
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    static Appareil a;
    fabriquer_fw(a.p[0].donnees, 1);
    a.p[0].version = 1; a.p[0].valide = 1; a.p[0].crc = crc32(a.p[0].donnees, TAILLE_FW);
    static uint8_t fw[TAILLE_FW];
    struct { const char *titre; uint32_t version; int corrompu, coupure, fonctionne; } essais[] = {
        {"Mise à jour v2 avec un bloc abîmé en route", 2, 3, -1, 1},
        {"Mise à jour v3 avec coupure de courant", 3, -1, 5, 1},
        {"Mise à jour v3 qui plante au démarrage", 3, -1, -1, 0},
        {"Tentative de retour à v1 (ancienne version vulnérable)", 1, -1, -1, 1},
    };
    for (int i = 0; i < 4; i++) {
        printf("%s (actuellement v%u) :\n", essais[i].titre, a.p[a.active].version);
        fabriquer_fw(fw, essais[i].version);
        if (mettre_a_jour(&a, fw, essais[i].version, crc32(fw, TAILLE_FW), essais[i].corrompu, essais[i].coupure))
            redemarrer(&a, essais[i].fonctionne);
        printf("   → en service : v%u\n\n", a.p[a.active].version);
    }
    return 0;
}
