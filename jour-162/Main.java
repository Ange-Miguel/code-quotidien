/*
 * Jour 162 — Panier e-commerce avec codes promo
 *
 * Problème : une boutique en ligne propose des codes promo variés :
 * pourcentage, montant fixe, livraison offerte, « 3 achetés = le moins cher
 * offert », avec un minimum d'achat, une date limite ou une catégorie. Les
 * règles de cumul et les arrondis sont des sources d'erreurs (et de pertes !).
 * Solution : une interface Promotion avec plusieurs implémentations, chacune
 * vérifiant ses conditions et calculant sa remise. Le panier applique au plus
 * un code (le meilleur pour le client si plusieurs sont fournis), plafonne la
 * remise au sous-total, calcule la livraison selon le montant et affiche un
 * récapitulatif clair. Montants en entiers de FCFA.
 * On apprend : les interfaces et le polymorphisme (types scellés), les règles
 * métier encapsulées, la comparaison de solutions et le calcul monétaire sûr.
 */
import java.time.LocalDate;
import java.util.*;

public class Main {
    record Article(String nom, String categorie, int prix, int quantite) { int total() { return prix * quantite; } }

    sealed interface Promotion permits Pourcentage, MontantFixe, LivraisonOfferte, TroisPourDeux {
        String code();
        LocalDate expiration();
        int minimum();
        int remise(List<Article> panier, int sousTotal, int livraison);
        default String verifier(int sousTotal, LocalDate jour) {
            if (jour.isAfter(expiration())) return "code expiré";
            if (sousTotal < minimum()) return "minimum d'achat " + minimum() + " FCFA";
            return null;
        }
    }
    record Pourcentage(String code, int pct, String categorie, int minimum, LocalDate expiration) implements Promotion {
        public int remise(List<Article> p, int st, int liv) {
            int base = p.stream().filter(a -> categorie == null || a.categorie().equals(categorie)).mapToInt(Article::total).sum();
            return base * pct / 100;
        }
    }
    record MontantFixe(String code, int montant, int minimum, LocalDate expiration) implements Promotion {
        public int remise(List<Article> p, int st, int liv) { return montant; }
    }
    record LivraisonOfferte(String code, int minimum, LocalDate expiration) implements Promotion {
        public int remise(List<Article> p, int st, int liv) { return liv; }
    }
    record TroisPourDeux(String code, int minimum, LocalDate expiration) implements Promotion {
        public int remise(List<Article> p, int st, int liv) {
            List<Integer> prix = new ArrayList<>();
            p.forEach(a -> { for (int i = 0; i < a.quantite(); i++) prix.add(a.prix()); });
            Collections.sort(prix);
            int r = 0;
            for (int i = 0; i < prix.size() / 3; i++) r += prix.get(i);   // le moins cher de chaque groupe de 3
            return r;
        }
    }

    static final Map<String, Promotion> CODES = Map.of(
            "BIENVENUE10", new Pourcentage("BIENVENUE10", 10, null, 10_000, LocalDate.of(2026, 12, 31)),
            "AUDIO20", new Pourcentage("AUDIO20", 20, "audio", 0, LocalDate.of(2026, 11, 30)),
            "MOINS3000", new MontantFixe("MOINS3000", 3000, 25_000, LocalDate.of(2026, 12, 31)),
            "LIVRAISON0", new LivraisonOfferte("LIVRAISON0", 15_000, LocalDate.of(2026, 12, 31)),
            "TROISPOURDEUX", new TroisPourDeux("TROISPOURDEUX", 0, LocalDate.of(2026, 11, 15)),
            "ETE2026", new Pourcentage("ETE2026", 15, null, 0, LocalDate.of(2026, 8, 31)));

    static void commander(List<Article> panier, List<String> codes, LocalDate jour) {
        int sousTotal = panier.stream().mapToInt(Article::total).sum();
        int livraison = sousTotal >= 50_000 ? 0 : 1_500;
        Promotion meilleure = null;
        int remise = 0;
        for (String c : codes) {
            Promotion p = CODES.get(c.toUpperCase(Locale.ROOT));
            String refus = p == null ? "code inconnu" : p.verifier(sousTotal, jour);
            if (refus != null) { System.out.printf("   code %-14s refusé : %s%n", c, refus); continue; }
            int r = Math.min(p.remise(panier, sousTotal, livraison), sousTotal + livraison);
            System.out.printf("   code %-14s valable : -%,d FCFA%n", c, r);
            if (r > remise) { remise = r; meilleure = p; }
        }
        for (Article a : panier) System.out.printf("   %-24s %d × %,7d = %,8d%n", a.nom(), a.quantite(), a.prix(), a.total());
        System.out.printf("   Sous-total %,d | livraison %,d | remise %s -%,d → TOTAL %,d FCFA%n%n", sousTotal, livraison,
                meilleure == null ? "" : "(" + meilleure.code() + ")", remise, sousTotal + livraison - remise);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        LocalDate jour = LocalDate.of(2026, 10, 28);
        List<Article> p1 = List.of(new Article("Écouteurs Bluetooth", "audio", 4_500, 2), new Article("Chargeur rapide", "énergie", 3_500, 1),
                new Article("Câble USB-C", "énergie", 1_500, 3));
        List<Article> p2 = List.of(new Article("Enceinte portable", "audio", 18_000, 1), new Article("Montre connectée", "montre", 25_000, 1),
                new Article("Écouteurs Bluetooth", "audio", 4_500, 1));
        System.out.println("Panier 1 (le client essaie plusieurs codes) :");
        commander(p1, args.length > 0 ? Arrays.asList(args) : List.of("BIENVENUE10", "AUDIO20", "TROISPOURDEUX", "PROMO2027"), jour);
        System.out.println("Panier 2 :");
        commander(p2, List.of("MOINS3000", "LIVRAISON0", "ETE2026", "BIENVENUE10"), jour);
    }
}
