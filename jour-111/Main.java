/*
 * Jour 111 — Contrôleur PID simulé pour régler une température
 *
 * Problème : maintenir un four, un incubateur à œufs ou une chambre de
 * fermentation à une température précise. Un simple tout-ou-rien (jour 107)
 * oscille ; il faut doser la puissance de chauffe en continu.
 * Solution : le régulateur PID calcule la commande à partir de l'erreur
 * e = consigne − mesure : P (proportionnel à l'erreur), I (somme des erreurs
 * passées, élimine l'écart résiduel), D (vitesse de variation, amortit). On
 * simule un incubateur (résistance chauffante puis air, avec retard et pertes) et on
 * compare P seul, PI et PID : dépassement, temps de stabilisation, erreur
 * finale. L'intégrale est bornée (anti-windup) car la puissance est limitée.
 * On apprend : le principe du PID, la discrétisation (pas de temps dt),
 * la saturation et l'anti-windup, et les indicateurs de performance.
 */
import java.util.Locale;

public class Main {
    static class Pid {
        final double kp, ki, kd;
        double integrale = 0, erreurPrecedente = Double.NaN;
        Pid(double kp, double ki, double kd) { this.kp = kp; this.ki = ki; this.kd = kd; }

        double commande(double consigne, double mesure, double dt) {
            double e = consigne - mesure;
            double derivee = Double.isNaN(erreurPrecedente) ? 0 : (e - erreurPrecedente) / dt;
            erreurPrecedente = e;
            double sortie = kp * e + ki * (integrale + e * dt) + kd * derivee;
            if (sortie > 0 && sortie < 100) integrale += e * dt;     // anti-windup : n'intègre pas en saturation
            return Math.max(0, Math.min(100, sortie));                // puissance de chauffe 0..100 %
        }
    }

    /**
     * Incubateur à deux « réservoirs » de chaleur : la résistance chauffe d'abord,
     * puis transmet sa chaleur à l'air avec un retard ; l'air perd vers l'ambiance (28 °C).
     * Ce retard est ce qui fait osciller une régulation mal réglée.
     */
    static double[] simuler(Pid pid, double consigne, int secondes) {
        double t = 28, resistance = 28, dt = 1, max = t;
        int stabilise = -1;
        for (int s = 0; s < secondes; s++) {
            double p = pid.commande(consigne, t, dt);
            resistance += (0.6 * p / 100 - 0.02 * (resistance - t)) * dt;
            t += (0.01 * (resistance - t) - 0.002 * (t - 28)) * dt;
            max = Math.max(max, t);
            if (Math.abs(t - consigne) > 0.2) stabilise = -1;
            else if (stabilise < 0) stabilise = s;
        }
        return new double[]{max - consigne, stabilise, consigne - t};
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        double consigne = 37.7;   // incubation d'œufs de poule
        if (args.length == 3) {
            try {
                Pid p = new Pid(Double.parseDouble(args[0]), Double.parseDouble(args[1]), Double.parseDouble(args[2]));
                double[] r = simuler(p, consigne, 3600);
                System.out.printf("Kp=%s Ki=%s Kd=%s → dépassement %.2f °C, stable à %.0f s, erreur finale %.3f °C%n",
                        args[0], args[1], args[2], r[0], r[1], r[2]);
                return;
            } catch (NumberFormatException e) { System.err.println("Usage : java Main [Kp Ki Kd]"); System.exit(1); }
        }
        if (args.length != 0) { System.err.println("Usage : java Main [Kp Ki Kd]"); System.exit(1); }
        System.out.printf("Incubateur : consigne %.1f °C, départ 28 °C, 1 heure simulée%n%n", consigne);
        System.out.printf("%-18s %13s %16s %14s%n", "Réglage", "Dépassement", "Stable (±0,2°C)", "Erreur finale");
        Object[][] reglages = {{"P seul (Kp=20)", new Pid(20, 0, 0)}, {"PI", new Pid(20, 0.05, 0)},
                {"PID", new Pid(20, 0.05, 400)}, {"PI trop nerveux", new Pid(60, 0.3, 0)}};
        for (Object[] r : reglages) {
            double[] res = simuler((Pid) r[1], consigne, 3600);
            String stable = res[1] < 0 ? "jamais" : String.format("%.0f s", res[1]);
            System.out.printf("%-18s %10.2f °C %16s %11.3f °C%n", r[0], res[0], stable, Math.abs(res[2]) < 0.0005 ? 0.0 : res[2]);
        }
        System.out.println("\nLe P seul garde un écart permanent ; le I l'annule ; le D réduit le dépassement.");
    }
}
