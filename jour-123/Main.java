/*
 * Jour 123 — Estimation d'autonomie d'une batterie selon la consommation
 *
 * Problème : un capteur IoT sur batterie (station météo, traceur, compteur
 * d'eau) doit tenir des mois sans intervention. Combien de temps dure une
 * batterie Li-ion de 3 000 mAh si l'appareil dort la plupart du temps et se
 * réveille pour mesurer et envoyer ?
 * Solution : on décrit le cycle de fonctionnement en phases (sommeil profond,
 * mesure, émission Wi-Fi ou LoRa) avec leur courant et leur durée. Le courant
 * moyen est la moyenne pondérée par le temps ; l'autonomie = capacité utile
 * (avec marge pour l'autodécharge et le vieillissement) / courant moyen. On
 * compare plusieurs intervalles d'envoi et deux technologies radio, et on
 * cherche l'intervalle minimal pour tenir un an.
 * On apprend : le bilan énergétique d'un objet connecté, la moyenne pondérée,
 * l'impact énorme du mode sommeil, et la recherche d'un paramètre par essais.
 */
import java.util.List;
import java.util.Locale;

public class Main {
    record Phase(String nom, double courantMa, double dureeS) {}

    static final double CAPACITE_MAH = 3000, MARGE = 0.8;   // 20 % de marge (autodécharge, vieillissement, froid)

    static double courantMoyen(List<Phase> actives, double sommeilMa, double intervalleS) {
        double charge = 0, dureeActive = 0;
        for (Phase p : actives) { charge += p.courantMa() * p.dureeS(); dureeActive += p.dureeS(); }
        if (dureeActive > intervalleS) throw new IllegalArgumentException("intervalle trop court");
        charge += sommeilMa * (intervalleS - dureeActive);
        return charge / intervalleS;
    }

    static double autonomieJours(double courantMa) { return CAPACITE_MAH * MARGE / courantMa / 24; }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        List<Phase> wifi = List.of(new Phase("réveil + mesure", 25, 0.3), new Phase("connexion Wi-Fi", 120, 2.5),
                new Phase("envoi HTTP", 170, 0.8));
        List<Phase> lora = List.of(new Phase("réveil + mesure", 25, 0.3), new Phase("émission LoRa", 45, 0.4));
        double sommeil = 0.015;   // 15 µA en sommeil profond
        if (args.length == 1) {
            try {
                double intervalle = Double.parseDouble(args[0]) * 60;
                double i = courantMoyen(wifi, sommeil, intervalle);
                System.out.printf("Wi-Fi toutes les %s min : %.3f mA en moyenne → %.0f jours%n", args[0], i, autonomieJours(i));
                return;
            } catch (IllegalArgumentException e) { System.err.println("Usage : java Main [intervalle d'envoi en minutes]"); System.exit(1); }
        }
        if (args.length > 1) { System.err.println("Usage : java Main [intervalle en minutes]"); System.exit(1); }
        System.out.printf("Batterie %,.0f mAh (utile %,.0f mAh), sommeil profond %.0f µA%n%n", CAPACITE_MAH, CAPACITE_MAH * MARGE, sommeil * 1000);
        System.out.printf("%-12s %14s %14s %14s %14s%n", "Intervalle", "Wi-Fi (mA)", "Wi-Fi", "LoRa (mA)", "LoRa");
        for (int minutes : new int[]{1, 5, 15, 60, 240}) {
            double iw = courantMoyen(wifi, sommeil, minutes * 60.0), il = courantMoyen(lora, sommeil, minutes * 60.0);
            System.out.printf("%-12s %14.3f %11.0f j %14.4f %11.0f j%n", minutes + " min", iw, autonomieJours(iw), il, autonomieJours(il));
        }
        int min = 1;
        while (autonomieJours(courantMoyen(wifi, sommeil, min * 60.0)) < 365) min++;
        System.out.printf("%nPour tenir 1 an en Wi-Fi, il faut envoyer au plus toutes les %d minutes.%n", min);
        double sansSommeil = courantMoyen(wifi, 25, 15 * 60.0);
        System.out.printf("Sans mode sommeil (25 mA en attente) : %.1f jours seulement !%n", autonomieJours(sansSommeil));
    }
}
