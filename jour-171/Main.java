/*
 * Jour 171 — Gestion d'association : membres et cotisations
 *
 * Problème : une association d'étudiants, un club ou une amicale suit ses
 * membres et leurs cotisations sur un cahier : qui est à jour, qui doit
 * combien, combien a-t-on encaissé, qui peut voter à l'assemblée générale ?
 * Solution : chaque membre a une catégorie (étudiant, actif, bienfaiteur)
 * qui fixe sa cotisation mensuelle, une date d'adhésion et la liste de ses
 * paiements. On calcule ce qui est dû depuis l'adhésion, les arriérés, le
 * statut (à jour, en retard, suspendu après 3 mois d'impayés), la liste
 * électorale (membres à jour depuis au moins 3 mois) et un état de caisse
 * mensuel. Membres et montants fictifs.
 * On apprend : YearMonth et les calculs par mois, les records et les streams,
 * les règles d'un règlement intérieur traduites en code.
 */
import java.time.YearMonth;
import java.time.temporal.ChronoUnit;
import java.util.*;

public class Main {
    enum Categorie { ETUDIANT(1_000), ACTIF(3_000), BIENFAITEUR(10_000); final int mensuel; Categorie(int m) { mensuel = m; } }
    record Paiement(YearMonth mois, int montant) {}
    record Membre(String nom, Categorie cat, YearMonth adhesion, List<Paiement> paiements) {
        int du(YearMonth jusqua) { return (int) (ChronoUnit.MONTHS.between(adhesion, jusqua) + 1) * cat.mensuel; }
        int paye() { return paiements.stream().mapToInt(Paiement::montant).sum(); }
        int arrieres(YearMonth m) { return Math.max(0, du(m) - paye()); }
        String statut(YearMonth m) {
            int moisRetard = arrieres(m) / cat.mensuel;
            return moisRetard == 0 ? "à jour" : moisRetard < 3 ? "en retard (" + moisRetard + " mois)" : "SUSPENDU (" + moisRetard + " mois)";
        }
        boolean electeur(YearMonth m) { return arrieres(m) == 0 && ChronoUnit.MONTHS.between(adhesion, m) >= 3; }
    }

    static List<Paiement> paiements(YearMonth debut, int nbMois, int montant) {
        List<Paiement> l = new ArrayList<>();
        for (int i = 0; i < nbMois; i++) l.add(new Paiement(debut.plusMonths(i), montant));
        return l;
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        YearMonth aujourdhui = YearMonth.of(2026, 10);
        List<Membre> membres = List.of(
                new Membre("Ange", Categorie.ETUDIANT, YearMonth.of(2026, 1), paiements(YearMonth.of(2026, 1), 10, 1_000)),
                new Membre("Awa", Categorie.ETUDIANT, YearMonth.of(2026, 3), paiements(YearMonth.of(2026, 3), 6, 1_000)),
                new Membre("M. Kpade", Categorie.ACTIF, YearMonth.of(2025, 11), paiements(YearMonth.of(2025, 11), 7, 3_000)),
                new Membre("Mme Zinsou", Categorie.BIENFAITEUR, YearMonth.of(2026, 1), List.of(new Paiement(YearMonth.of(2026, 1), 100_000))),
                new Membre("Kofi", Categorie.ETUDIANT, YearMonth.of(2026, 9), paiements(YearMonth.of(2026, 9), 2, 1_000)));
        System.out.printf("Situation au %s :%n%-12s %-12s %9s %9s %9s  %s%n", aujourdhui, "Membre", "Catégorie", "Dû", "Payé", "Arriérés", "Statut");
        for (Membre m : membres)
            System.out.printf("%-12s %-12s %,9d %,9d %,9d  %s%n", m.nom(), m.cat(), m.du(aujourdhui), m.paye(), m.arrieres(aujourdhui), m.statut(aujourdhui));
        System.out.println("\nListe électorale (à jour et membre depuis ≥ 3 mois) : "
                + membres.stream().filter(m -> m.electeur(aujourdhui)).map(Membre::nom).toList());
        Map<YearMonth, Integer> caisse = new TreeMap<>();
        membres.forEach(m -> m.paiements().forEach(p -> caisse.merge(p.mois(), p.montant(), Integer::sum)));
        System.out.println("\nEncaissements par mois :");
        caisse.forEach((mois, total) -> System.out.printf("   %s %,8d FCFA %s%n", mois, total, "▇".repeat(Math.min(40, total / 2_500))));
        System.out.printf("Total encaissé : %,d FCFA | arriérés à recouvrer : %,d FCFA%n",
                caisse.values().stream().mapToInt(Integer::intValue).sum(), membres.stream().mapToInt(m -> m.arrieres(aujourdhui)).sum());
    }
}
