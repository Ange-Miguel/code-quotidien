/*
 * Jour 150 — Bilan de la phase 3 : passerelle IoT simulée complète
 *
 * Problème : réunir en un seul programme les briques des jours 101 à 149 :
 * des capteurs envoient des trames binaires avec contrôle d'intégrité, une
 * passerelle les vérifie, filtre les mesures, détecte les anomalies, publie
 * sur des sujets de type MQTT, applique des règles d'automatisme et journalise
 * le tout. C'est l'architecture d'une vraie installation IoT.
 * Solution : plusieurs classes qui coopèrent : Capteur (produit des trames
 * signées avec CRC et compteur anti-rejeu), Passerelle (décode, vérifie,
 * filtre par médiane, calcule un score z), Bus (publier / s'abonner avec
 * jokers), Regles (actions domotiques) et Journal. Une simulation de 2 heures
 * avec un capteur défaillant, des trames corrompues et un rejeu montre que
 * chaque protection joue son rôle.
 * On apprend : l'architecture d'un système IoT de bout en bout, la composition
 * de classes, et comment les mécanismes vus séparément se complètent.
 */
import java.nio.ByteBuffer;
import java.util.*;
import java.util.function.BiConsumer;
import java.util.zip.CRC32;

public class Main {
    // ---- Capteur : trame = id(1) compteur(4) type(1) valeur×100(2) CRC32(4)
    static class Capteur {
        final int id; final String type; int compteur = 0; final Random r;
        Capteur(int id, String type, long graine) { this.id = id; this.type = type; this.r = new Random(graine); }
        byte[] trame(double valeur) {
            ByteBuffer b = ByteBuffer.allocate(12);
            b.put((byte) id).putInt(++compteur).put((byte) (type.equals("temperature") ? 1 : 2)).putShort((short) Math.round(valeur * 100));
            CRC32 crc = new CRC32(); crc.update(b.array(), 0, 8);
            return b.putInt((int) crc.getValue()).array();
        }
    }

    // ---- Bus publier / s'abonner avec jokers (jour 134)
    static class Bus {
        final Map<String, List<BiConsumer<String, Double>>> abonnes = new LinkedHashMap<>();
        void abonner(String filtre, BiConsumer<String, Double> f) { abonnes.computeIfAbsent(filtre, k -> new ArrayList<>()).add(f); }
        void publier(String sujet, double v) { abonnes.forEach((f, l) -> { if (correspond(f, sujet)) l.forEach(c -> c.accept(sujet, v)); }); }
        static boolean correspond(String filtre, String sujet) {
            String[] f = filtre.split("/"), s = sujet.split("/");
            for (int i = 0; i < f.length; i++) {
                if (f[i].equals("#")) return true;
                if (i >= s.length || (!f[i].equals("+") && !f[i].equals(s[i]))) return false;
            }
            return f.length == s.length;
        }
    }

    // ---- Passerelle : vérifications, filtrage, anomalies
    static class Passerelle {
        final Bus bus; final List<String> journal;
        final Map<Integer, Integer> derniersCompteurs = new HashMap<>();
        final Map<Integer, Deque<Double>> fenetres = new HashMap<>();
        final Map<Integer, double[]> stats = new HashMap<>();   // n, moyenne, m2 (Welford)
        int rejets = 0;
        Passerelle(Bus bus, List<String> journal) { this.bus = bus; this.journal = journal; }

        void recevoir(int minute, byte[] t) {
            ByteBuffer b = ByteBuffer.wrap(t);
            CRC32 crc = new CRC32(); crc.update(t, 0, 8);
            if ((int) crc.getValue() != b.getInt(8)) { rejets++; journal.add(String.format("%3d min  ✘ trame corrompue (CRC)", minute)); return; }
            int id = b.get(0), compteur = b.getInt(1);
            if (compteur <= derniersCompteurs.getOrDefault(id, 0)) { rejets++; journal.add(String.format("%3d min  ✘ capteur %d : rejeu refusé", minute, id)); return; }
            derniersCompteurs.put(id, compteur);
            double brut = b.getShort(6) / 100.0;
            Deque<Double> f = fenetres.computeIfAbsent(id, k -> new ArrayDeque<>());
            f.addLast(brut); if (f.size() > 5) f.removeFirst();
            double filtre = f.stream().sorted().toList().get(f.size() / 2);        // médiane (jour 105)
            double[] s = stats.computeIfAbsent(id, k -> new double[3]);
            double ecart = s[0] > 1 ? Math.sqrt(s[2] / (s[0] - 1)) : 0, z = s[0] > 20 && ecart > 0 ? (brut - s[1]) / ecart : 0;
            if (Math.abs(z) > 6) journal.add(String.format("%3d min  ⚠ capteur %d : valeur aberrante %.1f (z=%.0f), filtrée", minute, id, brut, z));
            else { s[0]++; double d = brut - s[1]; s[1] += d / s[0]; s[2] += d * (brut - s[1]); }
            bus.publier("serre/zone" + id + "/" + (b.get(5) == 1 ? "temperature" : "humidite"), filtre);
        }
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        List<String> journal = new ArrayList<>();
        Bus bus = new Bus();
        Passerelle p = new Passerelle(bus, journal);
        boolean[] ventilation = {false};
        double[] derniere = new double[4];
        int[] publications = {0};
        bus.abonner("#", (s, v) -> publications[0]++);
        bus.abonner("serre/+/temperature", (s, v) -> {                       // règle avec hystérésis (jour 107)
            int zone = s.charAt(10) - '0';
            derniere[zone] = v;
            if (!ventilation[0] && v > 33) { ventilation[0] = true; journal.add(String.format("        ▶ %s = %.1f °C : ventilation ON", s, v)); }
            if (ventilation[0] && Arrays.stream(derniere).max().orElse(0) < 31) { ventilation[0] = false; journal.add("        ▶ toutes zones < 31 °C : ventilation OFF"); }
        });
        Capteur[] capteurs = {new Capteur(1, "temperature", 1), new Capteur(2, "temperature", 2), new Capteur(3, "humidite", 3)};
        Random bruit = new Random(150);
        byte[] memorisee = null;
        for (int minute = 0; minute < 120; minute++) {
            for (Capteur c : capteurs) {
                double base = c.type.equals("humidite") ? 70 : 28 + 6 * Math.sin(minute / 25.0) + (c.id == 2 ? 1 : 0);
                double v = base + c.r.nextGaussian() * 0.3;
                if (c.id == 1 && minute == 44) v = 85;                         // capteur défaillant un instant
                byte[] t = c.trame(v);
                if (minute == 10 && c.id == 2) memorisee = t.clone();          // un attaquant enregistre une trame
                if (bruit.nextDouble() < 0.01) t[6] ^= 0x10;                   // interférence radio
                p.recevoir(minute, t);
            }
            if (minute == 70 && memorisee != null) p.recevoir(minute, memorisee);   // … et la rejoue
        }
        journal.forEach(System.out::println);
        System.out.printf("%nBilan sur 2 h : %d trames reçues, %d rejetées, %d publications, ventilation %s%n",
                120 * 3 + 1, p.rejets, publications[0], ventilation[0] ? "en marche" : "arrêtée");
    }
}
