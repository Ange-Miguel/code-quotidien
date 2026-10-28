/*
 * Jour 024 — Formateur de montants avec séparateurs de milliers
 *
 * Problème : « 12500000 » est difficile à lire sur une facture ou une affiche ;
 * « 12 500 000 FCFA » ou « 12,5 M FCFA » l'est beaucoup plus.
 * Solution : on écrit nous-mêmes l'insertion d'un espace tous les trois
 * chiffres (en partant de la droite), on gère les nombres négatifs, puis on
 * propose un format court (k, M, Md) pour les tableaux de bord. On compare avec
 * NumberFormat de la bibliothèque standard.
 * On apprend : la manipulation de chaînes avec StringBuilder, le parcours à
 * l'envers, les formats compacts et la localisation avec NumberFormat.
 */
import java.text.NumberFormat;
import java.util.Locale;

public class Main {
    static String milliers(long montant) {
        String chiffres = Long.toString(Math.abs(montant));
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < chiffres.length(); i++) {
            if (i > 0 && (chiffres.length() - i) % 3 == 0) sb.append(' ');
            sb.append(chiffres.charAt(i));
        }
        return (montant < 0 ? "-" : "") + sb;
    }

    static String court(long montant) {
        double a = Math.abs((double) montant);
        String signe = montant < 0 ? "-" : "";
        if (a >= 1e9) return signe + String.format("%.1f Md", a / 1e9);
        if (a >= 1e6) return signe + String.format("%.1f M", a / 1e6);
        if (a >= 1e3) return signe + String.format("%.1f k", a / 1e3);
        return Long.toString(montant);
    }

    static void afficher(long montant) {
        String standard = NumberFormat.getIntegerInstance(Locale.FRANCE).format(montant);
        System.out.printf("%15d → %-18s | court : %-10s | NumberFormat : %s%n",
                montant, milliers(montant) + " FCFA", court(montant), standard);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length == 0) {
            for (long m : new long[]{0, 950, 12_500, 250_000, 12_500_000, -3_750_000, 2_150_000_000L})
                afficher(m);
            return;
        }
        for (String a : args) {
            try {
                afficher(Long.parseLong(a.replace(" ", "")));
            } catch (NumberFormatException e) {
                System.err.println("Montant invalide : " + a);
                System.exit(1);
            }
        }
    }
}
