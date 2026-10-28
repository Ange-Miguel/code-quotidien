/*
 * Jour 045 — Calcul de la quantité de ciment et de sable pour une dalle
 *
 * Problème : pour couler une dalle (terrasse, sol de boutique, fondation
 * légère), il faut commander le bon nombre de sacs de ciment, de brouettes de
 * sable et de gravier. Une erreur coûte cher ou bloque le chantier.
 * Solution : volume = longueur × largeur × épaisseur. On applique un dosage
 * classique de béton (350 kg de ciment par m³, 400 L de sable, 800 L de
 * gravier, 175 L d'eau), majoré de 5 % pour les pertes, et on convertit en
 * sacs de 50 kg et en brouettes de 60 L. Valeurs indicatives à faire valider
 * par un professionnel.
 * On apprend : les enums avec attributs, l'arrondi au supérieur (Math.ceil)
 * et la conversion d'un besoin en unités d'achat.
 */
import java.util.Locale;

public class Main {
    enum Dosage {
        DALLE(350, 400, 800), FONDATION(300, 400, 800), CHAPE(400, 1000, 0);
        final int cimentKgParM3, sableLParM3, gravierLParM3;
        Dosage(int c, int s, int g) { cimentKgParM3 = c; sableLParM3 = s; gravierLParM3 = g; }
    }

    static final double PERTES = 1.05;
    static final int SAC_KG = 50, BROUETTE_L = 60, PRIX_SAC = 5_200;

    static void calculer(String nom, double longueur, double largeur, double epaisseurCm, Dosage d) {
        if (longueur <= 0 || largeur <= 0 || epaisseurCm <= 0 || epaisseurCm > 100)
            throw new IllegalArgumentException("dimensions invalides");
        double volume = longueur * largeur * epaisseurCm / 100 * PERTES;
        int sacs = (int) Math.ceil(volume * d.cimentKgParM3 / SAC_KG);
        int brouettesSable = (int) Math.ceil(volume * d.sableLParM3 / BROUETTE_L);
        int brouettesGravier = (int) Math.ceil(volume * d.gravierLParM3 / BROUETTE_L);
        System.out.printf("%s (%.1f × %.1f m, %.0f cm, dosage %s)%n", nom, longueur, largeur, epaisseurCm, d);
        System.out.printf("  Volume avec pertes : %.2f m³%n", volume);
        System.out.printf("  Ciment  : %d sacs de %d kg (~%,d FCFA)%n", sacs, SAC_KG, sacs * PRIX_SAC);
        System.out.printf("  Sable   : %d brouettes (%.0f L)%n", brouettesSable, volume * d.sableLParM3);
        if (d.gravierLParM3 > 0)
            System.out.printf("  Gravier : %d brouettes (%.0f L)%n", brouettesGravier, volume * d.gravierLParM3);
        System.out.printf("  Eau     : ~%.0f L%n%n", volume * 175);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        try {
            if (args.length == 0) {
                calculer("Terrasse", 5, 4, 12, Dosage.DALLE);
                calculer("Sol de boutique", 6, 3.5, 5, Dosage.CHAPE);
                System.out.println("Valeurs indicatives : faites valider le dosage par un maçon ou un ingénieur.");
            } else if (args.length == 3 || args.length == 4) {
                Dosage d = args.length == 4 ? Dosage.valueOf(args[3].toUpperCase(Locale.ROOT)) : Dosage.DALLE;
                calculer("Votre ouvrage", Double.parseDouble(args[0]), Double.parseDouble(args[1]),
                        Double.parseDouble(args[2]), d);
            } else {
                throw new IllegalArgumentException("nombre d'arguments incorrect");
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.err.println("Usage : java Main <longueur m> <largeur m> <épaisseur cm> [DALLE|FONDATION|CHAPE]");
            System.exit(1);
        }
    }
}
