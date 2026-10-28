/*
 * Jour 127 — Minuterie logicielle (software timers) sans blocage
 *
 * Problème : un microcontrôleur n'a que quelques timers matériels, mais un
 * projet domotique a besoin de dizaines de minuteries : éteindre la lumière
 * du couloir après 2 minutes, relancer une connexion après 30 s, couper une
 * pompe au bout de 10 min… sans jamais bloquer le programme avec delay().
 * Solution : un seul compteur de millisecondes (comme millis() d'Arduino) et
 * un tableau de minuteries logicielles : chacune a une échéance, une période
 * (0 = une seule fois) et une fonction de rappel (callback). La fonction
 * timers_traiter() appelée dans la boucle déclenche celles qui sont échues.
 * La comparaison des dates par différence signée reste correcte même quand
 * le compteur 32 bits déborde (après 49,7 jours).
 * On apprend : les pointeurs de fonction (callbacks), les minuteries
 * périodiques et uniques, le débordement des compteurs et la gestion d'un
 * pool statique sans malloc.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_TIMERS 8
typedef void (*Rappel)(int id);

typedef struct { bool actif; uint32_t echeance, periode; Rappel rappel; const char *nom; } Timer;

static Timer timers[MAX_TIMERS];
static uint32_t millis;          /* horloge simulée, déborde après 2^32 ms */

static int timer_demarrer(const char *nom, uint32_t delai, uint32_t periode, Rappel r) {
    for (int i = 0; i < MAX_TIMERS; i++)
        if (!timers[i].actif) {
            timers[i] = (Timer){true, millis + delai, periode, r, nom};
            return i;
        }
    return -1;
}

static void timer_arreter(int id) { if (id >= 0 && id < MAX_TIMERS) timers[id].actif = false; }

static void timers_traiter(void) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        Timer *t = &timers[i];
        if (t->actif && (int32_t)(millis - t->echeance) >= 0) {   /* correct même après débordement */
            if (t->periode) t->echeance += t->periode; else t->actif = false;
            t->rappel(i);
        }
    }
}

static int lumiere_couloir = -1, led = 0, tentatives = 0, id_reco = -1;

static void horodatage(void) { printf("  [%10u ms] ", millis); }
static void eteindre_couloir(int id) { (void)id; horodatage(); puts("lumière du couloir éteinte (2 min sans mouvement)"); lumiere_couloir = -1; }
static void clignoter(int id) { (void)id; led ^= 1; if (millis % 10000 < 1000) { horodatage(); printf("LED de vie : %s\n", led ? "allumée" : "éteinte"); } }
static void reconnecter(int id) {
    horodatage();
    printf("tentative de reconnexion Wi-Fi n°%d", ++tentatives);
    if (tentatives == 3) { puts(" → connecté"); timer_arreter(id); id_reco = -1; } else puts(" → échec");
}
static void mouvement(void) {
    horodatage(); puts("mouvement détecté : lumière du couloir allumée");
    timer_arreter(lumiere_couloir);                   /* on relance la temporisation */
    lumiere_couloir = timer_demarrer("couloir", 120000, 0, eteindre_couloir);
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc > 1) { fprintf(stderr, "Ce programme est une démonstration : lancez-le sans argument.\n"); return 1; }
    millis = UINT32_MAX - 30000;                      /* on démarre 30 s avant le débordement du compteur */
    timer_demarrer("LED", 0, 500, clignoter);
    id_reco = timer_demarrer("wifi", 5000, 30000, reconnecter);
    for (uint32_t pas = 0; pas < 300000; pas++) {    /* 5 minutes simulées */
        if (pas == 10000 || pas == 70000) mouvement();
        timers_traiter();
        millis++;
    }
    int actifs = 0;
    for (int i = 0; i < MAX_TIMERS; i++) actifs += timers[i].actif;
    printf("Fin : le compteur a débordé (%u ms) sans perturber les minuteries ; %d minuterie(s) encore active(s).\n", millis, actifs);
    return 0;
}
