/*
 * Jour 006 — Vérificateur d'année bissextile et nombre de jours du mois
 *
 * Problème : un échéancier, un calendrier de paiement ou un horodatage de
 * capteur doit savoir si février a 28 ou 29 jours.
 * Solution : une année est bissextile si elle est divisible par 4, sauf les
 * siècles, qui doivent être divisibles par 400 (2000 oui, 1900 non). On en
 * déduit le nombre de jours de chaque mois et de l'année.
 * On apprend : les opérateurs logiques, le modulo, le switch moderne de Java et
 * la comparaison avec la bibliothèque java.time pour vérifier notre résultat.
 */
import java.time.Year;
import java.time.YearMonth;

public class Main {
    static final String[] MOIS = {"janvier", "février", "mars", "avril", "mai", "juin",
            "juillet", "août", "septembre", "octobre", "novembre", "décembre"};

    static boolean bissextile(int annee) {
        return (annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0;
    }

    static int joursDansMois(int mois, int annee) {
        return switch (mois) {
            case 2 -> bissextile(annee) ? 29 : 28;
            case 4, 6, 9, 11 -> 30;
            default -> 31;
        };
    }

    static void analyser(int annee) {
        boolean b = bissextile(annee);
        System.out.printf("%d : %s (%d jours)%n", annee, b ? "bissextile" : "non bissextile", b ? 366 : 365);
        // Vérification croisée avec la bibliothèque standard
        if (b != Year.isLeap(annee)) throw new IllegalStateException("incohérence pour " + annee);
        for (int m = 1; m <= 12; m++) {
            int j = joursDansMois(m, annee);
            if (j != YearMonth.of(annee, m).lengthOfMonth()) throw new IllegalStateException("erreur mois " + m);
        }
    }

    public static void main(String[] args) {
        java.util.Locale.setDefault(java.util.Locale.FRANCE);
        if (args.length == 0) {
            for (int a : new int[]{1900, 2000, 2024, 2026, 2028}) analyser(a);
            System.out.println("\nJours par mois en 2028 :");
            for (int m = 1; m <= 12; m++)
                System.out.printf("  %-10s %d%n", MOIS[m - 1], joursDansMois(m, 2028));
            return;
        }
        try {
            int annee = Integer.parseInt(args[0]);
            if (annee < 1583) throw new NumberFormatException("calendrier grégorien : année ≥ 1583");
            analyser(annee);
        } catch (NumberFormatException e) {
            System.err.println("Année invalide : " + args[0] + ". Usage : java Main <année>");
            System.exit(1);
        }
    }
}
