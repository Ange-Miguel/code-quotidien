/*
 * Jour 012 — Compteur de mots, lignes et caractères d'un texte
 *
 * Problème : un devoir, une description de produit ou un post a souvent une
 * limite de mots ou de caractères ; il faut pouvoir compter précisément.
 * Solution : on lit un fichier (ou un texte d'exemple), on compte les lignes,
 * les mots (séquences séparées par des espaces), les caractères avec et sans
 * espaces, et on estime le temps de lecture (200 mots par minute).
 * On apprend : la lecture de fichiers avec java.nio, les expressions
 * régulières pour découper un texte et la gestion des exceptions d'E/S.
 */
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Locale;

public class Main {
    static final String EXEMPLE = """
            Abomey-Calavi est une commune du sud du Bénin.
            Elle accueille le campus principal de l'Université d'Abomey-Calavi.
            Chaque matin, des milliers d'étudiants y arrivent en zem.
            """;

    record Statistiques(int lignes, int mots, int caracteres, int sansEspaces) {}

    static Statistiques compter(String texte) {
        String nettoye = texte.strip();
        int lignes = nettoye.isEmpty() ? 0 : nettoye.split("\\R").length;
        int mots = nettoye.isEmpty() ? 0 : nettoye.split("\\s+").length;
        int sansEspaces = nettoye.replaceAll("\\s", "").length();
        return new Statistiques(lignes, mots, nettoye.length(), sansEspaces);
    }

    static void afficher(String nom, String texte) {
        Statistiques s = compter(texte);
        double minutes = s.mots() / 200.0;
        System.out.println("Texte : " + nom);
        System.out.printf("  Lignes                : %d%n", s.lignes());
        System.out.printf("  Mots                  : %d%n", s.mots());
        System.out.printf("  Caractères            : %d%n", s.caracteres());
        System.out.printf("  Caractères sans espace: %d%n", s.sansEspaces());
        System.out.printf("  Lecture estimée       : %.1f min%n", minutes);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length == 0) {
            afficher("exemple intégré", EXEMPLE);
            return;
        }
        for (String chemin : args) {
            try {
                afficher(chemin, Files.readString(Path.of(chemin)));
            } catch (IOException e) {
                System.err.println("Impossible de lire « " + chemin + " » : " + e.getClass().getSimpleName());
                System.exit(1);
            }
        }
    }
}
