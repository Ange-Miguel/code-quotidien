/*
 * Jour 078 — Sac à dos : choisir les produits les plus rentables à importer
 *
 * Problème : un importateur dispose d'un budget limité (ou d'un colis limité en
 * kilos) et d'une liste de produits, chacun avec un coût et un bénéfice
 * attendu. Quels produits commander pour maximiser le bénéfice total sans
 * dépasser le budget ?
 * Solution : c'est le problème du sac à dos 0/1. La programmation dynamique
 * remplit un tableau meilleur[i][b] = meilleur bénéfice avec les i premiers
 * produits et un budget b. On remonte ensuite le tableau pour savoir quels
 * produits ont été choisis. On compare avec l'approche gloutonne (meilleur
 * ratio d'abord), qui peut se tromper.
 * On apprend : la programmation dynamique, la reconstruction d'une solution,
 * la discrétisation (budget en milliers de FCFA) et les limites du glouton.
 */
import java.util.*;

public class Main {
    record Produit(String nom, int cout, int benefice) {}   // en milliers de FCFA

    static List<Produit> optimal(List<Produit> produits, int budget) {
        int n = produits.size();
        int[][] meilleur = new int[n + 1][budget + 1];
        for (int i = 1; i <= n; i++) {
            Produit p = produits.get(i - 1);
            for (int b = 0; b <= budget; b++) {
                meilleur[i][b] = meilleur[i - 1][b];
                if (p.cout() <= b) meilleur[i][b] = Math.max(meilleur[i][b], meilleur[i - 1][b - p.cout()] + p.benefice());
            }
        }
        List<Produit> choix = new ArrayList<>();
        for (int i = n, b = budget; i > 0; i--)
            if (meilleur[i][b] != meilleur[i - 1][b]) { Produit p = produits.get(i - 1); choix.add(p); b -= p.cout(); }
        return choix;
    }

    static List<Produit> glouton(List<Produit> produits, int budget) {
        List<Produit> tries = new ArrayList<>(produits);
        tries.sort(Comparator.comparingDouble((Produit p) -> (double) p.benefice() / p.cout()).reversed());
        List<Produit> choix = new ArrayList<>();
        for (Produit p : tries) if (p.cout() <= budget) { choix.add(p); budget -= p.cout(); }
        return choix;
    }

    static void afficher(String titre, List<Produit> choix) {
        int cout = choix.stream().mapToInt(Produit::cout).sum(), gain = choix.stream().mapToInt(Produit::benefice).sum();
        System.out.printf("%s : bénéfice %,d FCFA pour %,d FCFA investis%n", titre, gain * 1000L, cout * 1000L);
        for (Produit p : choix) System.out.printf("   - %-26s coût %4d k  bénéfice %4d k%n", p.nom(), p.cout(), p.benefice());
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        int budget = 750;   // 750 000 FCFA
        if (args.length == 1) {
            try {
                budget = Integer.parseInt(args[0]);
                if (budget < 1 || budget > 100_000) throw new NumberFormatException();
            } catch (NumberFormatException e) {
                System.err.println("Usage : java Main [budget en milliers de FCFA, 1..100000]");
                System.exit(1);
            }
        }
        List<Produit> produits = List.of(
                new Produit("Lot de montres connectées", 420, 210), new Produit("Écouteurs Bluetooth x100", 330, 180),
                new Produit("Lampes solaires x60", 260, 120), new Produit("Ventilateurs rechargeables", 350, 175),
                new Produit("Câbles et chargeurs", 120, 55), new Produit("Coques de téléphone x200", 90, 50),
                new Produit("Mini-projecteurs", 510, 230));
        System.out.printf("Budget : %,d FCFA%n%n", budget * 1000L);
        afficher("Solution optimale (programmation dynamique)", optimal(produits, budget));
        System.out.println();
        afficher("Approche gloutonne (meilleur ratio d'abord)", glouton(produits, budget));
    }
}
