/*
 * Jour 099 — Mini-tableur : évaluer des formules simples en cellules
 *
 * Problème : comment un tableur comme Excel recalcule-t-il automatiquement les
 * cellules (=A1*B1, =SOMME(C1:C3)) quand une valeur change, et comment
 * détecte-t-il une référence circulaire ?
 * Solution : chaque cellule contient un nombre ou une formule. L'évaluation
 * est récursive : pour calculer =A1*B1 on évalue A1 et B1 d'abord. On garde
 * en cache les valeurs calculées et un ensemble des cellules « en cours de
 * calcul » : retomber sur l'une d'elles signale une référence circulaire.
 * Formules gérées : + − * /, parenthèses, références (A1) et SOMME/MOYENNE sur
 * une plage (A1:A3).
 * On apprend : l'évaluation récursive avec mémoïsation, la détection de cycle,
 * les expressions régulières et un analyseur d'expressions (repris du jour 042).
 */
import java.util.*;
import java.util.regex.*;

public class Main {
    final Map<String, String> cellules = new TreeMap<>();
    final Map<String, Double> cache = new HashMap<>();
    final Set<String> enCours = new HashSet<>();

    void mettre(String ref, String contenu) { cellules.put(ref, contenu); cache.clear(); }

    double valeur(String ref) {
        if (cache.containsKey(ref)) return cache.get(ref);
        if (!enCours.add(ref)) throw new IllegalStateException("référence circulaire sur " + ref);
        try {
            String c = cellules.getOrDefault(ref, "0").trim();
            double v = c.startsWith("=") ? evaluer(remplacerFonctions(c.substring(1))) : Double.parseDouble(c);
            cache.put(ref, v);
            return v;
        } finally {
            enCours.remove(ref);
        }
    }

    static final Pattern FONCTION = Pattern.compile("(SOMME|MOYENNE)\\(([A-Z])(\\d+):([A-Z])(\\d+)\\)");
    static final Pattern REF = Pattern.compile("[A-Z]\\d+");

    String remplacerFonctions(String f) {
        Matcher m = FONCTION.matcher(f);
        StringBuilder sb = new StringBuilder();
        while (m.find()) {
            double somme = 0;
            int n = 0;
            for (char col = m.group(2).charAt(0); col <= m.group(4).charAt(0); col++)
                for (int l = Integer.parseInt(m.group(3)); l <= Integer.parseInt(m.group(5)); l++) { somme += valeur("" + col + l); n++; }
            m.appendReplacement(sb, String.valueOf(m.group(1).equals("SOMME") ? somme : somme / n));
        }
        m.appendTail(sb);
        Matcher r = REF.matcher(sb.toString());
        StringBuilder sb2 = new StringBuilder();
        while (r.find()) r.appendReplacement(sb2, "(" + valeur(r.group()) + ")");
        r.appendTail(sb2);
        return sb2.toString();
    }

    // Analyseur d'expressions à descente récursive (voir jour 042)
    private String txt; private int pos;
    double evaluer(String e) { txt = e.replace(" ", ""); pos = 0; double v = expr(); if (pos != txt.length()) throw new IllegalArgumentException("formule invalide"); return v; }
    private char cour() { return pos < txt.length() ? txt.charAt(pos) : '\0'; }
    private double expr() { double v = terme(); while (cour() == '+' || cour() == '-') v = txt.charAt(pos++) == '+' ? v + terme() : v - terme(); return v; }
    private double terme() { double v = facteur(); while (cour() == '*' || cour() == '/') v = txt.charAt(pos++) == '*' ? v * facteur() : v / facteur(); return v; }
    private double facteur() {
        if (cour() == '-') { pos++; return -facteur(); }
        if (cour() == '(') { pos++; double v = expr(); pos++; return v; }
        int d = pos; while (Character.isDigit(cour()) || cour() == '.' || cour() == 'E') pos++;
        if (d == pos) throw new IllegalArgumentException("formule invalide");
        return Double.parseDouble(txt.substring(d, pos));
    }

    void afficher() {
        for (String ref : cellules.keySet()) {
            String v;
            try { v = String.format(Locale.FRANCE, "%,.2f", valeur(ref)); } catch (RuntimeException e) { v = "#ERREUR : " + e.getMessage(); }
            System.out.printf("  %-4s %-26s → %s%n", ref, cellules.get(ref), v);
        }
    }

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        Main t = new Main();
        t.mettre("A1", "4500"); t.mettre("B1", "3");    t.mettre("C1", "=A1*B1");
        t.mettre("A2", "2500"); t.mettre("B2", "10");   t.mettre("C2", "=A2*B2");
        t.mettre("A3", "12000"); t.mettre("B3", "1");   t.mettre("C3", "=A3*B3");
        t.mettre("C4", "=SOMME(C1:C3)"); t.mettre("C5", "=C4*0.18"); t.mettre("C6", "=C4+C5");
        t.mettre("D1", "=MOYENNE(A1:A3)");
        System.out.println("Feuille de vente :");
        t.afficher();
        System.out.println("\nOn passe la quantité B2 de 10 à 20 : recalcul automatique");
        t.mettre("B2", "20");
        System.out.printf("  C6 (total TTC) = %,.2f%n", t.valeur("C6"));
        System.out.println("\nAjout d'une référence circulaire E1 = E2 + 1, E2 = E1 * 2 :");
        t.mettre("E1", "=E2+1"); t.mettre("E2", "=E1*2");
        t.afficher();
    }
}
