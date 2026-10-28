/*
 * Jour 088 — Organisation automatique des téléchargements par extension
 *
 * Problème : le dossier « Téléchargements » finit par contenir des centaines
 * de PDF, images, vidéos, archives et installateurs mélangés.
 * Solution : on parcourt le dossier avec opendir/readdir, on détermine la
 * catégorie de chaque fichier d'après son extension (table de
 * correspondance), on crée les sous-dossiers nécessaires (Documents, Images,
 * Vidéos, Archives, Programmes, Autres) et on déplace les fichiers avec
 * rename. Si un fichier du même nom existe déjà, on ajoute un suffixe. Par
 * défaut c'est une simulation ; --appliquer effectue les déplacements. La
 * démo travaille dans un dossier temporaire.
 * On apprend : l'API POSIX des répertoires (opendir, readdir, stat, mkdir,
 * rename), la gestion des chemins en C et le mode simulation.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct { const char *ext; const char *dossier; } Regle;
static const Regle REGLES[] = {
    {"pdf", "Documents"}, {"docx", "Documents"}, {"xlsx", "Documents"}, {"pptx", "Documents"}, {"txt", "Documents"},
    {"jpg", "Images"}, {"jpeg", "Images"}, {"png", "Images"}, {"gif", "Images"}, {"webp", "Images"},
    {"mp4", "Vidéos"}, {"mkv", "Vidéos"}, {"mov", "Vidéos"}, {"mp3", "Audio"}, {"wav", "Audio"},
    {"zip", "Archives"}, {"rar", "Archives"}, {"7z", "Archives"}, {"exe", "Programmes"}, {"apk", "Programmes"},
    {"c", "Code"}, {"py", "Code"}, {"java", "Code"},
};

static const char *categorie(const char *nom) {
    const char *point = strrchr(nom, '.');
    if (!point || point == nom) return "Autres";
    for (size_t i = 0; i < sizeof REGLES / sizeof REGLES[0]; i++)
        if (strcasecmp(point + 1, REGLES[i].ext) == 0) return REGLES[i].dossier;
    return "Autres";
}

static int ranger(const char *dossier, int appliquer) {
    DIR *d = opendir(dossier);
    if (!d) { perror(dossier); return -1; }
    struct dirent *e;
    int n = 0;
    char source[1300], cible_dir[1300], cible[1600];
    while ((e = readdir(d)) != NULL) {
        struct stat st;
        snprintf(source, sizeof source, "%s/%s", dossier, e->d_name);
        if (e->d_name[0] == '.' || stat(source, &st) != 0 || !S_ISREG(st.st_mode)) continue;
        const char *cat = categorie(e->d_name);
        snprintf(cible_dir, sizeof cible_dir, "%s/%s", dossier, cat);
        snprintf(cible, sizeof cible, "%s/%s", cible_dir, e->d_name);
        for (int k = 2; access(cible, F_OK) == 0; k++)   /* éviter d'écraser un fichier */
            snprintf(cible, sizeof cible, "%s/%d_%s", cible_dir, k, e->d_name);
        printf("  %-26s → %s/\n", e->d_name, cat);
        if (appliquer) {
            mkdir(cible_dir, 0755);
            if (rename(source, cible) != 0) { perror("rename"); continue; }
        }
        n++;
    }
    closedir(d);
    printf("%d fichier(s) %s\n", n, appliquer ? "rangé(s)" : "à ranger (simulation, ajoutez --appliquer)");
    return n;
}

static void creer(const char *dossier, const char *nom) {
    char chemin[1024];
    snprintf(chemin, sizeof chemin, "%s/%s", dossier, nom);
    FILE *f = fopen(chemin, "w");
    if (f) { fputs("exemple", f); fclose(f); }
}

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        int appliquer = argc == 3 && strcmp(argv[2], "--appliquer") == 0;
        return ranger(argv[1], appliquer) < 0 ? 1 : 0;
    }
    char modele[] = "/tmp/telechargementsXXXXXX";
    char *dossier = mkdtemp(modele);
    if (!dossier) { perror("mkdtemp"); return 2; }
    const char *fichiers[] = {"Cours_Electronique.pdf", "facture_octobre.xlsx", "IMG-20261012.jpg", "logo.PNG",
                              "tuto_arduino.mp4", "projet.zip", "WhatsApp.apk", "main.c", "notes", "chanson.mp3"};
    for (int i = 0; i < 10; i++) creer(dossier, fichiers[i]);
    puts("Simulation :");
    ranger(dossier, 0);
    puts("\nApplication :");
    ranger(dossier, 1);
    char commande[1100];
    snprintf(commande, sizeof commande, "rm -rf '%s'", dossier);   /* nettoyage de la démo */
    if (system(commande) != 0) fprintf(stderr, "Nettoyage incomplet de %s\n", dossier);
    return 0;
}
