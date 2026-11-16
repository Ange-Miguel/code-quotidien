/*
 * Jour 021 — Simulateur de remboursement de prêt (mensualités)
 *
 * Problème : avant d'emprunter pour une moto, du matériel ou un stock de
 * marchandises, il faut connaître la mensualité et le coût total du crédit.
 * Solution : formule de l'annuité constante : M = C × t / (1 − (1 + t)^−n), avec
 * t le taux mensuel et n le nombre de mois. On construit ensuite le tableau
 * d'amortissement : à chaque mois, intérêts = capital restant × t, et le reste
 * de la mensualité rembourse le capital.
 * On apprend : Math.pow, les tableaux d'amortissement, l'arrondi monétaire et
 * l'affichage en colonnes alignées.
 */
import java.util.Locale;

public class Main {
    static double mensualite(double capital, double tauxAnnuel, int mois) {
        double t = tauxAnnuel / 12;
        if (t == 0) return capital / mois;
        return capital * t / (1 - Math.pow(1 + t, -mois));
    }

    static void simuler(double capital, double tauxAnnuel, int mois, boolean detail) {
        if (capital <= 0 || tauxAnnuel < 0 || tauxAnnuel > 1 || mois < 1 || mois > 360)
            throw new IllegalArgumentException("paramètres hors limites");
        double m = mensualite(capital, tauxAnnuel, mois);
        System.out.printf("Prêt de %,.0f FCFA à %.2f %% sur %d mois%n", capital, tauxAnnuel * 100, mois);
        System.out.printf("  Mensualité : %,.0f FCFA%n", m);
        System.out.printf("  Coût total du crédit : %,.0f FCFA%n", m * mois - capital);
        if (!detail) return;
        System.out.printf("%n  %4s %12s %12s %12s %14s%n", "Mois", "Mensualité", "Intérêts", "Capital", "Reste dû");
        double reste = capital;
        for (int i = 1; i <= mois; i++) {
            double interets = reste * tauxAnnuel / 12;
            double amorti = m - interets;
            reste = Math.max(0, reste - amorti);
            System.out.printf("  %4d %,12.0f %,12.0f %,12.0f %,14.0f%n", i, m, interets, amorti, reste);
        }
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        try {
            if (args.length == 0) {
                simuler(1_200_000, 0.12, 12, true);
                System.out.println();
                simuler(5_000_000, 0.09, 36, false);
            } else if (args.length == 3) {
                simuler(Double.parseDouble(args[0]), Double.parseDouble(args[1]) / 100, Integer.parseInt(args[2]), true);
            } else {
                throw new IllegalArgumentException("trois arguments attendus");
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.err.println("Usage : java Main <capital> <taux annuel %> <durée en mois>");
            System.exit(1);
        }
    }
}
