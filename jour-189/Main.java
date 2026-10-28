/*
 * Jour 189 — Validation de formulaire d'inscription
 *
 * Problème : un formulaire d'inscription (application de covoiturage,
 * association, boutique) reçoit des saisies incomplètes, mal formatées ou
 * malveillantes. Valider seulement dans le navigateur ne suffit pas : le
 * serveur doit tout revérifier, et les messages d'erreur doivent aider
 * l'utilisateur à corriger, champ par champ.
 * Solution : un petit moteur de validation : chaque champ a une liste de
 * règles (obligatoire, longueur, format par expression régulière, plage de
 * valeurs, règle croisée comme « confirmation identique ») qui renvoient un
 * message clair. On normalise d'abord les saisies (espaces, majuscules du
 * nom, téléphone). Toutes les erreurs sont collectées en une fois, plutôt
 * que la première seulement. Les données ne sont jamais réaffichées sans
 * échappement.
 * On apprend : les interfaces fonctionnelles et les lambdas, les expressions
 * régulières, la normalisation des entrées et la collecte d'erreurs.
 */
import java.time.LocalDate;
import java.time.Period;
import java.util.*;
import java.util.function.Function;
import java.util.regex.Pattern;

public class Main {
    interface Regle extends Function<Map<String, String>, String> {}   // renvoie null si OK, sinon le message

    static Regle obligatoire(String champ) { return f -> f.getOrDefault(champ, "").isBlank() ? "est obligatoire" : null; }
    static Regle longueur(String champ, int min, int max) {
        return f -> { int l = f.getOrDefault(champ, "").length(); return l < min || l > max ? "doit faire entre " + min + " et " + max + " caractères" : null; };
    }
    static Regle format(String champ, String regex, String message) {
        Pattern p = Pattern.compile(regex);
        return f -> f.getOrDefault(champ, "").isEmpty() || p.matcher(f.get(champ)).matches() ? null : message;
    }

    static final Map<String, List<Regle>> REGLES = new LinkedHashMap<>();
    static {
        REGLES.put("nom", List.of(obligatoire("nom"), longueur("nom", 2, 50), format("nom", "[\\p{L} '-]+", "ne doit contenir que des lettres")));
        REGLES.put("email", List.of(obligatoire("email"), format("email", "[\\w.+-]+@[\\w-]+(\\.[\\w-]+)*\\.[a-z]{2,}", "n'est pas une adresse e-mail valide")));
        REGLES.put("telephone", List.of(obligatoire("telephone"), format("telephone", "\\+22901\\d{8}", "doit être un numéro béninois (01 + 8 chiffres)")));
        REGLES.put("naissance", List.of(obligatoire("naissance"), f -> {
            try {
                int age = Period.between(LocalDate.parse(f.get("naissance")), LocalDate.of(2026, 10, 28)).getYears();
                return age < 16 ? "il faut avoir au moins 16 ans" : age > 100 ? "date improbable" : null;
            } catch (Exception e) { return "doit être au format AAAA-MM-JJ"; }
        }));
        REGLES.put("motDePasse", List.of(obligatoire("motDePasse"), longueur("motDePasse", 10, 128),
                format("motDePasse", "(?=.*\\d)(?=.*[A-Za-z]).*", "doit contenir des lettres et des chiffres")));
        REGLES.put("confirmation", List.of(f -> Objects.equals(f.get("motDePasse"), f.get("confirmation")) ? null : "ne correspond pas au mot de passe"));
    }

    static Map<String, String> normaliser(Map<String, String> brut) {
        Map<String, String> f = new HashMap<>();
        brut.forEach((k, v) -> f.put(k, v == null ? "" : v.strip()));
        f.computeIfPresent("email", (k, v) -> v.toLowerCase(Locale.ROOT));
        f.computeIfPresent("nom", (k, v) -> String.join(" ", Arrays.stream(v.split("\\s+")).filter(s -> !s.isEmpty())
                .map(s -> s.substring(0, 1).toUpperCase(Locale.ROOT) + s.substring(1).toLowerCase(Locale.ROOT)).toList()));
        f.computeIfPresent("telephone", (k, v) -> { String c = v.replaceAll("[^\\d+]", ""); return c.startsWith("01") && c.length() == 10 ? "+229" + c : c; });
        return f;
    }

    static String echapper(String s) { return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\"", "&quot;"); }

    static List<String> valider(Map<String, String> brut) {
        Map<String, String> f = normaliser(brut);
        List<String> erreurs = new ArrayList<>();
        REGLES.forEach((champ, regles) -> {
            for (Regle r : regles) { String m = r.apply(f); if (m != null) { erreurs.add(champ + " " + m); break; } }
        });
        if (erreurs.isEmpty()) System.out.println("   normalisé : " + f.get("nom") + " | " + f.get("email") + " | " + f.get("telephone"));
        return erreurs;
    }

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        List<Map<String, String>> formulaires = List.of(
                Map.of("nom", "  akpanon   ange-miguel ", "email", "Ange.Miguel@Exemple.BJ", "telephone", "01 66 54 61 44",
                        "naissance", "2005-03-14", "motDePasse", "Calavi2026xyz", "confirmation", "Calavi2026xyz"),
                Map.of("nom", "<script>alert(1)</script>", "email", "pas-un-email", "telephone", "97 00 00 00",
                        "naissance", "2015-01-01", "motDePasse", "court", "confirmation", "autre"),
                Map.of("nom", "Awa", "email", "", "telephone", "+229 01 97 12 34 56", "naissance", "14/03/2005",
                        "motDePasse", "seulementdeslettres", "confirmation", "seulementdeslettres"));
        for (Map<String, String> f : formulaires) {
            System.out.println("Inscription de « " + echapper(f.get("nom")) + " » :");
            List<String> e = valider(f);
            if (e.isEmpty()) System.out.println("   ✔ valide, compte créé");
            else e.forEach(m -> System.out.println("   ✘ " + m));
            System.out.println();
        }
    }
}
