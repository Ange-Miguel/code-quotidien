/*
 * Jour 027 — Liste de courses en ligne de commande
 *
 * Problème : on oublie toujours quelque chose au marché de Dantokpa. Une liste
 * persistante, qu'on complète au fil de la semaine, règle le problème.
 * Solution : la liste est enregistrée dans un fichier texte (une ligne par
 * article : quantité;nom). Des commandes simples permettent d'ajouter,
 * retirer, cocher et afficher. Une LinkedHashMap garde l'ordre d'ajout et
 * fusionne les doublons.
 * On apprend : la persistance dans un fichier, les Map, la lecture/écriture
 * avec java.nio et la conception d'une petite interface en ligne de commande.
 */
import java.io.IOException;
import java.nio.file.*;
import java.util.*;

public class Main {
    static final Path FICHIER = Path.of("liste_courses.txt");
    final Map<String, Integer> articles = new LinkedHashMap<>();

    void charger(Path p) throws IOException {
        if (!Files.exists(p)) return;
        for (String ligne : Files.readAllLines(p)) {
            String[] parts = ligne.split(";", 2);
            if (parts.length == 2) articles.put(parts[1], Integer.parseInt(parts[0]));
        }
    }

    void sauver(Path p) throws IOException {
        List<String> lignes = new ArrayList<>();
        articles.forEach((nom, q) -> lignes.add(q + ";" + nom));
        Files.write(p, lignes);
    }

    void ajouter(String nom, int quantite) {
        if (quantite <= 0) throw new IllegalArgumentException("quantité invalide");
        articles.merge(nom.toLowerCase(Locale.ROOT).trim(), quantite, Integer::sum);
    }

    boolean retirer(String nom) {
        return articles.remove(nom.toLowerCase(Locale.ROOT).trim()) != null;
    }

    void afficher() {
        if (articles.isEmpty()) { System.out.println("  (liste vide)"); return; }
        articles.forEach((nom, q) -> System.out.printf("  [ ] %3d × %s%n", q, nom));
        System.out.printf("  %d article(s) différent(s)%n", articles.size());
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        Main liste = new Main();
        try {
            if (args.length == 0) {
                System.out.println("Démo (sans fichier) :");
                liste.ajouter("Tomates", 2);
                liste.ajouter("riz 25 kg", 1);
                liste.ajouter("Piment", 3);
                liste.ajouter("tomates", 1);
                liste.afficher();
                System.out.println("Retrait du piment : " + liste.retirer("piment"));
                liste.afficher();
                System.out.println("\nCommandes : ajouter <nom> [qté] | retirer <nom> | voir | vider");
                return;
            }
            liste.charger(FICHIER);
            switch (args[0]) {
                case "ajouter" -> liste.ajouter(args[1], args.length > 2 ? Integer.parseInt(args[2]) : 1);
                case "retirer" -> { if (!liste.retirer(args[1])) System.out.println("Absent : " + args[1]); }
                case "voir" -> { }
                case "vider" -> liste.articles.clear();
                default -> throw new IllegalArgumentException("commande inconnue : " + args[0]);
            }
            liste.sauver(FICHIER);
            liste.afficher();
        } catch (IllegalArgumentException | ArrayIndexOutOfBoundsException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.err.println("Usage : java Main ajouter <nom> [qté] | retirer <nom> | voir | vider");
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Fichier illisible : " + e.getMessage());
            System.exit(2);
        }
    }
}
