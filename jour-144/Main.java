/*
 * Jour 144 — Prédiction simple de consommation (régression linéaire)
 *
 * Problème : un gestionnaire d'immeuble ou une boutique veut prévoir sa
 * consommation d'électricité de la semaine prochaine, qui dépend fortement de
 * la chaleur (climatiseurs, ventilateurs). Peut-on la prévoir à partir de la
 * température prévue ?
 * Solution : la régression linéaire des moindres carrés trouve la droite
 * y = a·x + b qui passe au plus près des points (température, kWh) observés :
 * a = cov(x, y) / var(x), b = moyenne(y) − a·moyenne(x). Le coefficient R²
 * indique la qualité de l'ajustement. On entraîne le modèle sur 30 jours,
 * on le teste sur 7 jours qu'il n'a jamais vus, et on prédit avec les
 * prévisions météo.
 * On apprend : la régression linéaire, R², la séparation entraînement /
 * test, l'erreur moyenne absolue et les limites d'un modèle simple.
 */
import java.util.*;

public class Main {
    record Modele(double a, double b) { double predire(double x) { return a * x + b; } }

    static Modele ajuster(double[] x, double[] y) {
        double mx = Arrays.stream(x).average().orElseThrow(), my = Arrays.stream(y).average().orElseThrow();
        double cov = 0, var = 0;
        for (int i = 0; i < x.length; i++) { cov += (x[i] - mx) * (y[i] - my); var += (x[i] - mx) * (x[i] - mx); }
        if (var == 0) throw new IllegalArgumentException("toutes les températures sont identiques");
        return new Modele(cov / var, my - cov / var * mx);
    }

    static double r2(Modele m, double[] x, double[] y) {
        double my = Arrays.stream(y).average().orElseThrow(), sr = 0, st = 0;
        for (int i = 0; i < x.length; i++) { sr += Math.pow(y[i] - m.predire(x[i]), 2); st += Math.pow(y[i] - my, 2); }
        return 1 - sr / st;
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        Random r = new Random(144);
        int n = 37;
        double[] temp = new double[n], kwh = new double[n];
        for (int i = 0; i < n; i++) {
            temp[i] = 27 + 5 * r.nextDouble();                                   // 27 à 32 °C
            kwh[i] = 18 + 6.5 * (temp[i] - 27) + r.nextGaussian() * 3;           // relation cachée + bruit
        }
        double[] xa = Arrays.copyOf(temp, 30), ya = Arrays.copyOf(kwh, 30);
        double[] xt = Arrays.copyOfRange(temp, 30, n), yt = Arrays.copyOfRange(kwh, 30, n);
        Modele m = ajuster(xa, ya);
        System.out.printf("Modèle appris sur 30 jours : kWh = %.2f × température %+.1f%n", m.a(), m.b());
        System.out.printf("Chaque degré en plus coûte environ %.1f kWh par jour. R² = %.2f%n%n", m.a(), r2(m, xa, ya));
        double erreur = 0;
        System.out.println("Test sur 7 jours jamais vus :");
        for (int i = 0; i < xt.length; i++) {
            double p = m.predire(xt[i]);
            erreur += Math.abs(p - yt[i]);
            System.out.printf("   %4.1f °C : réel %5.1f kWh, prédit %5.1f kWh (écart %+.1f)%n", xt[i], yt[i], p, p - yt[i]);
        }
        System.out.printf("Erreur moyenne : %.1f kWh/jour%n%n", erreur / xt.length);
        double[] prevision = args.length > 0 ? new double[args.length] : new double[]{29.5, 31.0, 32.5, 30.0, 28.0, 33.5, 34.0};
        try {
            for (int i = 0; i < args.length; i++) prevision[i] = Double.parseDouble(args[i]);
        } catch (NumberFormatException e) { System.err.println("Usage : java Main [températures prévues…]"); System.exit(1); }
        double total = 0;
        System.out.println("Prévisions avec la météo annoncée :");
        for (double t : prevision) {
            double p = m.predire(t);
            total += p;
            System.out.printf("   %4.1f °C → %5.1f kWh%s%n", t, p, t > 32 ? "  (hors des températures apprises : prudence)" : "");
        }
        System.out.printf("Total prévu : %.0f kWh, soit environ %,.0f FCFA à 125 FCFA/kWh%n", total, total * 125);
    }
}
