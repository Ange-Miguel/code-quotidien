/*
 * Jour 057 — Pile (stack) : vérifier l'équilibre des parenthèses d'une formule
 *
 * Problème : une formule de tableur, une expression mathématique ou un code
 * source avec une parenthèse, un crochet ou une accolade mal fermé ne
 * fonctionne pas, et l'erreur est difficile à repérer à l'œil.
 * Solution : on parcourt le texte ; chaque symbole ouvrant est empilé avec sa
 * position ; chaque symbole fermant doit correspondre au sommet de la pile.
 * À la fin, la pile doit être vide. On indique précisément la position et le
 * type de l'erreur, en ignorant ce qui est entre guillemets.
 * On apprend : la structure de pile (LIFO) avec ArrayDeque, les records, les
 * correspondances avec une Map et le signalement précis des erreurs.
 */
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Map;

public class Main {
    record Ouvrant(char symbole, int position) {}

    static final Map<Character, Character> PAIRES = Map.of(')', '(', ']', '[', '}', '{');

    static String verifier(String texte) {
        Deque<Ouvrant> pile = new ArrayDeque<>();
        boolean dansChaine = false;
        for (int i = 0; i < texte.length(); i++) {
            char c = texte.charAt(i);
            if (c == '"') dansChaine = !dansChaine;
            if (dansChaine) continue;
            if (c == '(' || c == '[' || c == '{') {
                pile.push(new Ouvrant(c, i));
            } else if (PAIRES.containsKey(c)) {
                if (pile.isEmpty()) return "« " + c + " » en position " + (i + 1) + " ferme quelque chose qui n'est pas ouvert";
                Ouvrant o = pile.pop();
                if (o.symbole() != PAIRES.get(c))
                    return "« " + c + " » en position " + (i + 1) + " ne correspond pas à « " + o.symbole()
                            + " » ouvert en position " + (o.position() + 1);
            }
        }
        if (dansChaine) return "guillemet non fermé";
        if (!pile.isEmpty()) {
            Ouvrant o = pile.peek();
            return "« " + o.symbole() + " » ouvert en position " + (o.position() + 1) + " n'est jamais fermé";
        }
        return null;
    }

    static String souligner(String texte, String erreur) {
        java.util.regex.Matcher m = java.util.regex.Pattern.compile("position (\\d+)").matcher(erreur);
        if (!m.find()) return "";
        return "\n      " + " ".repeat(Integer.parseInt(m.group(1)) - 1) + "^";
    }

    public static void main(String[] args) {
        String[] formules = args.length > 0 ? args : new String[]{
                "=SOMME(A1:A10)*(B2+C2)", "{ a[i] = (b + c) * d; }", "((2 + 3) * [4 - 1]",
                "f(x) = [x + 1)", "=SI(A1>10;\"ok (bon\";\"non\")", "a + b)"};
        boolean toutBon = true;
        for (String f : formules) {
            String erreur = verifier(f);
            System.out.printf("%-30s → %s%n", f, erreur == null ? "équilibrée ✔" : "ERREUR : " + erreur);
            if (erreur != null) {
                toutBon = false;
                String s = souligner(f, erreur);
                if (!s.isEmpty()) System.out.println("      " + f + s);
            }
        }
        if (!toutBon && args.length > 0) System.exit(1);
    }
}
