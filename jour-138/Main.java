/*
 * Jour 138 — Socket UDP : découverte d'appareils sur le réseau local
 *
 * Problème : quand on branche une prise connectée ou une caméra IP, comment
 * l'application du téléphone la trouve-t-elle sans connaître son adresse IP ?
 * Par la découverte : l'application envoie un message « qui est là ? » et
 * chaque appareil répond avec son nom, son type et ses capacités.
 * Solution : UDP, sans connexion, est adapté à ces petits messages. Sur un
 * vrai réseau, la question part en diffusion (broadcast 255.255.255.255) ;
 * pour que la démo marche sur n'importe quelle machine, les appareils
 * simulés écoutent chacun sur un port local et la question leur est envoyée
 * un par un. On gère les délais d'attente (un appareil éteint ne répond pas),
 * les réponses dans le désordre et les messages invalides.
 * On apprend : DatagramSocket et DatagramPacket, la différence entre UDP et
 * TCP, les délais d'attente (setSoTimeout) et un protocole texte simple.
 */
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Main {
    record Appareil(String nom, String type, String capacites, boolean allume) {}

    static DatagramSocket lancerAppareil(Appareil a) throws SocketException {
        DatagramSocket s = new DatagramSocket(0, InetAddress.getLoopbackAddress());
        Thread t = new Thread(() -> {
            byte[] buf = new byte[512];
            while (!s.isClosed()) {
                try {
                    DatagramPacket p = new DatagramPacket(buf, buf.length);
                    s.receive(p);
                    String q = new String(p.getData(), 0, p.getLength(), StandardCharsets.UTF_8);
                    if (!a.allume() || !q.equals("DECOUVERTE?v1")) continue;      // ignore les autres messages
                    Thread.sleep(new Random().nextInt(40));                      // réponses dans le désordre
                    byte[] r = ("ICI|" + a.nom() + "|" + a.type() + "|" + a.capacites()).getBytes(StandardCharsets.UTF_8);
                    s.send(new DatagramPacket(r, r.length, p.getSocketAddress()));
                } catch (Exception e) { return; }
            }
        });
        t.setDaemon(true);
        t.start();
        return s;
    }

    public static void main(String[] args) throws Exception {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        List<Appareil> reseau = List.of(new Appareil("prise-salon", "prise", "on,off,conso", true),
                new Appareil("camera-portail", "camera", "flux,photo", true),
                new Appareil("capteur-cuve", "capteur", "niveau,batterie", false),
                new Appareil("thermostat", "climat", "consigne,mesure", true));
        List<DatagramSocket> sockets = new ArrayList<>();
        for (Appareil a : reseau) sockets.add(lancerAppareil(a));

        try (DatagramSocket client = new DatagramSocket(0, InetAddress.getLoopbackAddress())) {
            client.setSoTimeout(300);                                            // 300 ms d'attente maximum
            byte[] question = "DECOUVERTE?v1".getBytes(StandardCharsets.UTF_8);
            for (DatagramSocket s : sockets)                                     // (vrai réseau : un seul broadcast)
                client.send(new DatagramPacket(question, question.length, s.getLocalSocketAddress()));
            byte[] faux = "BONJOUR".getBytes(StandardCharsets.UTF_8);
            client.send(new DatagramPacket(faux, faux.length, sockets.get(0).getLocalSocketAddress()));
            System.out.println("Question « DECOUVERTE?v1 » envoyée, attente des réponses…");
            List<String> trouves = new ArrayList<>();
            long debut = System.nanoTime();
            byte[] buf = new byte[512];
            while (true) {
                try {
                    DatagramPacket p = new DatagramPacket(buf, buf.length);
                    client.receive(p);
                    String[] champs = new String(p.getData(), 0, p.getLength(), StandardCharsets.UTF_8).split("\\|");
                    if (champs.length != 4 || !champs[0].equals("ICI")) continue;
                    trouves.add(champs[1]);
                    System.out.printf("  %4d ms  %-15s type %-8s capacités [%s]  (port %d)%n",
                            (System.nanoTime() - debut) / 1_000_000, champs[1], champs[2], champs[3], p.getPort());
                } catch (SocketTimeoutException e) { break; }
            }
            System.out.printf("%d appareil(s) trouvé(s) sur %d ; sans réponse : ", trouves.size(), reseau.size());
            System.out.println(reseau.stream().map(Appareil::nom).filter(n -> !trouves.contains(n)).toList());
        } finally {
            sockets.forEach(DatagramSocket::close);
        }
    }
}
