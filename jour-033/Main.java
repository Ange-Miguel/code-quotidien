/*
 * Jour 033 — Recherche du maximum et du minimum dans une liste de prix
 *
 * Problème : on compare les prix d'un même produit chez plusieurs fournisseurs
 * ou vendeurs : qui est le moins cher, le plus cher, et quel est l'écart ?
 * Solution : un seul parcours de la liste suffit pour trouver à la fois le
 * minimum et le maximum (et leur position). On calcule aussi l'économie
 * réalisée en choisissant le moins cher et le prix médian du marché. On
 * compare avec l'API Stream de Java.
 * On apprend : l'algorithme de recherche min/max en un passage, les records,
 * les comparateurs et l'API Stream.
 */
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

public class Main {
    record Offre(String vendeur, int prix) {}

    static void analyser(String produit, List<Offre> offres) {
        if (offres.isEmpty()) throw new IllegalArgumentException("aucune offre");
        Offre min = offres.get(0), max = offres.get(0);
        for (Offre o : offres) {               // un seul passage
            if (o.prix() < min.prix()) min = o;
            if (o.prix() > max.prix()) max = o;
        }
        // Vérification avec les streams
        Offre minStream = offres.stream().min(Comparator.comparingInt(Offre::prix)).orElseThrow();
        if (minStream.prix() != min.prix()) throw new IllegalStateException("incohérence");

        int[] tries = offres.stream().mapToInt(Offre::prix).sorted().toArray();
        double mediane = tries.length % 2 == 1 ? tries[tries.length / 2]
                : (tries[tries.length / 2 - 1] + tries[tries.length / 2]) / 2.0;

        System.out.println(produit);
        for (Offre o : offres) {
            String tag = o == min ? "  ← moins cher" : o == max ? "  ← plus cher" : "";
            System.out.printf("  %-18s %,8d FCFA%s%n", o.vendeur(), o.prix(), tag);
        }
        System.out.printf("  Écart : %,d FCFA (%.0f %%) | prix médian : %,.0f FCFA%n%n",
                max.prix() - min.prix(), 100.0 * (max.prix() - min.prix()) / min.prix(), mediane);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length == 0) {
            analyser("Bouteille de gaz 12,5 kg (recharge)", List.of(
                    new Offre("Godomey", 5600), new Offre("Calavi Kpota", 5500),
                    new Offre("Akpakpa", 5800), new Offre("Cadjèhoun", 5650)));
            analyser("Smartphone d'entrée de gamme", List.of(
                    new Offre("Boutique Dantokpa", 62000), new Offre("En ligne (import)", 48500),
                    new Offre("Revendeur Ganhi", 70000)));
            return;
        }
        try {
            List<Offre> offres = new java.util.ArrayList<>();
            for (int i = 0; i < args.length; i++) offres.add(new Offre("Offre " + (i + 1), Integer.parseInt(args[i])));
            analyser("Vos prix", offres);
        } catch (NumberFormatException e) {
            System.err.println("Prix invalide. Usage : java Main prix1 prix2 ...");
            System.exit(1);
        }
    }
}
