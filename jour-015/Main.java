/*
 * Jour 015 — Convertisseur décimal / binaire / hexadécimal
 *
 * Problème : en électronique et en programmation de microcontrôleurs, on lit
 * des registres en hexadécimal, on règle des bits en binaire et on raisonne en
 * décimal. Il faut passer de l'un à l'autre sans erreur.
 * Solution : on accepte un nombre écrit en décimal (42), binaire (0b101010) ou
 * hexadécimal (0x2A). On le convertit nous-mêmes par divisions successives,
 * puis on vérifie le résultat avec les méthodes de la bibliothèque Java.
 * On apprend : les bases de numération, l'algorithme des divisions
 * successives, StringBuilder et l'affichage des bits par groupes de 4.
 */
import java.util.Locale;

public class Main {
    static final String CHIFFRES = "0123456789ABCDEF";

    static String versBase(long n, int base) {
        if (n == 0) return "0";
        StringBuilder sb = new StringBuilder();
        while (n > 0) {
            sb.append(CHIFFRES.charAt((int) (n % base)));
            n /= base;
        }
        return sb.reverse().toString();
    }

    static long lire(String texte) {
        String t = texte.trim().toLowerCase(Locale.ROOT).replace("_", "");
        if (t.startsWith("0x")) return Long.parseLong(t.substring(2), 16);
        if (t.startsWith("0b")) return Long.parseLong(t.substring(2), 2);
        return Long.parseLong(t);
    }

    static String groupes(String binaire) {
        StringBuilder sb = new StringBuilder();
        int decalage = (4 - binaire.length() % 4) % 4;
        String complet = "0".repeat(decalage) + binaire;
        for (int i = 0; i < complet.length(); i += 4) {
            if (i > 0) sb.append(' ');
            sb.append(complet, i, i + 4);
        }
        return sb.toString();
    }

    static void afficher(String entree) {
        long n = lire(entree);
        if (n < 0) throw new NumberFormatException("nombre négatif non géré");
        String bin = versBase(n, 2), hex = versBase(n, 16), oct = versBase(n, 8);
        if (!bin.equals(Long.toBinaryString(n)) || !hex.equalsIgnoreCase(Long.toHexString(n)))
            throw new IllegalStateException("erreur de conversion");
        System.out.printf("%-12s → déc %d | hex 0x%s | oct %s | bin %s%n", entree, n, hex, oct, groupes(bin));
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        String[] entrees = args.length > 0 ? args : new String[]{"42", "0xFF", "0b1010_0101", "1023", "0x3F8"};
        for (String e : entrees) {
            try {
                afficher(e);
            } catch (NumberFormatException ex) {
                System.err.println("Nombre invalide : " + e + " (exemples : 42, 0x2A, 0b101010)");
                System.exit(1);
            }
        }
    }
}
