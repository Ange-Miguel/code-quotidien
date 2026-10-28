/*
 * Jour 114 — Encodage et décodage d'une trame série personnalisée
 *
 * Problème : une carte capteur envoie ses mesures à une passerelle par liaison
 * série (UART) ou radio. Les octets arrivent en flux continu, parfois coupés
 * ou avec du bruit : comment savoir où commence une trame, combien elle
 * contient d'octets et si elle est intacte ?
 * Solution : on définit un format : [0xAA][0x55][longueur][type][données…]
 * [somme XOR]. Le décodeur est une machine à états qui lit octet par octet :
 * recherche de l'en-tête, lecture de la longueur, des données, vérification
 * du contrôle. Il se resynchronise tout seul après une erreur. Les mesures
 * (température, humidité, batterie) sont encodées en entiers signés de 16
 * bits, gros-boutistes, avec ByteBuffer.
 * On apprend : la conception d'un protocole, ByteBuffer et l'ordre des
 * octets, le décodage en flux par machine à états et la resynchronisation.
 */
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.util.*;

public class Main {
    static final int ENTETE1 = 0xAA, ENTETE2 = 0x55, TYPE_MESURE = 0x01;

    static byte[] encoder(int capteur, double temperature, double humidite, int batterieMv) {
        ByteBuffer donnees = ByteBuffer.allocate(7);
        donnees.put((byte) capteur).putShort((short) Math.round(temperature * 100))
               .putShort((short) Math.round(humidite * 100)).putShort((short) batterieMv);
        byte[] d = donnees.array();
        ByteArrayOutputStream t = new ByteArrayOutputStream();
        t.write(ENTETE1); t.write(ENTETE2); t.write(d.length + 1); t.write(TYPE_MESURE);
        t.writeBytes(d);
        int xor = TYPE_MESURE;
        for (byte b : d) xor ^= b;
        t.write(xor & 0xFF);
        return t.toByteArray();
    }

    static class Decodeur {
        enum Etat { ENTETE1, ENTETE2, LONGUEUR, CHARGE, CONTROLE }
        Etat etat = Etat.ENTETE1;
        int longueur, xor, erreurs;
        ByteArrayOutputStream charge = new ByteArrayOutputStream();
        final List<String> recues = new ArrayList<>();

        void octet(int b) {
            switch (etat) {
                case ENTETE1 -> { if (b == ENTETE1) etat = Etat.ENTETE2; }
                case ENTETE2 -> etat = b == ENTETE2 ? Etat.LONGUEUR : (b == ENTETE1 ? Etat.ENTETE2 : Etat.ENTETE1);
                case LONGUEUR -> {
                    if (b < 1 || b > 32) { erreurs++; etat = Etat.ENTETE1; return; }
                    longueur = b; xor = 0; charge.reset(); etat = Etat.CHARGE;
                }
                case CHARGE -> { charge.write(b); xor ^= b; if (charge.size() == longueur) etat = Etat.CONTROLE; }
                case CONTROLE -> {
                    if ((xor & 0xFF) == b) interpreter(charge.toByteArray()); else erreurs++;
                    etat = Etat.ENTETE1;
                }
            }
        }

        void interpreter(byte[] p) {
            ByteBuffer bb = ByteBuffer.wrap(p);
            if (bb.get() != TYPE_MESURE || p.length != 8) { erreurs++; return; }
            recues.add(String.format(Locale.FRANCE, "capteur %d : %.2f °C, %.2f %% HR, batterie %d mV",
                    bb.get(), bb.getShort() / 100.0, bb.getShort() / 100.0, bb.getShort()));
        }
    }

    static String hexa(byte[] t) {
        StringBuilder sb = new StringBuilder();
        for (byte b : t) sb.append(String.format("%02X ", b));
        return sb.toString().trim();
    }

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        byte[] t1 = encoder(3, 28.47, 71.3, 3712), t2 = encoder(7, -4.25, 40.0, 3305), t3 = encoder(3, 29.02, 70.8, 3708);
        System.out.println("Trame 1 : " + hexa(t1));
        ByteArrayOutputStream flux = new ByteArrayOutputStream();
        flux.writeBytes(new byte[]{0x13, (byte) 0xAA, 0x00});        // bruit au démarrage
        flux.writeBytes(t1);
        byte[] abimee = t2.clone();
        abimee[6] ^= 0x04;                                           // un bit inversé en route
        flux.writeBytes(abimee);
        flux.writeBytes(Arrays.copyOf(t3, 5));                       // trame coupée
        flux.writeBytes(t3);
        flux.writeBytes(t2);
        System.out.println("Flux reçu : " + flux.size() + " octets (bruit, trame abîmée, trame coupée)");
        Decodeur d = new Decodeur();
        for (byte b : flux.toByteArray()) d.octet(b & 0xFF);
        d.recues.forEach(r -> System.out.println("  ✔ " + r));
        System.out.println("  ✘ trames rejetées : " + d.erreurs);
    }
}
