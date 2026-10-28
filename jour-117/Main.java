/*
 * Jour 117 — Géorepérage : un point est-il dans une zone ?
 *
 * Problème : un traceur GPS sur une moto ou un colis doit déclencher une
 * alerte quand il sort d'une zone autorisée (quartier, ville, entrepôt), ou
 * signaler une arrivée à destination. C'est le géorepérage (geofencing).
 * Solution : deux types de zones. Zone circulaire : on compare la distance au
 * centre (Haversine, jour 116) au rayon. Zone polygonale : algorithme du
 * lancer de rayon : on trace une demi-droite depuis le point et on compte les
 * côtés du polygone traversés ; impair = dedans. On suit ensuite un trajet GPS
 * simulé et on détecte les entrées et sorties de zone (changements d'état).
 * On apprend : l'algorithme du point dans un polygone, les interfaces Java
 * (Zone), la détection d'événements par comparaison avec l'état précédent.
 */
import java.util.*;

public class Main {
    interface Zone { String nom(); boolean contient(double lat, double lon); }

    record Cercle(String nom, double lat, double lon, double rayonM) implements Zone {
        public boolean contient(double la, double lo) { return distance(lat, lon, la, lo) <= rayonM; }
    }

    record Polygone(String nom, double[][] sommets) implements Zone {
        public boolean contient(double la, double lo) {
            boolean dedans = false;
            for (int i = 0, j = sommets.length - 1; i < sommets.length; j = i++) {
                double yi = sommets[i][0], xi = sommets[i][1], yj = sommets[j][0], xj = sommets[j][1];
                if ((yi > la) != (yj > la) && lo < (xj - xi) * (la - yi) / (yj - yi) + xi) dedans = !dedans;
            }
            return dedans;
        }
    }

    static double distance(double lat1, double lon1, double lat2, double lon2) {
        double p1 = Math.toRadians(lat1), p2 = Math.toRadians(lat2);
        double h = Math.pow(Math.sin((p2 - p1) / 2), 2)
                + Math.cos(p1) * Math.cos(p2) * Math.pow(Math.sin(Math.toRadians(lon2 - lon1) / 2), 2);
        return 2 * 6_371_000 * Math.asin(Math.sqrt(h));
    }

    public static void main(String[] args) {
        // Zones approximatives, pour l'exemple
        List<Zone> zones = List.of(
                new Cercle("Entrepôt", 6.4440, 2.3470, 300),
                new Polygone("Campus (zone autorisée)", new double[][]{
                        {6.4140, 2.3370}, {6.4230, 2.3360}, {6.4250, 2.3470}, {6.4170, 2.3500}, {6.4120, 2.3440}}));
        if (args.length == 2) {
            try {
                double la = Double.parseDouble(args[0]), lo = Double.parseDouble(args[1]);
                for (Zone z : zones) System.out.printf("%-26s : %s%n", z.nom(), z.contient(la, lo) ? "DEDANS" : "dehors");
                return;
            } catch (NumberFormatException e) { System.err.println("Usage : java Main [latitude longitude]"); System.exit(1); }
        }
        if (args.length != 0) { System.err.println("Usage : java Main [latitude longitude]"); System.exit(1); }
        double[][] trajet = {{6.4445, 2.3468}, {6.4410, 2.3450}, {6.4330, 2.3430}, {6.4240, 2.3415}, {6.4200, 2.3420},
                {6.4160, 2.3440}, {6.4100, 2.3460}, {6.4080, 2.3500}, {6.4150, 2.3460}, {6.4300, 2.3440}, {6.4438, 2.3472}};
        Map<Zone, Boolean> etat = new HashMap<>();
        for (int i = 0; i < trajet.length; i++) {
            double la = trajet[i][0], lo = trajet[i][1];
            StringBuilder evenements = new StringBuilder();
            for (Zone z : zones) {
                boolean dedans = z.contient(la, lo);
                Boolean avant = etat.put(z, dedans);
                if (avant != null && avant != dedans) evenements.append(dedans ? " ▶ ENTRÉE " : " ◀ SORTIE ").append(z.nom()).append(';');
            }
            System.out.printf(Locale.ROOT, "08:%02d  %.4f, %.4f%s%n", i * 3, la, lo, evenements);
        }
    }
}
