/*
 * Jour 156 — Stock de pharmacie avec dates de péremption
 *
 * Problème : une pharmacie ou un centre de santé perd de l'argent avec les
 * médicaments périmés et peut mettre des patients en danger s'ils sont
 * délivrés par erreur. Un même médicament existe souvent en plusieurs lots
 * avec des dates différentes.
 * Solution : le stock est organisé par lots (numéro, quantité, date de
 * péremption). La délivrance suit la règle FEFO (First Expired, First Out) :
 * on sert toujours le lot qui expire le plus tôt, grâce à une file de
 * priorité par produit. Les lots périmés sont bloqués, ceux qui expirent dans
 * moins de 90 jours sont signalés, et on calcule la valeur à risque. Produits
 * et lots fictifs.
 * On apprend : la règle FEFO, PriorityQueue par clé dans une Map,
 * LocalDate et ChronoUnit, et le blocage automatique d'articles non conformes.
 */
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import java.util.*;

public class Main {
    static final class Lot {
        final String produit, numero; int quantite; final LocalDate peremption; final int prixUnitaire;
        Lot(String produit, String numero, int quantite, LocalDate peremption, int prixUnitaire) {
            this.produit = produit; this.numero = numero; this.quantite = quantite; this.peremption = peremption; this.prixUnitaire = prixUnitaire;
        }
    }

    final Map<String, PriorityQueue<Lot>> stock = new TreeMap<>();
    LocalDate aujourdhui;

    void recevoir(Lot l) { stock.computeIfAbsent(l.produit, k -> new PriorityQueue<>(Comparator.comparing((Lot x) -> x.peremption))).add(l); }

    String delivrer(String produit, int quantite) {
        PriorityQueue<Lot> lots = stock.get(produit);
        if (lots == null) return "produit inconnu";
        while (!lots.isEmpty() && !lots.peek().peremption.isAfter(aujourdhui)) {
            Lot p = lots.poll();
            System.out.printf("   ⛔ lot %s de %s périmé le %s : bloqué, retiré du stock (%d unités)%n", p.numero, produit, p.peremption, p.quantite);
        }
        int dispo = lots.stream().mapToInt(l -> l.quantite).sum();
        if (dispo < quantite) return "stock insuffisant (" + dispo + " disponibles)";
        StringBuilder detail = new StringBuilder();
        while (quantite > 0) {
            Lot l = lots.peek();
            int pris = Math.min(quantite, l.quantite);
            l.quantite -= pris; quantite -= pris;
            detail.append(pris).append(" du lot ").append(l.numero).append(" (").append(l.peremption).append(") ");
            if (l.quantite == 0) lots.poll();
        }
        return "servi : " + detail.toString().trim();
    }

    void rapport() {
        long risque = 0;
        System.out.println("\nÉtat du stock au " + aujourdhui + " :");
        for (var e : stock.entrySet())
            for (Lot l : e.getValue().stream().sorted(Comparator.comparing((Lot x) -> x.peremption)).toList()) {
                long jours = ChronoUnit.DAYS.between(aujourdhui, l.peremption);
                String etat = jours <= 0 ? "PÉRIMÉ" : jours <= 90 ? "expire dans " + jours + " j ⚠" : "ok";
                if (jours <= 90) risque += (long) l.quantite * l.prixUnitaire;
                System.out.printf("   %-24s lot %-7s %4d u.  %s  %s%n", e.getKey(), l.numero, l.quantite, l.peremption, etat);
            }
        System.out.printf("Valeur à écouler en priorité (≤ 90 jours) : %,d FCFA%n", risque);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        Main p = new Main();
        p.aujourdhui = LocalDate.of(2026, 10, 28);
        p.recevoir(new Lot("Paracétamol 500 mg (boîte)", "P2311", 40, LocalDate.of(2027, 6, 30), 600));
        p.recevoir(new Lot("Paracétamol 500 mg (boîte)", "P2207", 15, LocalDate.of(2026, 12, 15), 600));
        p.recevoir(new Lot("Amoxicilline 500 mg (boîte)", "A0901", 10, LocalDate.of(2026, 10, 20), 1800));
        p.recevoir(new Lot("Amoxicilline 500 mg (boîte)", "A1102", 25, LocalDate.of(2027, 3, 31), 1800));
        p.recevoir(new Lot("Sels de réhydratation (sachet)", "S0455", 120, LocalDate.of(2027, 1, 10), 150));
        String[][] ventes = {{"Paracétamol 500 mg (boîte)", "20"}, {"Amoxicilline 500 mg (boîte)", "3"},
                {"Sels de réhydratation (sachet)", "200"}, {"Vitamine C", "1"}};
        for (String[] v : ventes) {
            System.out.printf("Délivrance de %s × %s :%n", v[1], v[0]);
            System.out.println("   → " + p.delivrer(v[0], Integer.parseInt(v[1])));
        }
        p.rapport();
    }
}
