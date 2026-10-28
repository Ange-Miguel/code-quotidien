/*
 * Jour 102 — Masques de bits : lecture d'un registre d'état simulé
 *
 * Problème : un capteur I2C (accéléromètre, chargeur de batterie, module GPS)
 * renvoie souvent un seul octet ou mot de « statut » dans lequel chaque bit
 * ou groupe de bits a une signification : donnée prête, erreur, mode de
 * charge, niveau de batterie… Il faut savoir décoder ce registre.
 * Solution : on décrit chaque champ du registre (nom, position, largeur) dans
 * une table, puis on extrait chaque champ avec (registre >>> position) & masque.
 * On simule le registre d'état 16 bits d'un chargeur de batterie et on décode
 * plusieurs valeurs lues, avec un affichage bit par bit.
 * On apprend : les masques multi-bits, le décalage non signé >>> en Java, les
 * tables de description de registres (comme dans une fiche technique) et
 * l'affichage binaire formaté.
 */
import java.util.List;
import java.util.Map;

public class Main {
    record Champ(String nom, int position, int largeur, Map<Integer, String> valeurs) {
        int extraire(int registre) { return (registre >>> position) & ((1 << largeur) - 1); }
    }

    // Registre d'état fictif d'un chargeur de batterie (inspiré des fiches techniques réelles)
    static final List<Champ> STATUT = List.of(
            new Champ("DONNEE_PRETE", 0, 1, Map.of(0, "non", 1, "oui")),
            new Champ("ENTREE_PRESENTE", 1, 1, Map.of(0, "pas d'alimentation", 1, "alimentation branchée")),
            new Champ("MODE_CHARGE", 2, 2, Map.of(0, "arrêt", 1, "pré-charge", 2, "charge rapide", 3, "terminée")),
            new Champ("SURCHAUFFE", 4, 1, Map.of(0, "non", 1, "OUI")),
            new Champ("ERREUR", 5, 3, Map.of(0, "aucune", 1, "surtension", 2, "batterie absente", 5, "délai dépassé")),
            new Champ("NIVEAU_BATTERIE", 8, 7, Map.of()),     // 0..100 %
            new Champ("ALERTE", 15, 1, Map.of(0, "non", 1, "OUI")));

    static String binaire(int r) {
        String b = String.format("%16s", Integer.toBinaryString(r & 0xFFFF)).replace(' ', '0');
        return b.substring(0, 4) + " " + b.substring(4, 8) + " " + b.substring(8, 12) + " " + b.substring(12);
    }

    static void decoder(int registre) {
        System.out.printf("Registre = 0x%04X = %s%n", registre, binaire(registre));
        for (Champ c : STATUT) {
            int v = c.extraire(registre);
            String sens = c.valeurs().isEmpty() ? v + " %" : c.valeurs().getOrDefault(v, "valeur réservée (" + v + ")");
            System.out.printf("   bits %2d-%-2d %-16s = %-3d → %s%n", c.position(), c.position() + c.largeur() - 1, c.nom(), v, sens);
        }
        System.out.println();
    }

    static int construire(int niveau, int mode, boolean entree) {
        return (niveau & 0x7F) << 8 | (mode & 0b11) << 2 | (entree ? 1 : 0) << 1 | 1;
    }

    public static void main(String[] args) {
        if (args.length == 1) {
            try {
                String a = args[0].toLowerCase().replace("0x", "");
                int r = Integer.parseInt(a, 16);
                if (r < 0 || r > 0xFFFF) throw new NumberFormatException();
                decoder(r);
            } catch (NumberFormatException e) {
                System.err.println("Usage : java Main <registre hexadécimal 16 bits, ex. 0x4A0B>");
                System.exit(1);
            }
            return;
        }
        decoder(construire(74, 2, true));     // en charge rapide, 74 %
        decoder(0x640F);                      // charge terminée à 100 %
        decoder(0x8A50);                      // surchauffe + erreur + alerte
    }
}
