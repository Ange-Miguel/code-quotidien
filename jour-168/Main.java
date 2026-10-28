/*
 * Jour 168 — Budget personnel 50/30/20
 *
 * Problème : un étudiant ou un jeune actif reçoit un revenu (bourse, salaire,
 * missions freelance) qui fond sans qu'il sache où. La règle 50/30/20 est un
 * repère simple : 50 % pour les besoins (logement, nourriture, transport),
 * 30 % pour les envies (sorties, vêtements), 20 % pour l'épargne et les dettes.
 * Solution : chaque dépense du mois est rangée dans une catégorie, elle-même
 * associée à un des trois groupes. On compare la répartition réelle à la
 * cible, on affiche les écarts en barres, et on propose des ajustements
 * concrets : réduire en priorité les postes « envies » les plus importants
 * pour revenir à 20 % d'épargne. Montants d'exemple.
 * On apprend : les enums avec attributs, le regroupement avec les Collectors
 * (groupingBy, summingInt), les pourcentages et la génération de conseils.
 */
import java.util.*;
import java.util.stream.Collectors;

public class Main {
    enum Groupe { BESOINS(50), ENVIES(30), EPARGNE(20); final int cible; Groupe(int c) { cible = c; } }

    enum Categorie {
        LOGEMENT(Groupe.BESOINS), NOURRITURE(Groupe.BESOINS), TRANSPORT(Groupe.BESOINS), SANTE(Groupe.BESOINS), ETUDES(Groupe.BESOINS),
        SORTIES(Groupe.ENVIES), VETEMENTS(Groupe.ENVIES), FORFAIT_DATA(Groupe.ENVIES), CADEAUX(Groupe.ENVIES),
        EPARGNE(Groupe.EPARGNE), REMBOURSEMENT(Groupe.EPARGNE);
        final Groupe groupe; Categorie(Groupe g) { groupe = g; }
    }

    record Depense(Categorie categorie, String libelle, int montant) {}

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        int revenu = 150_000;
        if (args.length == 1) {
            try { revenu = Integer.parseInt(args[0]); if (revenu <= 0) throw new NumberFormatException(); }
            catch (NumberFormatException e) { System.err.println("Usage : java Main [revenu mensuel en FCFA]"); System.exit(1); }
        }
        List<Depense> mois = List.of(new Depense(Categorie.LOGEMENT, "chambre", 35_000), new Depense(Categorie.NOURRITURE, "marché + cantine", 32_000),
                new Depense(Categorie.TRANSPORT, "zem", 12_000), new Depense(Categorie.ETUDES, "photocopies", 4_000),
                new Depense(Categorie.SORTIES, "maquis, cinéma", 18_000), new Depense(Categorie.VETEMENTS, "chaussures", 15_000),
                new Depense(Categorie.FORFAIT_DATA, "internet mobile", 10_000), new Depense(Categorie.CADEAUX, "anniversaire", 7_000),
                new Depense(Categorie.EPARGNE, "tontine", 10_000));
        double facteur = revenu / 150_000.0;
        Map<Groupe, Integer> parGroupe = mois.stream().collect(Collectors.groupingBy(d -> d.categorie().groupe,
                () -> new EnumMap<>(Groupe.class), Collectors.summingInt(d -> (int) Math.round(d.montant() * facteur))));
        int depense = parGroupe.values().stream().mapToInt(Integer::intValue).sum();
        int reste = revenu - depense;
        parGroupe.merge(Groupe.EPARGNE, Math.max(0, reste), Integer::sum);       // ce qui reste est de l'épargne
        System.out.printf("Revenu du mois : %,d FCFA%n%n%-9s %10s %6s %6s%n", revenu, "Groupe", "Réel", "%", "Cible");
        for (Groupe g : Groupe.values()) {
            int m = parGroupe.getOrDefault(g, 0);
            double pct = 100.0 * m / revenu;
            System.out.printf("%-9s %,10d %5.0f%% %5d%%  %s%s%n", g, m, pct, g.cible, "█".repeat((int) Math.round(pct / 2.5)),
                    Math.abs(pct - g.cible) > 3 ? (pct > g.cible ? "  trop élevé" : "  insuffisant") : "  ✔");
        }
        int objectifEpargne = revenu * Groupe.EPARGNE.cible / 100, manque = objectifEpargne - parGroupe.get(Groupe.EPARGNE);
        if (manque > 0) {
            System.out.printf("%nPour atteindre 20 %% d'épargne (%,d FCFA), il manque %,d FCFA. Pistes :%n", objectifEpargne, manque);
            List<Depense> envies = mois.stream().filter(d -> d.categorie().groupe == Groupe.ENVIES)
                    .sorted(Comparator.comparingInt(Depense::montant).reversed()).toList();
            for (Depense d : envies) {
                if (manque <= 0) break;
                int reduction = Math.min(manque, (int) Math.round(d.montant() * facteur / 2 / 500) * 500);
                System.out.printf("   - réduire « %s » de %,d FCFA%n", d.libelle(), reduction);
                manque -= reduction;
            }
        } else System.out.println("\nObjectif d'épargne atteint, bravo !");
    }
}
