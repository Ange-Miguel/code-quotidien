/*
 * Jour 081 — Distance de Levenshtein : correcteur orthographique simple
 *
 * Problème : un client tape « telefone », « ordinatuer » ou « calvi » dans la
 * recherche d'une boutique en ligne et ne trouve rien. Il faut lui proposer le
 * mot le plus proche (« Vouliez-vous dire… ? »).
 * Solution : la distance de Levenshtein compte le nombre minimal d'insertions,
 * suppressions et substitutions pour passer d'un mot à l'autre. On la calcule
 * par programmation dynamique avec seulement deux lignes de tableau (mémoire
 * O(n)). On compare ensuite le mot saisi à un dictionnaire et on propose les
 * mots à distance ≤ 2, du plus proche au plus éloigné.
 * On apprend : la distance d'édition, l'optimisation mémoire d'un tableau de
 * programmation dynamique et le classement de suggestions.
 */
import java.text.Normalizer;
import java.util.*;

public class Main {
    static int distance(String a, String b) {
        int[] precedente = new int[b.length() + 1], courante = new int[b.length() + 1];
        for (int j = 0; j <= b.length(); j++) precedente[j] = j;
        for (int i = 1; i <= a.length(); i++) {
            courante[0] = i;
            for (int j = 1; j <= b.length(); j++) {
                int cout = a.charAt(i - 1) == b.charAt(j - 1) ? 0 : 1;
                courante[j] = Math.min(Math.min(courante[j - 1] + 1, precedente[j] + 1), precedente[j - 1] + cout);
            }
            int[] t = precedente; precedente = courante; courante = t;
        }
        return precedente[b.length()];
    }

    static String normaliser(String s) {
        return Normalizer.normalize(s.toLowerCase(Locale.ROOT), Normalizer.Form.NFD).replaceAll("\\p{M}", "");
    }

    static final List<String> DICTIONNAIRE = List.of("téléphone", "ordinateur", "chargeur", "écouteurs", "batterie",
            "câble", "montre", "lampe", "ventilateur", "imprimante", "clavier", "souris", "écran", "calavi",
            "cotonou", "parakou", "porto-novo", "godomey", "livraison", "commande");

    static List<String> suggestions(String saisie) {
        String s = normaliser(saisie);
        Map<String, Integer> proches = new HashMap<>();
        for (String mot : DICTIONNAIRE) {
            int d = distance(s, normaliser(mot));
            if (d <= Math.max(1, Math.min(2, s.length() / 3))) proches.put(mot, d);
        }
        List<String> liste = new ArrayList<>(proches.keySet());
        liste.sort(Comparator.comparing((String m) -> proches.get(m)).thenComparing(Comparator.naturalOrder()));
        return liste;
    }

    public static void main(String[] args) {
        if (distance("kitten", "sitting") != 3 || distance("", "abc") != 3) throw new IllegalStateException("test");
        String[] saisies = args.length > 0 ? args : new String[]{"telefone", "ordinatuer", "calvi", "chargeurs",
                "ecran", "porto novo", "xylophone"};
        for (String s : saisies) {
            List<String> sug = suggestions(s);
            if (sug.isEmpty()) System.out.printf("%-12s → aucune suggestion%n", s);
            else if (normaliser(sug.get(0)).equals(normaliser(s))) System.out.printf("%-12s → correct (%s)%n", s, sug.get(0));
            else System.out.printf("%-12s → vouliez-vous dire « %s » ? ?%s%n", s, sug.get(0),
                        sug.size() > 1 ? " (autres : " + String.join(", ", sug.subList(1, sug.size())) + ")" : "");
        }
    }
}
