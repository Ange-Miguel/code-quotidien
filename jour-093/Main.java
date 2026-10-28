/*
 * Jour 093 — Calendrier mensuel affiché en console
 *
 * Problème : afficher le calendrier d'un mois (comme la commande cal sous
 * Linux), avec les semaines commençant le lundi, en français, et en
 * signalant les jours fériés.
 * Solution : on détermine le jour de la semaine du 1er du mois. Pour savoir
 * le calculer sans bibliothèque, on implémente la congruence de Zeller, que
 * l'on vérifie avec java.time. On aligne ensuite les jours dans une grille de
 * 7 colonnes et on marque d'un astérisque les jours fériés fixes du Bénin.
 * On apprend : la congruence de Zeller, YearMonth, l'alignement en grille et
 * la vérification d'un calcul maison par la bibliothèque standard.
 */
import java.time.*;
import java.time.format.TextStyle;
import java.util.*;

public class Main {
    // Jours fériés à date fixe au Bénin (les fêtes religieuses mobiles ne sont pas incluses)
    static final Map<MonthDay, String> FERIES = Map.of(
            MonthDay.of(1, 1), "Jour de l'An", MonthDay.of(1, 10), "Fête du Vodoun",
            MonthDay.of(5, 1), "Fête du Travail", MonthDay.of(8, 1), "Fête de l'Indépendance",
            MonthDay.of(8, 15), "Assomption", MonthDay.of(11, 1), "Toussaint", MonthDay.of(12, 25), "Noël");

    /** Congruence de Zeller : 0 = lundi … 6 = dimanche. */
    static int jourSemaine(int annee, int mois, int jour) {
        if (mois < 3) { mois += 12; annee--; }
        int k = annee % 100, j = annee / 100;
        int h = (jour + 13 * (mois + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;   // 0 = samedi
        return (h + 5) % 7;
    }

    static void afficher(int annee, int mois) {
        YearMonth ym = YearMonth.of(annee, mois);
        int premier = jourSemaine(annee, mois, 1);
        if (premier != ym.atDay(1).getDayOfWeek().getValue() - 1) throw new IllegalStateException("Zeller faux");
        String titre = ym.getMonth().getDisplayName(TextStyle.FULL, Locale.FRANCE) + " " + annee;
        System.out.printf("%n%s%s%n", " ".repeat((28 - titre.length()) / 2), titre);
        System.out.println(" Lu  Ma  Me  Je  Ve  Sa  Di");
        StringBuilder ligne = new StringBuilder("    ".repeat(premier));
        List<String> notes = new ArrayList<>();
        for (int d = 1; d <= ym.lengthOfMonth(); d++) {
            String ferie = FERIES.get(MonthDay.of(mois, d));
            ligne.append(String.format("%3d%s", d, ferie != null ? "*" : " "));
            if (ferie != null) notes.add(d + " : " + ferie);
            if ((premier + d) % 7 == 0) { System.out.println(ligne.toString().stripTrailing()); ligne.setLength(0); }
        }
        if (!ligne.isEmpty()) System.out.println(ligne.toString().stripTrailing());
        for (String n : notes) System.out.println("  * " + n);
    }

    public static void main(String[] args) {
        try {
            if (args.length == 2) afficher(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
            else if (args.length == 0) { afficher(2026, 10); afficher(2026, 11); afficher(2027, 1); }
            else throw new IllegalArgumentException();
        } catch (RuntimeException e) {
            if (e instanceof IllegalStateException) throw e;
            System.err.println("Usage : java Main <année> <mois 1-12>");
            System.exit(1);
        }
    }
}
