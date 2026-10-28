/*
 * Jour 120 — Irrigation intelligente selon l'humidité du sol simulée
 *
 * Problème : arroser un potager ou une petite ferme à heure fixe gaspille
 * l'eau quand il a plu et laisse les plantes souffrir en saison sèche. Un
 * capteur d'humidité du sol (capacitif) permet d'arroser seulement quand il
 * le faut, idéalement le matin tôt ou le soir pour limiter l'évaporation.
 * Solution : on simule 14 jours : l'humidité baisse avec l'évaporation (plus
 * forte en milieu de journée et par forte chaleur), monte avec la pluie et
 * l'arrosage. La règle d'arrosage : humidité < seuil de la culture, créneau
 * autorisé (5 h–8 h ou 17 h–19 h), et pas d'arrosage si la pluie prévue est
 * importante. On compare la consommation d'eau avec un arrosage fixe
 * quotidien, et le nombre d'heures où les plantes sont en stress hydrique.
 * On apprend : la modélisation d'un système agricole simple, les règles de
 * décision combinées, les records et la comparaison de stratégies.
 */
import java.util.Locale;
import java.util.Random;

public class Main {
    record Culture(String nom, double seuilBas, double cible) {}
    record Resultat(double litres, int heuresStress, int arrosages) {}

    static Resultat simuler(Culture c, boolean intelligent, long graine) {
        Random r = new Random(graine);
        double humidite = 45, litres = 0;
        int stress = 0, arrosages = 0;
        boolean[] pluie = new boolean[14];
        for (int j = 0; j < 14; j++) pluie[j] = r.nextDouble() < 0.3;
        for (int h = 0; h < 14 * 24; h++) {
            int jour = h / 24, heure = h % 24;
            double chaleur = heure >= 10 && heure <= 16 ? 1.0 : heure >= 7 && heure <= 19 ? 0.5 : 0.15;
            humidite -= 1.5 * chaleur;
            if (pluie[jour] && heure == 15) humidite += 25;                       // averse de l'après-midi
            boolean creneau = (heure >= 5 && heure < 8) || (heure >= 17 && heure < 19);
            boolean pluiePrevue = pluie[jour] && heure < 15;
            boolean arroser = intelligent
                    ? creneau && humidite < c.seuilBas() && !pluiePrevue
                    : heure == 12;                                                // arrosage fixe à midi
            if (arroser) {
                double apport = intelligent ? c.cible() - humidite : 20;
                double efficacite = heure >= 10 && heure <= 16 ? 0.6 : 0.95;      // évaporation à midi
                humidite += apport * efficacite;
                litres += apport * 2.5;
                arrosages++;
            }
            humidite = Math.max(5, Math.min(90, humidite));
            if (humidite < c.seuilBas() - 10) stress++;
        }
        return new Resultat(litres, stress, arrosages);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        long graine = 120;
        if (args.length == 1) {
            try { graine = Long.parseLong(args[0]); }
            catch (NumberFormatException e) { System.err.println("Usage : java Main [graine météo]"); System.exit(1); }
        } else if (args.length > 1) { System.err.println("Usage : java Main [graine]"); System.exit(1); }
        Culture[] cultures = {new Culture("Tomates", 40, 60), new Culture("Piment", 35, 55), new Culture("Laitue", 50, 70)};
        System.out.println("14 jours simulés (pluies aléatoires), parcelle de 10 m²\n");
        System.out.printf("%-9s %-13s %10s %12s %10s%n", "Culture", "Stratégie", "Eau (L)", "Stress (h)", "Arrosages");
        for (Culture c : cultures) {
            for (boolean intelligent : new boolean[]{false, true}) {
                Resultat res = simuler(c, intelligent, graine);
                System.out.printf("%-9s %-13s %10.0f %12d %10d%n", c.nom(), intelligent ? "capteur" : "fixe à midi",
                        res.litres(), res.heuresStress(), res.arrosages());
            }
        }
        System.out.println("\nLe capteur arrose au bon moment, évite les jours de pluie et réduit le stress des plantes.");
    }
}
