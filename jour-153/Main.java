/*
 * Jour 153 — Calcul de tarif zem selon la distance et l'heure
 *
 * Problème : avec un zémidjan, le prix se négocie à chaque course, et le
 * client comme le conducteur ne savent pas toujours si le prix est juste.
 * Une application de réservation doit afficher un tarif clair et justifiable.
 * Solution : un tarif = prise en charge + prix au kilomètre (dégressif au-delà
 * de 5 km) + majorations (nuit, pluie, forte demande), avec un minimum et un
 * arrondi aux 50 FCFA, car on paie en espèces. On calcule aussi la part du
 * conducteur après commission de la plateforme et son gain par litre
 * d'essence. Tous les montants sont des exemples à ajuster au terrain.
 * On apprend : la conception d'une grille tarifaire, les enums pour les
 * conditions, l'arrondi monétaire et la décomposition d'un prix.
 */
import java.time.LocalTime;
import java.util.*;

public class Main {
    static final int PRISE_EN_CHARGE = 150, PRIX_KM = 100, PRIX_KM_LONG = 75, MINIMUM = 200;
    static final double COMMISSION = 0.10, CONSO_L_100KM = 2.5, PRIX_LITRE = 680;

    enum Condition {
        NUIT("nuit (22 h-5 h)", 0.30), PLUIE("pluie", 0.20), DEMANDE("forte demande", 0.15);
        final String libelle; final double majoration;
        Condition(String l, double m) { libelle = l; majoration = m; }
    }

    static Set<Condition> conditions(LocalTime h, boolean pluie, boolean demande) {
        Set<Condition> s = EnumSet.noneOf(Condition.class);
        if (h.getHour() >= 22 || h.getHour() < 5) s.add(Condition.NUIT);
        if (pluie) s.add(Condition.PLUIE);
        if (demande) s.add(Condition.DEMANDE);
        return s;
    }

    static int tarif(double km, Set<Condition> conds, StringBuilder detail) {
        if (km <= 0 || km > 60) throw new IllegalArgumentException("distance entre 0 et 60 km");
        double base = PRISE_EN_CHARGE + Math.min(km, 5) * PRIX_KM + Math.max(0, km - 5) * PRIX_KM_LONG;
        detail.append(String.format("prise en charge %d + %.1f km → %.0f", PRISE_EN_CHARGE, km, base));
        double majoration = 0;
        for (Condition c : conds) { majoration += c.majoration; detail.append(String.format(" | %s +%.0f%%", c.libelle, c.majoration * 100)); }
        double prix = Math.max(MINIMUM, base * (1 + Math.min(majoration, 0.5)));   // majoration plafonnée à 50 %
        return (int) (Math.ceil(prix / 50) * 50);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length == 1 || args.length == 2) {
            try {
                double km = Double.parseDouble(args[0].replace(',', '.'));
                LocalTime h = args.length == 2 ? LocalTime.parse(args[1]) : LocalTime.now();
                StringBuilder d = new StringBuilder();
                System.out.printf("%,d FCFA (%s)%n", tarif(km, conditions(h, false, false), d), d);
                return;
            } catch (RuntimeException e) {
                System.err.println("Usage : java Main <km> [HH:MM]");
                System.exit(1);
            }
        }
        if (args.length > 2) { System.err.println("Usage : java Main <km> [HH:MM]"); System.exit(1); }
        Object[][] courses = {
                {"Zogbadjè → Campus UAC", 1.2, "07:30", false, true}, {"Godomey → Calavi Kpota", 6.8, "08:10", false, false},
                {"Calavi → Cotonou Ganhi", 17.5, "18:20", true, true}, {"Akpakpa → Cadjèhoun", 7.4, "23:15", false, false},
                {"Calavi → Cotonou (nuit + pluie)", 17.5, "23:40", true, false}};
        for (Object[] c : courses) {
            StringBuilder d = new StringBuilder();
            double km = (double) c[1];
            int prix = tarif(km, conditions(LocalTime.parse((String) c[2]), (boolean) c[3], (boolean) c[4]), d);
            double net = prix * (1 - COMMISSION), essence = km * CONSO_L_100KM / 100 * PRIX_LITRE;
            System.out.printf("%-32s %s  → %,6d FCFA%n", c[0] + " (" + c[2] + ")", "", prix);
            System.out.printf("   %s%n   conducteur : %,.0f FCFA après commission, %,.0f FCFA d'essence → gain %,.0f FCFA%n",
                    d, net, essence, net - essence);
        }
    }
}
