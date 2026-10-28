/*
 * Jour 076 — Résolveur de sudoku (backtracking)
 *
 * Problème : résoudre automatiquement n'importe quelle grille de sudoku, même
 * difficile, et comprendre la technique générale du « retour sur trace »
 * (backtracking), utilisée pour les emplois du temps, les labyrinthes…
 * Solution : on cherche une case vide ; on essaie chaque chiffre autorisé
 * (absent de la ligne, de la colonne et du bloc 3×3) ; on continue
 * récursivement ; en cas d'impasse, on efface et on essaie le chiffre suivant.
 * Optimisation : on choisit toujours la case vide qui a le moins de
 * possibilités, ce qui réduit énormément l'exploration. Des masques de bits
 * mémorisent les chiffres déjà utilisés.
 * On apprend : le backtracking, la récursivité, les masques de bits et
 * l'heuristique « variable la plus contrainte d'abord ».
 */
#include <stdio.h>
#include <string.h>

static int g[9][9];
static unsigned lig[9], col[9], bloc[9];   /* bit k = chiffre k déjà utilisé */
static long essais;

static int nb_bits(unsigned x) { int n = 0; while (x) { n += x & 1; x >>= 1; } return n; }

static void poser(int l, int c, int v, int mettre) {
    unsigned m = 1u << v;
    int b = l / 3 * 3 + c / 3;
    if (mettre) { g[l][c] = v; lig[l] |= m; col[c] |= m; bloc[b] |= m; }
    else { g[l][c] = 0; lig[l] &= ~m; col[c] &= ~m; bloc[b] &= ~m; }
}

static int resoudre(void) {
    int bl = -1, bc = -1, meilleur = 10;
    unsigned possibles_meilleur = 0;
    for (int l = 0; l < 9; l++)
        for (int c = 0; c < 9; c++) {
            if (g[l][c]) continue;
            unsigned possibles = ~(lig[l] | col[c] | bloc[l / 3 * 3 + c / 3]) & 0x3FE;
            int n = nb_bits(possibles);
            if (n < meilleur) { meilleur = n; bl = l; bc = c; possibles_meilleur = possibles; }
        }
    if (bl < 0) return 1;            /* plus de case vide : résolu */
    for (int v = 1; v <= 9; v++) {
        if (!(possibles_meilleur & (1u << v))) continue;
        essais++;
        poser(bl, bc, v, 1);
        if (resoudre()) return 1;
        poser(bl, bc, v, 0);         /* retour sur trace */
    }
    return 0;
}

static int charger(const char *texte) {
    if (strlen(texte) != 81) return 0;
    memset(lig, 0, sizeof lig); memset(col, 0, sizeof col); memset(bloc, 0, sizeof bloc);
    for (int i = 0; i < 81; i++) {
        int l = i / 9, c = i % 9;
        char ch = texte[i];
        g[l][c] = 0;
        if (ch == '.' || ch == '0') continue;
        if (ch < '1' || ch > '9') return 0;
        int v = ch - '0';
        if ((lig[l] | col[c] | bloc[l / 3 * 3 + c / 3]) & (1u << v)) return 0;   /* doublon */
        poser(l, c, v, 1);
    }
    return 1;
}

static void afficher(void) {
    for (int l = 0; l < 9; l++) {
        if (l && l % 3 == 0) puts("   ------+-------+------");
        printf("   ");
        for (int c = 0; c < 9; c++) printf("%s%c ", c && c % 3 == 0 ? "| " : "", g[l][c] ? '0' + g[l][c] : '.');
        putchar('\n');
    }
}

static int traiter(const char *nom, const char *grille) {
    if (!charger(grille)) { fprintf(stderr, "%s : grille invalide (81 caractères, 1-9 ou .)\n", nom); return 0; }
    printf("%s :\n", nom);
    afficher();
    essais = 0;
    if (!resoudre()) { printf("   Aucune solution.\n\n"); return 0; }
    printf("Solution (%ld essais) :\n", essais);
    afficher();
    putchar('\n');
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 2) return traiter("Votre grille", argv[1]) ? 0 : 1;
    if (argc > 2) { fprintf(stderr, "Usage : %s [grille de 81 caractères]\n", argv[0]); return 1; }
    traiter("Grille facile",
            "53..7....6..195....98....6.8...6...34..8.3..17...2...6.6....28....419..5....8..79");
    traiter("Grille très difficile",
            "8..........36......7..9.2...5...7.......457.....1...3...1....68..85...1..9....4..");
    return 0;
}
