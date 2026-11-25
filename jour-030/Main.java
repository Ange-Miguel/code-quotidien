/*
 * Jour 030 — Chronomètre avec tours intermédiaires
 *
 * Problème : pour un entraînement sportif, un test de vitesse ou la mesure du
 * temps d'exécution d'un programme, on veut un chronomètre qui enregistre des
 * tours et donne le meilleur, le pire et la moyenne.
 * Solution : une classe Chronometre basée sur System.nanoTime (horloge
 * monotone, insensible aux changements d'heure). L'horloge est injectable via
 * une interface LongSupplier : la démo utilise une horloge simulée pour un
 * résultat reproductible, le mode --reel mesure les appuis sur Entrée.
 * On apprend : l'injection de dépendance, les interfaces fonctionnelles, les
 * horloges monotones et les statistiques simples sur une liste.
 */
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;
import java.util.function.LongSupplier;

public class Main {
    static class Chronometre {
        private final LongSupplier horloge;
        private long dernier = -1;
        final List<Double> tours = new ArrayList<>();

        Chronometre(LongSupplier horloge) { this.horloge = horloge; }

        void demarrer() { dernier = horloge.getAsLong(); tours.clear(); }

        double tour() {
            if (dernier < 0) throw new IllegalStateException("chronomètre non démarré");
            long maintenant = horloge.getAsLong();
            double secondes = (maintenant - dernier) / 1e9;
            dernier = maintenant;
            tours.add(secondes);
            return secondes;
        }

        void resume() {
            if (tours.isEmpty()) { System.out.println("Aucun tour."); return; }
            double min = tours.stream().mapToDouble(Double::doubleValue).min().orElse(0);
            double max = tours.stream().mapToDouble(Double::doubleValue).max().orElse(0);
            double total = tours.stream().mapToDouble(Double::doubleValue).sum();
            for (int i = 0; i < tours.size(); i++) {
                double t = tours.get(i);
                String marque = t == min ? " ← meilleur" : t == max ? " ← pire" : "";
                System.out.printf("  Tour %2d : %s%s%n", i + 1, formater(t), marque);
            }
            System.out.printf("  Total   : %s%n  Moyenne : %s%n", formater(total), formater(total / tours.size()));
        }
    }

    static String formater(double s) {
        return String.format("%02d:%06.3f", (int) (s / 60), s % 60);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length == 1 && args[0].equals("--reel")) {
            Chronometre c = new Chronometre(System::nanoTime);
            Scanner in = new Scanner(System.in);
            System.out.println("Entrée pour démarrer…");
            in.nextLine();
            c.demarrer();
            System.out.println("Entrée = tour, q + Entrée = arrêt");
            while (in.hasNextLine() && !in.nextLine().trim().equalsIgnoreCase("q"))
                System.out.println("   " + formater(c.tour()));
            c.tour();
            c.resume();
            return;
        }
        if (args.length > 0) {
            System.err.println("Usage : java Main [--reel]");
            System.exit(1);
        }
        long[] temps = {0L, 61_420_000_000L, 119_800_000_000L, 182_050_000_000L, 243_300_000_000L, 301_950_000_000L};
        int[] index = {0};
        Chronometre c = new Chronometre(() -> temps[index[0]++]);
        System.out.println("Démo : 400 m en 5 tours (horloge simulée)");
        c.demarrer();
        for (int i = 0; i < 5; i++) c.tour();
        c.resume();
    }
}
