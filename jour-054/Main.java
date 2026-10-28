/*
 * Jour 054 — Gestionnaire de tâches (to-do) avec priorités, sauvegardé en fichier
 *
 * Problème : entre les cours, les projets freelance et les commandes, on
 * oublie des tâches importantes et on traite d'abord les plus faciles.
 * Solution : chaque tâche a une priorité (HAUTE, MOYENNE, BASSE) et une
 * échéance. Une PriorityQueue les ordonne : d'abord la priorité, puis la date
 * la plus proche. Les tâches sont enregistrées dans un fichier texte et les
 * tâches en retard sont signalées.
 * On apprend : PriorityQueue et Comparator composés, les enums, LocalDate et
 * la sérialisation simple d'objets dans un fichier.
 */
import java.io.IOException;
import java.nio.file.*;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import java.util.*;

public class Main {
    enum Priorite { HAUTE, MOYENNE, BASSE }

    record Tache(String titre, Priorite priorite, LocalDate echeance) {
        String versLigne() { return priorite + "|" + echeance + "|" + titre; }
        static Tache depuisLigne(String l) {
            String[] p = l.split("\\|", 3);
            return new Tache(p[2], Priorite.valueOf(p[0]), LocalDate.parse(p[1]));
        }
    }

    static final Comparator<Tache> ORDRE = Comparator.comparing(Tache::priorite).thenComparing(Tache::echeance);

    static void sauver(Path f, Collection<Tache> taches) throws IOException {
        Files.write(f, taches.stream().map(Tache::versLigne).toList());
    }

    static PriorityQueue<Tache> charger(Path f) throws IOException {
        PriorityQueue<Tache> file = new PriorityQueue<>(ORDRE);
        for (String l : Files.readAllLines(f)) if (!l.isBlank()) file.add(Tache.depuisLigne(l));
        return file;
    }

    static void afficher(PriorityQueue<Tache> file, LocalDate aujourdHui) {
        PriorityQueue<Tache> copie = new PriorityQueue<>(file);
        int n = 1;
        while (!copie.isEmpty()) {
            Tache t = copie.poll();
            long jours = ChronoUnit.DAYS.between(aujourdHui, t.echeance());
            String etat = jours < 0 ? "EN RETARD de " + (-jours) + " j" : jours == 0 ? "aujourd'hui" : "dans " + jours + " j";
            System.out.printf("%2d. [%-7s] %-38s %s (%s)%n", n++, t.priorite(), t.titre(), t.echeance(), etat);
        }
    }

    public static void main(String[] args) throws IOException {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) {
            try {
                afficher(charger(Path.of(args[0])), LocalDate.now());
            } catch (IOException | RuntimeException e) {
                System.err.println("Fichier de tâches illisible : " + args[0]);
                System.exit(1);
            }
            return;
        }
        LocalDate ref = LocalDate.of(2026, 10, 28);
        List<Tache> taches = List.of(
                new Tache("Réviser électronique numérique", Priorite.HAUTE, ref.plusDays(3)),
                new Tache("Livrer l'animation au client", Priorite.HAUTE, ref.minusDays(1)),
                new Tache("Répondre aux messages WhatsApp", Priorite.BASSE, ref),
                new Tache("Commander le stock d'écouteurs", Priorite.MOYENNE, ref.plusDays(7)),
                new Tache("Pousser le code du jour", Priorite.MOYENNE, ref));
        Path tmp = Files.createTempFile("taches", ".txt");
        try {
            sauver(tmp, taches);
            System.out.println("Tâches par ordre de traitement :");
            PriorityQueue<Tache> file = charger(tmp);
            afficher(file, ref);
            System.out.println("\nÀ faire maintenant : " + file.peek().titre());
        } finally {
            Files.deleteIfExists(tmp);
        }
    }
}
