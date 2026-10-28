/*
 * Jour 190 — Conversion Markdown → HTML simple
 *
 * Problème : écrire directement du HTML pour un blog, une documentation ou
 * une fiche produit est fastidieux. Le Markdown (celui des README de GitHub)
 * est beaucoup plus simple à taper : # pour un titre, **gras**, *italique*,
 * - pour une liste, [texte](lien). Il faut ensuite le convertir en HTML.
 * Solution : un convertisseur ligne par ligne avec un petit automate : on
 * sait si on est dans un paragraphe, une liste ou un bloc de code, et on ferme
 * les balises au bon moment. Dans chaque ligne, un analyseur traite les
 * éléments en ligne (gras, italique, code, liens) et échappe les caractères
 * spéciaux (<, >, &) pour éviter qu'un texte injecte du HTML ou du script.
 * On apprend : l'analyse de texte par états, l'écriture dans un tampon,
 * l'échappement HTML (sécurité) et la structure d'un document HTML.
 */
#include <stdio.h>
#include <string.h>

typedef enum { RIEN, PARAGRAPHE, LISTE, CODE } Bloc;
static Bloc bloc = RIEN;

static void echapper(char c) {
    if (c == '<') fputs("&lt;", stdout);
    else if (c == '>') fputs("&gt;", stdout);
    else if (c == '&') fputs("&amp;", stdout);
    else if (c == '"') fputs("&quot;", stdout);
    else putchar(c);
}

static void en_ligne(const char *s) {
    int gras = 0, italique = 0;
    for (const char *p = s; *p; p++) {
        if (p[0] == '*' && p[1] == '*') { fputs(gras ? "</strong>" : "<strong>", stdout); gras = !gras; p++; continue; }
        if (*p == '*') { fputs(italique ? "</em>" : "<em>", stdout); italique = !italique; continue; }
        if (*p == '`') {
            const char *fin = strchr(p + 1, '`');
            if (fin) { fputs("<code>", stdout); for (const char *q = p + 1; q < fin; q++) echapper(*q); fputs("</code>", stdout); p = fin; continue; }
        }
        if (*p == '[') {
            const char *ferm = strchr(p, ']');
            const char *fin = NULL;
            if (ferm && ferm[1] == '(') {                    /* parenthèse fermante correspondante */
                int profondeur = 0;
                for (const char *q = ferm + 1; *q && !fin; q++) {
                    if (*q == '(') profondeur++;
                    else if (*q == ')' && --profondeur == 0) fin = q;
                }
            }
            if (fin) {
                fputs("<a href=\"", stdout);
                if (!strncmp(ferm + 2, "javascript:", 11)) fputs("#", stdout);          /* lien dangereux neutralisé */
                else for (const char *q = ferm + 2; q < fin; q++) echapper(*q);
                fputs("\">", stdout);
                for (const char *q = p + 1; q < ferm; q++) echapper(*q);
                fputs("</a>", stdout);
                p = fin;
                continue;
            }
        }
        echapper(*p);
    }
    if (gras) fputs("</strong>", stdout);          /* balises restées ouvertes */
    if (italique) fputs("</em>", stdout);
}

static void fermer(void) {
    if (bloc == PARAGRAPHE) puts("</p>");
    else if (bloc == LISTE) puts("</ul>");
    bloc = RIEN;
}

static void convertir_ligne(char *l) {
    l[strcspn(l, "\r\n")] = '\0';
    if (!strncmp(l, "```", 3)) {
        if (bloc == CODE) { puts("</code></pre>"); bloc = RIEN; } else { fermer(); fputs("<pre><code>", stdout); bloc = CODE; }
        return;
    }
    if (bloc == CODE) { for (char *p = l; *p; p++) echapper(*p); putchar('\n'); return; }
    if (*l == '\0') { fermer(); return; }
    int niveau = 0;
    while (l[niveau] == '#' && niveau < 6) niveau++;
    if (niveau && l[niveau] == ' ') { fermer(); printf("<h%d>", niveau); en_ligne(l + niveau + 1); printf("</h%d>\n", niveau); return; }
    if ((l[0] == '-' || l[0] == '*') && l[1] == ' ') {
        if (bloc != LISTE) { fermer(); puts("<ul>"); bloc = LISTE; }
        fputs("  <li>", stdout); en_ligne(l + 2); puts("</li>");
        return;
    }
    if (bloc != PARAGRAPHE) { fermer(); fputs("<p>", stdout); bloc = PARAGRAPHE; } else putchar(' ');
    en_ligne(l);
}

int main(int argc, char *argv[]) {
    char ligne[512];
    if (argc == 2) {
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
        while (fgets(ligne, sizeof ligne, f)) convertir_ligne(ligne);
        fclose(f);
        fermer();
        return 0;
    }
    if (argc > 2) { fprintf(stderr, "Usage : %s [fichier.md]\n", argv[0]); return 1; }
    const char *md[] = {
        "# Boutique ORVIX", "", "Bienvenue ! Nos produits sont **importés directement** et *garantis 6 mois*.",
        "Livraison à Cotonou et Calavi.", "", "## Nos best-sellers", "- **Écouteurs** Bluetooth : 4 500 FCFA",
        "- Lampe solaire, idéale pendant les coupures", "- Chargeur rapide `20 W`", "",
        "Contact : [WhatsApp](https://wa.me/22901000000) ou [ce lien piégé](javascript:alert(1))", "",
        "```", "if (prix < 5000) { promo = true; }", "```", "", "Attention : 2 < 3 & <script> n'est pas exécuté."};
    for (size_t i = 0; i < sizeof md / sizeof md[0]; i++) {
        snprintf(ligne, sizeof ligne, "%s", md[i]);
        convertir_ligne(ligne);
    }
    fermer();
    return 0;
}
