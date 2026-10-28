/*
 * Jour 193 — Mini-langage de calcul : analyseur et évaluateur
 *
 * Problème : comment fonctionne un langage de programmation ? Entre le texte
 * « total = prix * quantite » et le résultat, il y a trois étapes : découper
 * le texte en jetons (analyse lexicale), vérifier la structure et calculer
 * (analyse syntaxique et évaluation), en signalant clairement les erreurs.
 * Solution : un petit langage pour faire des calculs de gestion : variables,
 * affectation (=), les quatre opérations, %, parenthèses, puissance (^),
 * comparaisons (<, >, ==) qui valent 1 ou 0, et l'instruction « afficher ».
 * L'analyseur lexical produit des jetons ; l'analyseur à descente récursive
 * évalue directement selon la priorité des opérateurs. Les erreurs indiquent
 * la ligne et la colonne. Les variables sont stockées dans une petite table.
 * On apprend : l'analyse lexicale, la grammaire et la priorité des
 * opérateurs, la descente récursive, la gestion d'erreurs avec setjmp/longjmp
 * et une table de symboles.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <setjmp.h>

typedef enum { NOMBRE, IDENT, OP, FIN } TypeJeton;
typedef struct { TypeJeton type; double valeur; char texte[32]; int colonne; } Jeton;

static const char *src;
static int pos, ligne_num;
static Jeton j;
static jmp_buf erreur_saut;
static char vars_nom[32][32];
static double vars_val[32];
static int nb_vars;

static void erreur(const char *msg) {
    printf("   ✘ ligne %d, colonne %d : %s\n", ligne_num, j.colonne + 1, msg);
    longjmp(erreur_saut, 1);
}

static void suivant(void) {                                   /* analyse lexicale */
    while (src[pos] == ' ') pos++;
    j.colonne = pos;
    if (!src[pos]) { j.type = FIN; return; }
    if (isdigit((unsigned char)src[pos]) || src[pos] == '.') {
        char *fin;
        j.valeur = strtod(src + pos, &fin);
        pos = (int)(fin - src);
        j.type = NOMBRE;
        return;
    }
    if (isalpha((unsigned char)src[pos]) || src[pos] == '_') {
        int n = 0;
        while ((isalnum((unsigned char)src[pos]) || src[pos] == '_') && n < 31) j.texte[n++] = src[pos++];
        j.texte[n] = '\0';
        j.type = IDENT;
        return;
    }
    if (src[pos] == '=' && src[pos + 1] == '=') { strcpy(j.texte, "=="); pos += 2; j.type = OP; return; }
    if (strchr("+-*/%^()=<>", src[pos])) { j.texte[0] = src[pos++]; j.texte[1] = '\0'; j.type = OP; return; }
    erreur("caractère inattendu");
}

static int est(const char *op) { return j.type == OP && !strcmp(j.texte, op); }

static double *variable(const char *nom, int creer) {
    for (int i = 0; i < nb_vars; i++) if (!strcmp(vars_nom[i], nom)) return &vars_val[i];
    if (!creer) return NULL;
    if (nb_vars == 32) erreur("trop de variables");
    snprintf(vars_nom[nb_vars], 32, "%s", nom);
    vars_val[nb_vars] = 0;
    return &vars_val[nb_vars++];
}

static double expression(void);

static double primaire(void) {
    if (j.type == NOMBRE) { double v = j.valeur; suivant(); return v; }
    if (j.type == IDENT) {
        double *v = variable(j.texte, 0);
        if (!v) erreur("variable non définie");
        suivant();
        return *v;
    }
    if (est("(")) { suivant(); double v = expression(); if (!est(")")) erreur("« ) » attendue"); suivant(); return v; }
    if (est("-")) { suivant(); return -primaire(); }
    erreur("valeur attendue");
    return 0;
}

static double puissance(void) {
    double b = primaire();
    if (est("^")) { suivant(); return pow(b, puissance()); }  /* associatif à droite */
    return b;
}

static double terme(void) {
    double v = puissance();
    while (est("*") || est("/") || est("%")) {
        char op = j.texte[0];
        suivant();
        double d = puissance();
        if (op != '*' && d == 0) erreur("division par zéro");
        v = op == '*' ? v * d : op == '/' ? v / d : fmod(v, d);
    }
    return v;
}

static double somme(void) {
    double v = terme();
    while (est("+") || est("-")) { char op = j.texte[0]; suivant(); v = op == '+' ? v + terme() : v - terme(); }
    return v;
}

static double expression(void) {
    double v = somme();
    if (est("<")) { suivant(); return v < somme(); }
    if (est(">")) { suivant(); return v > somme(); }
    if (est("==")) { suivant(); return fabs(v - somme()) < 1e-9; }
    return v;
}

static void instruction(const char *texte) {
    src = texte; pos = 0;
    if (setjmp(erreur_saut)) return;
    suivant();
    if (j.type == FIN) return;
    if (j.type == IDENT && !strcmp(j.texte, "afficher")) {
        suivant();
        double v = expression();
        if (j.type != FIN) erreur("fin de ligne attendue");
        printf("   → %g\n", v);
        return;
    }
    if (j.type == IDENT) {
        char nom[32];
        int sauv_pos = pos;
        Jeton sauv = j;
        snprintf(nom, sizeof nom, "%s", j.texte);
        suivant();
        if (est("=")) {
            suivant();
            double v = expression();
            if (j.type != FIN) erreur("fin de ligne attendue");
            *variable(nom, 1) = v;
            return;
        }
        pos = sauv_pos; j = sauv;                             /* pas une affectation : on relit */
        pos -= (int)strlen(nom); suivant();
    }
    double v = expression();
    if (j.type != FIN) erreur("opérateur ou fin de ligne attendu");
    printf("   = %g\n", v);
}

int main(int argc, char *argv[]) {
    const char *programme[] = {
        "prix = 4500", "quantite = 12", "remise = 10", "total = prix * quantite * (1 - remise / 100)", "afficher total",
        "afficher total > 50000", "marge = (total - 30000) / total * 100", "afficher marge", "2 ^ 3 ^ 2",
        "17 % 5 + -3", "afficher benefice", "x = 10 / (quantite - 12)", "y = 3 +", "z = 5 $ 2"};
    int n = argc > 1 ? argc - 1 : (int)(sizeof programme / sizeof programme[0]);
    for (int i = 0; i < n; i++) {
        const char *l = argc > 1 ? argv[i + 1] : programme[i];
        ligne_num = i + 1;
        printf("%2d │ %s\n", ligne_num, l);
        instruction(l);
    }
    return 0;
}
