/*
 * Jour 175 — Raccourcisseur d'URL en mémoire
 *
 * Problème : une longue adresse de produit ou de formulaire est difficile à
 * partager par SMS ou à imprimer sur une affiche. Les raccourcisseurs (comme
 * bit.ly) la remplacent par un code court, et comptent les clics.
 * Solution : chaque URL reçoit un identifiant numérique croissant, converti en
 * base 62 (0-9, a-z, A-Z) : 7 caractères suffisent pour plus de 3 500 milliards
 * d'adresses. Une table de hachage (adressage ouvert, sondage linéaire)
 * retrouve l'URL longue à partir du code ; une seconde évite de créer deux
 * codes pour la même URL. On compte les visites et on refuse les URL qui ne
 * commencent pas par http:// ou https://.
 * On apprend : la conversion en base 62, les tables de hachage à adressage
 * ouvert, la fonction de hachage djb2 et la validation d'entrées.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITE 64
static const char ALPHABET[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef struct { int occupe; char code[12]; char url[200]; int visites; } Entree;
static Entree par_code[CAPACITE], par_url[CAPACITE];
static unsigned long prochain_id = 125000;      /* on démarre plus haut pour des codes de 3 caractères */

static unsigned long djb2(const char *s) { unsigned long h = 5381; while (*s) h = h * 33 + (unsigned char)*s++; return h; }

static void base62(unsigned long n, char *sortie) {
    char tmp[12];
    int i = 0;
    do { tmp[i++] = ALPHABET[n % 62]; n /= 62; } while (n);
    for (int k = 0; k < i; k++) sortie[k] = tmp[i - 1 - k];
    sortie[i] = '\0';
}

static Entree *chercher(Entree *table, const char *cle, int par_url_cle) {
    unsigned long h = djb2(cle) % CAPACITE;
    for (int essai = 0; essai < CAPACITE; essai++, h = (h + 1) % CAPACITE) {   /* sondage linéaire */
        Entree *e = &table[h];
        if (!e->occupe || !strcmp(par_url_cle ? e->url : e->code, cle)) return e;
    }
    return NULL;
}

static const char *raccourcir(const char *url) {
    if (strncmp(url, "http://", 7) && strncmp(url, "https://", 8)) return NULL;
    if (strlen(url) >= 200) return NULL;
    Entree *existant = chercher(par_url, url, 1);
    if (existant && existant->occupe) return existant->code;        /* déjà raccourcie */
    Entree c = {1, "", "", 0};
    base62(prochain_id++, c.code);
    snprintf(c.url, sizeof c.url, "%s", url);
    Entree *a = chercher(par_code, c.code, 0), *b = existant;
    if (!a || !b) return NULL;                                      /* table pleine */
    *a = c; *b = c;
    return a->code;
}

static const char *visiter(const char *code) {
    Entree *e = chercher(par_code, code, 0);
    if (!e || !e->occupe) return NULL;
    e->visites++;
    return e->url;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        const char *c = raccourcir(argv[1]);
        if (!c) { fprintf(stderr, "URL invalide (doit commencer par http:// ou https://)\n"); return 1; }
        printf("https://crt.bj/%s\n", c);
        return 0;
    }
    if (argc > 2) { fprintf(stderr, "Usage : %s [url]\n", argv[0]); return 1; }
    const char *urls[] = {"https://boutique-exemple.bj/produits/ecouteurs-bluetooth-sans-fil-reduction-bruit?ref=whatsapp",
                          "https://forms.exemple.com/inscription-tournoi-football-campus-2026",
                          "https://boutique-exemple.bj/produits/ecouteurs-bluetooth-sans-fil-reduction-bruit?ref=whatsapp",
                          "ftp://fichiers.exemple.bj/cours.pdf"};
    char codes[4][12] = {""};
    for (int i = 0; i < 4; i++) {
        const char *c = raccourcir(urls[i]);
        printf("%-60.60s… → %s\n", urls[i], c ? c : "refusée (protocole non autorisé)");
        if (c) snprintf(codes[i], sizeof codes[i], "%s", c);
    }
    const char *clics[] = {codes[0], codes[1], codes[0], codes[0], "zzz9", codes[1]};
    printf("\nVisites :\n");
    for (int i = 0; i < 6; i++) {
        const char *u = visiter(clics[i]);
        printf("   crt.bj/%-6s → %s\n", clics[i], u ? "redirection vers l'adresse longue" : "404 : lien inconnu");
    }
    printf("\nStatistiques : crt.bj/%s = %d clics, crt.bj/%s = %d clics\n", codes[0], chercher(par_code, codes[0], 0)->visites,
           codes[1], chercher(par_code, codes[1], 0)->visites);
    printf("Capacité avec 7 caractères en base 62 : 62^7 = 3 521 614 606 208 liens\n");
    return 0;
}
