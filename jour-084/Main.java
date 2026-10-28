/*
 * Jour 084 — Encodage / décodage Base64
 *
 * Problème : on ne peut pas mettre directement des octets binaires (image,
 * clé, trame de capteur) dans un e-mail, une URL ou un message JSON : ces
 * canaux n'acceptent que du texte. Base64 transforme n'importe quels octets
 * en caractères sûrs (A-Z, a-z, 0-9, + et /).
 * Solution : on regroupe les octets par 3 (24 bits) et on découpe ces 24 bits
 * en 4 groupes de 6 bits, chacun devenant un caractère de l'alphabet de 64
 * symboles. Le signe « = » complète le dernier bloc. On écrit l'encodeur et
 * le décodeur à la main avec des décalages de bits, et on vérifie contre
 * java.util.Base64.
 * On apprend : les opérations sur les bits (<<, >>, &, |), les octets signés
 * en Java, le remplissage (padding) et la vérification par la bibliothèque.
 */
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Base64;

public class Main {
    static final String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static String encoder(byte[] donnees) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < donnees.length; i += 3) {
            int b0 = donnees[i] & 0xFF;
            int b1 = i + 1 < donnees.length ? donnees[i + 1] & 0xFF : 0;
            int b2 = i + 2 < donnees.length ? donnees[i + 2] & 0xFF : 0;
            int bloc = (b0 << 16) | (b1 << 8) | b2;            // 24 bits
            sb.append(ALPHABET.charAt((bloc >> 18) & 63)).append(ALPHABET.charAt((bloc >> 12) & 63));
            sb.append(i + 1 < donnees.length ? ALPHABET.charAt((bloc >> 6) & 63) : '=');
            sb.append(i + 2 < donnees.length ? ALPHABET.charAt(bloc & 63) : '=');
        }
        return sb.toString();
    }

    static byte[] decoder(String texte) {
        if (texte.length() % 4 != 0) throw new IllegalArgumentException("longueur non multiple de 4");
        int remplissage = texte.endsWith("==") ? 2 : texte.endsWith("=") ? 1 : 0;
        byte[] sortie = new byte[texte.length() / 4 * 3 - remplissage];
        int n = 0;
        for (int i = 0; i < texte.length(); i += 4) {
            int bloc = 0;
            for (int k = 0; k < 4; k++) {
                char c = texte.charAt(i + k);
                int v = c == '=' ? 0 : ALPHABET.indexOf(c);
                if (v < 0) throw new IllegalArgumentException("caractère invalide : " + c);
                bloc = (bloc << 6) | v;
            }
            for (int k = 2; k >= 0 && n < sortie.length; k--) sortie[n++] = (byte) (bloc >> (8 * k));
        }
        return sortie;
    }

    static void tester(byte[] donnees, String description) {
        String moi = encoder(donnees), java = Base64.getEncoder().encodeToString(donnees);
        if (!moi.equals(java) || !Arrays.equals(decoder(moi), donnees)) throw new IllegalStateException("erreur pour " + description);
        System.out.printf("%-34s → %s%n", description, moi);
    }

    public static void main(String[] args) {
        if (args.length == 2 && args[0].equals("-d")) {
            try {
                System.out.println(new String(decoder(args[1]), StandardCharsets.UTF_8));
            } catch (IllegalArgumentException e) {
                System.err.println("Base64 invalide : " + e.getMessage());
                System.exit(1);
            }
            return;
        }
        if (args.length == 1) { tester(args[0].getBytes(StandardCharsets.UTF_8), "\"" + args[0] + "\""); return; }
        if (args.length > 2) { System.err.println("Usage : java Main [texte] | -d <base64>"); System.exit(1); }
        tester("Man".getBytes(StandardCharsets.US_ASCII), "\"Man\" (3 octets, sans =)");
        tester("Ma".getBytes(StandardCharsets.US_ASCII), "\"Ma\" (2 octets, un =)");
        tester("M".getBytes(StandardCharsets.US_ASCII), "\"M\" (1 octet, deux =)");
        tester("Bénin 2026 ✓".getBytes(StandardCharsets.UTF_8), "\"Bénin 2026 ✓\" (UTF-8)");
        tester(new byte[]{(byte) 0xA5, 0x01, 0x2C, (byte) 0xFF, 0x00, 0x7E}, "trame capteur binaire A5 01 2C FF 00 7E");
        System.out.println("Décodage de \"SGVsbG8gQ2FsYXZpICE=\" → " + new String(decoder("SGVsbG8gQ2FsYXZpICE="), StandardCharsets.UTF_8));
        System.out.println("Taille : Base64 ajoute environ 33 % (4 caractères pour 3 octets).");
    }
}
