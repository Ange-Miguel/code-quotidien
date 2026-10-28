/*
 * Jour 147 — Authentification HMAC simple d'un message capteur
 *
 * Problème : chiffrer cache le contenu, mais n'empêche pas un attaquant de
 * modifier une trame ou de rejouer une ancienne commande « ouvrir le
 * portail » enregistrée la veille. Il faut prouver que le message vient bien
 * de l'appareil légitime, qu'il n'a pas été modifié et qu'il est récent.
 * Solution : un code d'authentification HMAC-SHA256 calculé avec une clé
 * secrète partagée sur (identifiant, compteur, horodatage, contenu). Le
 * récepteur recalcule le HMAC et le compare en temps constant ; il refuse un
 * compteur déjà vu (rejeu) ou un horodatage trop ancien. On tronque le HMAC à
 * 8 octets pour économiser la bande passante radio, un compromis courant.
 * On apprend : javax.crypto.Mac et HMAC, la comparaison en temps constant
 * (MessageDigest.isEqual), la protection contre le rejeu et la troncature.
 */
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.*;

public class Main {
    static final byte[] CLE = "cle-secrete-partagee-32-octets!!".getBytes(StandardCharsets.UTF_8);
    static final int TAILLE_TAG = 8;

    record Message(String appareil, long compteur, long horodatage, String contenu, byte[] tag) {
        byte[] donnees() {
            return (appareil + "|" + compteur + "|" + horodatage + "|" + contenu).getBytes(StandardCharsets.UTF_8);
        }
    }

    static byte[] hmac(byte[] donnees) throws Exception {
        Mac mac = Mac.getInstance("HmacSHA256");
        mac.init(new SecretKeySpec(CLE, "HmacSHA256"));
        return Arrays.copyOf(mac.doFinal(donnees), TAILLE_TAG);
    }

    static Message signer(String appareil, long compteur, long horodatage, String contenu) throws Exception {
        Message m = new Message(appareil, compteur, horodatage, contenu, null);
        return new Message(appareil, compteur, horodatage, contenu, hmac(m.donnees()));
    }

    static final Map<String, Long> dernierCompteur = new HashMap<>();

    static String verifier(Message m, long maintenant) throws Exception {
        if (!MessageDigest.isEqual(hmac(m.donnees()), m.tag())) return "REFUSÉ : signature invalide (message modifié ou faux)";
        if (Math.abs(maintenant - m.horodatage()) > 30) return "REFUSÉ : message trop ancien";
        if (m.compteur() <= dernierCompteur.getOrDefault(m.appareil(), -1L)) return "REFUSÉ : rejeu détecté (compteur déjà utilisé)";
        dernierCompteur.put(m.appareil(), m.compteur());
        return "ACCEPTÉ : " + m.contenu();
    }

    static String hexa(byte[] b) { StringBuilder sb = new StringBuilder(); for (byte x : b) sb.append(String.format("%02x", x)); return sb.toString(); }

    public static void main(String[] args) throws Exception {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        long t = 1_793_174_400L;
        Message m1 = signer("telecommande-01", 41, t, "OUVRIR_PORTAIL");
        System.out.println("Message signé : " + new String(m1.donnees(), StandardCharsets.UTF_8) + " | tag " + hexa(m1.tag()) + "\n");
        Map<String, Message> essais = new LinkedHashMap<>();
        essais.put("Message légitime", m1);
        essais.put("Rejeu du même message 10 s plus tard", m1);
        essais.put("Contenu modifié par un attaquant", new Message("telecommande-01", 42, t, "OUVRIR_GARAGE", m1.tag()));
        essais.put("Faux message sans la clé", new Message("telecommande-01", 43, t, "OUVRIR_PORTAIL", new byte[TAILLE_TAG]));
        essais.put("Message enregistré hier puis renvoyé", signer("telecommande-01", 44, t - 86_400, "OUVRIR_PORTAIL"));
        essais.put("Nouveau message légitime", signer("telecommande-01", 45, t + 12, "FERMER_PORTAIL"));
        long horloge = t;
        for (var e : essais.entrySet()) {
            System.out.printf("%-38s → %s%n", e.getKey(), verifier(e.getValue(), horloge));
            horloge += 5;
        }
        System.out.printf("%nTag tronqué à %d octets : un faussaire a 1 chance sur 2^%d de deviner au hasard.%n", TAILLE_TAG, TAILLE_TAG * 8);
        if (ByteBuffer.wrap(m1.tag()).getLong() == 0) throw new IllegalStateException();
    }
}
