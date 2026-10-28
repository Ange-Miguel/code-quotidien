/*
 * Jour 066 — Système de réservation de places de bus
 *
 * Problème : une compagnie de transport Cotonou–Parakou vend des places sur
 * papier ; des places sont vendues deux fois et on ne sait pas combien il en
 * reste par trajet.
 * Solution : chaque voyage a un plan de sièges (4 par rangée, couloir au
 * milieu). On peut réserver un siège précis ou le premier libre (fenêtre de
 * préférence), annuler, et afficher le plan. Chaque réservation reçoit un
 * code unique. Les méthodes sont synchronisées pour éviter les doubles
 * ventes si plusieurs guichets réservent en même temps.
 * On apprend : les tableaux à deux dimensions, les Optional, la génération de
 * codes, la synchronisation des accès concurrents et l'affichage d'un plan.
 */
import java.util.*;

public class Main {
    static class Voyage {
        final String trajet;
        final String[][] sieges;            // null = libre, sinon code de réservation
        final Map<String, String> passagers = new HashMap<>();
        private int compteur = 0;

        Voyage(String trajet, int rangees) { this.trajet = trajet; sieges = new String[rangees][4]; }

        synchronized Optional<String> reserver(String passager, int rangee, int colonne) {
            if (rangee < 0 || rangee >= sieges.length || colonne < 0 || colonne > 3 || sieges[rangee][colonne] != null)
                return Optional.empty();
            String code = String.format("%s-%03d", trajet.substring(0, 3).toUpperCase(Locale.ROOT), ++compteur);
            sieges[rangee][colonne] = code;
            passagers.put(code, passager + " (siège " + nomSiege(rangee, colonne) + ")");
            return Optional.of(code);
        }

        synchronized Optional<String> reserverPremierLibre(String passager, boolean fenetre) {
            int[] colonnes = fenetre ? new int[]{0, 3, 1, 2} : new int[]{1, 2, 0, 3};
            for (int c : colonnes)
                for (int r = 0; r < sieges.length; r++)
                    if (sieges[r][c] == null) return reserver(passager, r, c);
            return Optional.empty();
        }

        synchronized boolean annuler(String code) {
            for (String[] rangee : sieges)
                for (int c = 0; c < 4; c++)
                    if (code.equals(rangee[c])) { rangee[c] = null; passagers.remove(code); return true; }
            return false;
        }

        static String nomSiege(int r, int c) { return (r + 1) + "" + "ABCD".charAt(c); }

        void plan() {
            int libres = 0;
            System.out.println("Plan du bus " + trajet + " (X = occupé)");
            for (int r = 0; r < sieges.length; r++) {
                StringBuilder sb = new StringBuilder(String.format("  %2d ", r + 1));
                for (int c = 0; c < 4; c++) {
                    if (c == 2) sb.append("  ");
                    sb.append(sieges[r][c] == null ? "[ ]" : "[X]");
                    if (sieges[r][c] == null) libres++;
                }
                System.out.println(sb);
            }
            System.out.printf("  %d place(s) libre(s) sur %d%n", libres, sieges.length * 4);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) {
            System.err.println("Ce programme est une démonstration : lancez-le sans argument.");
            System.exit(1);
        }
        Voyage v = new Voyage("Cotonou-Parakou", 6);
        System.out.println("Réservation 3B : " + v.reserver("Awa", 2, 1).orElse("refusée"));
        System.out.println("Réservation 3B à nouveau : " + v.reserver("Kofi", 2, 1).orElse("refusée (déjà prise)"));
        System.out.println("Premier siège fenêtre : " + v.reserverPremierLibre("Kofi", true).orElse("aucun"));

        // 4 guichets vendent en même temps : aucune double vente possible
        List<Thread> guichets = new ArrayList<>();
        for (int g = 0; g < 4; g++) {
            int numero = g;
            guichets.add(new Thread(() -> {
                for (int i = 0; i < 5; i++) v.reserverPremierLibre("Client G" + numero + "-" + i, i % 2 == 0);
            }));
        }
        for (Thread t : guichets) t.start();
        for (Thread t : guichets) t.join();

        String code = v.passagers.keySet().iterator().next();
        System.out.println("Annulation de " + code + " : " + (v.annuler(code) ? "ok" : "inconnu"));
        v.plan();
        System.out.println("Réservations enregistrées : " + v.passagers.size());
    }
}
