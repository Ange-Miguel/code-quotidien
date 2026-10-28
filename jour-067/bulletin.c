/*
 * Jour 067 — Gestion des notes d'étudiants avec bulletin
 *
 * Problème : produire pour chaque étudiant un bulletin clair : notes de
 * devoirs et d'examen par matière, moyenne pondérée, rang dans la classe,
 * appréciation, et validation des crédits (UE) selon les règles du système
 * LMD (moyenne ≥ 10 dans l'UE).
 * Solution : un tableau à deux dimensions [étudiants][matières] de structures
 * de notes. Moyenne de matière = 40 % devoir + 60 % examen ; moyenne générale
 * pondérée par les crédits ; crédits acquis si moyenne de matière ≥ 10.
 * Le rang est calculé en comparant les moyennes.
 * On apprend : les tableaux 2D de structures, les constantes partagées,
 * l'impression formatée d'un document et le calcul de rang.
 */
#include <stdio.h>
#include <string.h>

#define NB_MATIERES 4
#define NB_ETUDIANTS 4

typedef struct { const char *nom; int credits; } Matiere;
typedef struct { double devoir, examen; } Note;
typedef struct { const char *nom; Note notes[NB_MATIERES]; double moyenne; int credits; } Etudiant;

static const Matiere MATIERES[NB_MATIERES] = {
    {"Électronique analogique", 6}, {"Programmation C", 5}, {"Mathématiques", 5}, {"Anglais technique", 2}
};

static double moyenne_matiere(Note n) { return 0.4 * n.devoir + 0.6 * n.examen; }

static const char *appreciation(double m) {
    if (m >= 16) return "Excellent travail, félicitations.";
    if (m >= 14) return "Très bon semestre.";
    if (m >= 12) return "Bon travail, continuez.";
    if (m >= 10) return "Résultats justes, il faut s'investir davantage.";
    return "Semestre insuffisant, un rattrapage s'impose.";
}

static void calculer(Etudiant *e) {
    double somme = 0;
    int total_credits = 0;
    e->credits = 0;
    for (int m = 0; m < NB_MATIERES; m++) {
        double mm = moyenne_matiere(e->notes[m]);
        somme += mm * MATIERES[m].credits;
        total_credits += MATIERES[m].credits;
        if (mm >= 10) e->credits += MATIERES[m].credits;
    }
    e->moyenne = somme / total_credits;
}

static void bulletin(const Etudiant *e, int rang) {
    int total = 0;
    for (int m = 0; m < NB_MATIERES; m++) total += MATIERES[m].credits;
    printf("==================== BULLETIN — Semestre 3 ====================\n");
    printf("Étudiant : %-20s Rang : %d/%d\n\n", e->nom, rang, NB_ETUDIANTS);
    printf("%-26s %7s %7s %8s %8s\n", "Matière (crédits)", "Devoir", "Examen", "Moyenne", "Crédits");
    for (int m = 0; m < NB_MATIERES; m++) {
        double mm = moyenne_matiere(e->notes[m]);
        printf("%-23s(%d) %7.2f %7.2f %8.2f %8s\n", MATIERES[m].nom, MATIERES[m].credits,
               e->notes[m].devoir, e->notes[m].examen, mm, mm >= 10 ? "acquis" : "—");
    }
    printf("\nMoyenne générale : %.2f/20 | Crédits : %d/%d\n", e->moyenne, e->credits, total);
    printf("Appréciation : %s\n\n", appreciation(e->moyenne));
}

int main(int argc, char *argv[]) {
    Etudiant classe[NB_ETUDIANTS] = {
        {"Akpanon Ange", {{14, 15.5}, {17, 16}, {12, 11}, {15, 14}}, 0, 0},
        {"Dossou Carine", {{12, 13}, {13, 12}, {16, 15.5}, {14, 16}}, 0, 0},
        {"Houngbo Serge", {{9, 8.5}, {11, 12}, {7, 9}, {13, 12}}, 0, 0},
        {"Adjovi Mireille", {{16, 17}, {15, 14}, {14, 15}, {17, 18}}, 0, 0},
    };
    for (int i = 0; i < NB_ETUDIANTS; i++) calculer(&classe[i]);
    int affiches = 0;
    for (int i = 0; i < NB_ETUDIANTS; i++) {
        if (argc > 1 && strstr(classe[i].nom, argv[1]) == NULL) continue;
        int rang = 1;
        for (int j = 0; j < NB_ETUDIANTS; j++) if (classe[j].moyenne > classe[i].moyenne) rang++;
        bulletin(&classe[i], rang);
        affiches++;
    }
    if (!affiches) {
        fprintf(stderr, "Aucun étudiant ne correspond à « %s ».\n", argv[1]);
        return 1;
    }
    return 0;
}
