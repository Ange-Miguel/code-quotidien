/*
 * Jour 009 — Partage d'une addition entre amis avec pourboire
 *
 * Problème : au maquis ou au restaurant, il faut partager l'addition, ajouter un
 * pourboire et arrondir pour que chacun paie un montant pratique.
 * Solution : on calcule le total avec pourboire, la part de chacun arrondie
 * au multiple de 50 FCFA supérieur, puis le surplus éventuel collecté (qui peut
 * servir de pourboire en plus).
 * On apprend : les calculs avec des entiers pour éviter les erreurs d'arrondi
 * de la monnaie, l'arrondi au multiple supérieur et le formatage avec printf.
 */
public class Main {
    static final int ARRONDI = 50; // plus petite somme pratique à rendre

    static long arrondirAuDessus(long montant, int pas) {
        return ((montant + pas - 1) / pas) * pas;
    }

    static void partager(long addition, int personnes, int pourcentagePourboire) {
        if (addition <= 0 || personnes <= 0 || pourcentagePourboire < 0 || pourcentagePourboire > 50)
            throw new IllegalArgumentException("valeurs hors limites");
        long pourboire = addition * pourcentagePourboire / 100;
        long total = addition + pourboire;
        long exacte = total / personnes;
        long part = arrondirAuDessus((total + personnes - 1) / personnes, ARRONDI);
        long collecte = part * personnes;

        System.out.printf("Addition : %,d FCFA | Pourboire %d %% : %,d FCFA%n", addition, pourcentagePourboire, pourboire);
        System.out.printf("Total : %,d FCFA pour %d personne(s)%n", total, personnes);
        System.out.printf("Part exacte : %,d FCFA → chacun paie %,d FCFA%n", exacte, part);
        System.out.printf("Collecté : %,d FCFA (surplus %,d FCFA)%n%n", collecte, collecte - total);
    }

    public static void main(String[] args) {
        java.util.Locale.setDefault(java.util.Locale.FRANCE);
        try {
            if (args.length == 0) {
                partager(23500, 4, 10);
                partager(18000, 3, 0);
                partager(47250, 6, 5);
            } else if (args.length == 3) {
                partager(Long.parseLong(args[0]), Integer.parseInt(args[1]), Integer.parseInt(args[2]));
            } else {
                throw new IllegalArgumentException("nombre d'arguments incorrect");
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.err.println("Usage : java Main <addition> <personnes> <pourboire %>");
            System.exit(1);
        }
    }
}
