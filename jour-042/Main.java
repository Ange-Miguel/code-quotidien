/*
 * Jour 042 — Calculatrice en ligne de commande (opérations et priorités)
 *
 * Problème : évaluer une expression comme « 2 + 3 * (4 - 1) / 2 » en respectant
 * la priorité des opérateurs et les parenthèses, comme le fait une vraie
 * calculatrice.
 * Solution : un analyseur à descente récursive. La grammaire a trois niveaux :
 * expression = terme (+|- terme)* ; terme = facteur (*|/|% facteur)* ;
 * facteur = nombre | -facteur | ( expression ). Chaque niveau est une méthode,
 * ce qui encode naturellement la priorité.
 * On apprend : l'analyse syntaxique, la récursivité mutuelle, la gestion
 * d'erreurs précises (position de l'erreur) et la division par zéro.
 */
import java.util.Locale;

public class Main {
    private final String texte;
    private int pos = 0;

    Main(String texte) { this.texte = texte.replace(" ", ""); }

    static double evaluer(String expression) {
        Main p = new Main(expression);
        double v = p.expression();
        if (p.pos != p.texte.length()) throw p.erreur("caractère inattendu");
        return v;
    }

    private IllegalArgumentException erreur(String message) {
        return new IllegalArgumentException(message + " à la position " + (pos + 1) + " : " + texte);
    }

    private char courant() { return pos < texte.length() ? texte.charAt(pos) : '\0'; }

    private double expression() {
        double v = terme();
        while (courant() == '+' || courant() == '-') {
            char op = texte.charAt(pos++);
            v = op == '+' ? v + terme() : v - terme();
        }
        return v;
    }

    private double terme() {
        double v = facteur();
        while (courant() == '*' || courant() == '/' || courant() == '%') {
            char op = texte.charAt(pos++);
            double d = facteur();
            if (op != '*' && d == 0) throw erreur("division par zéro");
            v = op == '*' ? v * d : op == '/' ? v / d : v % d;
        }
        return v;
    }

    private double facteur() {
        if (courant() == '-') { pos++; return -facteur(); }
        if (courant() == '(') {
            pos++;
            double v = expression();
            if (courant() != ')') throw erreur("parenthèse fermante manquante");
            pos++;
            return v;
        }
        int debut = pos;
        while (Character.isDigit(courant()) || courant() == '.') pos++;
        if (debut == pos) throw erreur("nombre attendu");
        return Double.parseDouble(texte.substring(debut, pos));
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        String[] expressions = args.length > 0 ? new String[]{String.join(" ", args)}
                : new String[]{"2 + 3 * 4", "(2 + 3) * 4", "10 / 4 - 1.5", "-(3 - 8) * 2", "17 % 5",
                               "2 * (3 + (4 - 1)) / 3", "5 / (2 - 2)", "3 + * 2"};
        boolean erreur = false;
        for (String e : expressions) {
            try {
                System.out.printf("%-24s = %s%n", e, new java.text.DecimalFormat("#.######").format(evaluer(e)));
            } catch (IllegalArgumentException ex) {
                System.out.printf("%-24s → erreur : %s%n", e, ex.getMessage());
                erreur = true;
            }
        }
        if (erreur && args.length > 0) System.exit(1);
    }
}
