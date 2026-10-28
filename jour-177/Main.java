/*
 * Jour 177 — Analyse de sentiment simple d'avis clients
 *
 * Problème : une boutique reçoit chaque semaine des dizaines d'avis et de
 * commentaires. Les lire tous prend du temps ; on voudrait savoir rapidement
 * ce qui plaît, ce qui fâche, et repérer les clients mécontents à recontacter.
 * Solution : une analyse à base de lexique : chaque mot porte une polarité
 * (« excellent » +2, « déçu » −2…). On gère les négations (« pas bon »
 * inverse le mot suivant), les intensificateurs (« très », « trop »), puis on
 * normalise le score. On extrait aussi les thèmes cités (livraison, prix,
 * qualité, accueil) pour savoir de quoi parlent les avis positifs et négatifs.
 * C'est simple et explicable ; l'ironie lui échappe, ce qu'on montre aussi.
 * On apprend : le traitement de texte (découpage en mots, normalisation),
 * les Map de lexiques, la gestion du contexte (négation) et l'agrégation.
 */
import java.text.Normalizer;
import java.util.*;

public class Main {
    static final Map<String, Integer> LEXIQUE = Map.ofEntries(
            Map.entry("excellent", 2), Map.entry("parfait", 2), Map.entry("super", 2), Map.entry("bon", 1), Map.entry("bien", 1),
            Map.entry("rapide", 1), Map.entry("satisfait", 2), Map.entry("recommande", 2), Map.entry("merci", 1), Map.entry("gentil", 1),
            Map.entry("mauvais", -2), Map.entry("decu", -2), Map.entry("lent", -1), Map.entry("cher", -1), Map.entry("casse", -2),
            Map.entry("retard", -1), Map.entry("arnaque", -3), Map.entry("nul", -2), Map.entry("impoli", -2), Map.entry("probleme", -1));
    static final Set<String> NEGATIONS = Set.of("pas", "jamais", "plus", "aucun", "rien");
    static final Map<String, Double> INTENSITE = Map.of("tres", 1.5, "trop", 1.5, "vraiment", 1.3, "peu", 0.5);
    static final Map<String, String> THEMES = Map.of("livraison", "livraison", "livre", "livraison", "prix", "prix", "cher", "prix",
            "qualite", "qualité", "casse", "qualité", "vendeur", "accueil", "accueil", "accueil", "impoli", "accueil");

    static String normaliser(String s) {
        return Normalizer.normalize(s.toLowerCase(Locale.ROOT), Normalizer.Form.NFD).replaceAll("\\p{M}", "")
                .replaceAll("[.,;:!?]", " | ").replaceAll("[^a-z'| ]", " ");
    }

    record Analyse(double score, Set<String> themes) {}

    static Analyse analyser(String avis) {
        String[] mots = normaliser(avis).replace("'", " ").split("\\s+");
        double score = 0, multiplicateur = 1;
        int negation = 0, comptes = 0;
        Set<String> themes = new TreeSet<>();
        for (String m : mots) {
            if (m.equals("|")) { negation = 0; multiplicateur = 1; continue; }   // la ponctuation coupe la négation
            if (THEMES.containsKey(m)) themes.add(THEMES.get(m));
            if (NEGATIONS.contains(m)) { negation = 3; continue; }      // la négation porte sur les 3 mots suivants
            if (INTENSITE.containsKey(m)) { multiplicateur = INTENSITE.get(m); continue; }
            Integer p = LEXIQUE.get(m.endsWith("s") && !LEXIQUE.containsKey(m) ? m.substring(0, m.length() - 1) : m);
            if (p != null) { score += p * multiplicateur * (negation > 0 ? -1 : 1); comptes++; multiplicateur = 1; }
            if (negation > 0) negation--;
        }
        return new Analyse(comptes == 0 ? 0 : Math.max(-1, Math.min(1, score / (2 * Math.sqrt(comptes)))), themes);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        List<String> avis = args.length > 0 ? List.of(String.join(" ", args)) : List.of(
                "Livraison très rapide, produit excellent. Je recommande !",
                "Écouteurs cassés au bout d'une semaine, vraiment déçu de la qualité.",
                "Le vendeur était gentil mais le prix est trop cher.",
                "Pas mauvais du tout, bon rapport qualité prix.",
                "Aucun problème, livré à Calavi le jour même. Merci !",
                "Commande jamais livrée, arnaque !",
                "Super, encore un retard de livraison... bravo.");
        Map<String, double[]> parTheme = new TreeMap<>();
        for (String a : avis) {
            Analyse r = analyser(a);
            String etiquette = r.score() > 0.2 ? "positif 🙂" : r.score() < -0.2 ? "NÉGATIF ☹" : "neutre 😐";
            System.out.printf("%+.2f %-10s %s%n", r.score(), etiquette, a);
            for (String t : r.themes()) { double[] v = parTheme.computeIfAbsent(t, k -> new double[2]); v[0] += r.score(); v[1]++; }
        }
        System.out.println("\nSentiment moyen par thème :");
        parTheme.forEach((t, v) -> System.out.printf("   %-10s %+.2f (%d avis)%n", t, v[0] / v[1], (int) v[1]));
        if (args.length == 0) System.out.println("\nLimite : l'ironie du dernier avis (« Super… bravo ») trompe le lexique.");
    }
}
