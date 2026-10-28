/*
 * Jour 132 — Décodage d'une commande JSON reçue d'un serveur
 *
 * Problème : une passerelle domotique reçoit des ordres de l'application
 * mobile sous forme JSON, par exemple {"appareil":"lampe_salon","action":"on",
 * "luminosite":70}. Elle doit les lire de façon sûre : champs manquants,
 * mauvais types, valeurs hors limites ou texte malformé ne doivent jamais la
 * faire planter ni allumer n'importe quoi.
 * Solution : on écrit un petit analyseur JSON (sans bibliothèque externe, ce
 * qui est fréquent sur une petite passerelle) : objets, tableaux, chaînes avec
 * échappements, nombres, booléens et null, avec la position de l'erreur. On
 * valide ensuite la commande avec des règles explicites (liste blanche
 * d'appareils et d'actions, bornes) avant de l'exécuter.
 * On apprend : l'analyse syntaxique récursive d'un format réel, les types
 * dynamiques en Java (Object, Map, List), la validation stricte des entrées
 * et le principe de « liste blanche ».
 */
import java.util.*;

public class Main {
    private final String s; private int i = 0;
    Main(String s) { this.s = s; }

    static Object analyser(String texte) {
        Main p = new Main(texte);
        Object v = p.valeur();
        p.espaces();
        if (p.i != texte.length()) throw p.erreur("caractères en trop");
        return v;
    }

    private IllegalArgumentException erreur(String m) { return new IllegalArgumentException(m + " (position " + i + ")"); }
    private void espaces() { while (i < s.length() && Character.isWhitespace(s.charAt(i))) i++; }
    private char attendre(char c) {
        espaces();
        if (i >= s.length() || s.charAt(i) != c) throw erreur("« " + c + " » attendu");
        return s.charAt(i++);
    }

    private Object valeur() {
        espaces();
        if (i >= s.length()) throw erreur("fin inattendue");
        char c = s.charAt(i);
        if (c == '{') return objet();
        if (c == '[') return tableau();
        if (c == '"') return chaine();
        if (s.startsWith("true", i)) { i += 4; return true; }
        if (s.startsWith("false", i)) { i += 5; return false; }
        if (s.startsWith("null", i)) { i += 4; return null; }
        int d = i;
        while (i < s.length() && "+-0123456789.eE".indexOf(s.charAt(i)) >= 0) i++;
        if (d == i) throw erreur("valeur invalide");
        try { return Double.parseDouble(s.substring(d, i)); } catch (NumberFormatException e) { throw erreur("nombre invalide"); }
    }

    private Map<String, Object> objet() {
        Map<String, Object> m = new LinkedHashMap<>();
        attendre('{'); espaces();
        if (i < s.length() && s.charAt(i) == '}') { i++; return m; }
        do { espaces(); String cle = chaine(); attendre(':'); m.put(cle, valeur()); espaces(); }
        while (i < s.length() && s.charAt(i) == ',' && i++ >= 0);
        attendre('}');
        return m;
    }

    private List<Object> tableau() {
        List<Object> l = new ArrayList<>();
        attendre('['); espaces();
        if (i < s.length() && s.charAt(i) == ']') { i++; return l; }
        do { l.add(valeur()); espaces(); } while (i < s.length() && s.charAt(i) == ',' && i++ >= 0);
        attendre(']');
        return l;
    }

    private String chaine() {
        attendre('"');
        StringBuilder sb = new StringBuilder();
        while (i < s.length() && s.charAt(i) != '"') {
            char c = s.charAt(i++);
            if (c == '\\' && i < s.length()) {
                char e = s.charAt(i++);
                switch (e) {
                    case 'n' -> sb.append('\n'); case 't' -> sb.append('\t');
                    case 'u' -> { sb.append((char) Integer.parseInt(s.substring(i, i + 4), 16)); i += 4; }
                    default -> sb.append(e);
                }
            } else sb.append(c);
        }
        attendre('"');
        return sb.toString();
    }

    static final Set<String> APPAREILS = Set.of("lampe_salon", "lampe_chambre", "ventilateur", "portail");
    static final Set<String> ACTIONS = Set.of("on", "off", "basculer");

    static String executer(String json) {
        Object o;
        try { o = analyser(json); } catch (IllegalArgumentException e) { return "REJET (JSON) : " + e.getMessage(); }
        if (!(o instanceof Map<?, ?> m)) return "REJET : un objet JSON est attendu";
        if (!(m.get("appareil") instanceof String a) || !APPAREILS.contains(a)) return "REJET : appareil inconnu ou absent";
        if (!(m.get("action") instanceof String act) || !ACTIONS.contains(act)) return "REJET : action invalide";
        String detail = "";
        if (m.containsKey("luminosite")) {
            if (!(m.get("luminosite") instanceof Double l) || l < 0 || l > 100 || l != Math.floor(l))
                return "REJET : luminosité entière entre 0 et 100 attendue";
            detail = " à " + l.intValue() + " %";
        }
        return "OK : " + a + " → " + act + detail;
    }

    public static void main(String[] args) {
        String[] commandes = args.length > 0 ? args : new String[]{
                "{\"appareil\":\"lampe_salon\",\"action\":\"on\",\"luminosite\":70}",
                "{ \"appareil\" : \"ventilateur\", \"action\" : \"off\" }",
                "{\"appareil\":\"four\",\"action\":\"on\"}",
                "{\"appareil\":\"lampe_chambre\",\"action\":\"on\",\"luminosite\":250}",
                "{\"appareil\":\"portail\",\"action\":\"ouvrir\"}",
                "{\"appareil\":\"portail\",\"action\":\"on\"",
                "[\"lampe_salon\",\"on\"]",
                "{\"appareil\":\"lampe_salon\",\"action\":\"basculer\",\"note\":\"Caf\\u00e9 \\\"ouvert\\\"\"}"};
        boolean rejet = false;
        for (String c : commandes) {
            String r = executer(c);
            rejet |= r.startsWith("REJET");
            System.out.printf("%s%n   → %s%n", c, r);
        }
        if (rejet && args.length > 0) System.exit(1);
    }
}
