/*
 * Jour 075 — Jeu du pendu avec mots en français
 *
 * Problème : le pendu est un bon exercice pour manipuler des chaînes : il faut
 * accepter « e » pour « é », « è » ou « ê », afficher les lettres trouvées et
 * gérer les erreurs de saisie.
 * Solution : chaque mot est normalisé (accents supprimés avec
 * java.text.Normalizer) pour la comparaison, mais affiché avec ses accents.
 * Un Set mémorise les lettres proposées. En démo, un joueur automatique
 * propose les lettres dans l'ordre de fréquence du français (e, a, s, i…) ;
 * avec --jouer, c'est vous qui jouez.
 * On apprend : Normalizer et les caractères Unicode, les Set, la boucle de jeu,
 * Scanner pour lire la saisie et l'affichage progressif d'un dessin ASCII.
 */
import java.text.Normalizer;
import java.util.*;

public class Main {
    static final String[] MOTS = {"électronique", "microcontrôleur", "capteur", "université", "téléphone",
            "marché", "programmation", "lumière", "réseau", "zémidjan"};
    static final String[] POTENCE = {"", " O", " O\n |", " O\n/|", " O\n/|\\", " O\n/|\\\n/", " O\n/|\\\n/ \\"};
    static final int ESSAIS = POTENCE.length - 1;

    static String sansAccent(String s) {
        return Normalizer.normalize(s, Normalizer.Form.NFD).replaceAll("\\p{M}", "");
    }

    static String masque(String mot, Set<Character> trouvees) {
        StringBuilder sb = new StringBuilder();
        String base = sansAccent(mot);
        for (int i = 0; i < mot.length(); i++)
            sb.append(trouvees.contains(base.charAt(i)) ? mot.charAt(i) : '_').append(' ');
        return sb.toString().trim();
    }

    interface Joueur { char proposer(Set<Character> dejaDites); }

    static boolean partie(String mot, Joueur joueur, boolean verbeux) {
        String base = sansAccent(mot);
        Set<Character> dites = new TreeSet<>();
        int erreurs = 0;
        while (erreurs < ESSAIS && masque(mot, dites).contains("_")) {
            char c = joueur.proposer(dites);
            if (!dites.add(c)) continue;
            boolean bon = base.indexOf(c) >= 0;
            if (!bon) erreurs++;
            if (verbeux) System.out.printf("  %c → %-5s %s  (erreurs %d/%d)%n", c, bon ? "oui" : "non", masque(mot, dites), erreurs, ESSAIS);
        }
        boolean gagne = !masque(mot, dites).contains("_");
        if (verbeux) {
            if (!gagne) System.out.println(POTENCE[ESSAIS]);
            System.out.println(gagne ? "  Gagné ! Le mot était « " + mot + " »" : "  Perdu ! Le mot était « " + mot + " »");
        }
        return gagne;
    }

    public static void main(String[] args) {
        Random rng = new Random(75);
        if (args.length == 1 && args[0].equals("--jouer")) {
            Scanner in = new Scanner(System.in);
            String mot = MOTS[new Random().nextInt(MOTS.length)];
            partie(mot, dejaDites -> {
                while (true) {
                    System.out.print("Lettre : ");
                    if (!in.hasNextLine()) System.exit(0);
                    String s = sansAccent(in.nextLine().trim().toLowerCase(Locale.ROOT));
                    if (s.length() == 1 && Character.isLetter(s.charAt(0))) return s.charAt(0);
                    System.out.println("Une seule lettre, s'il vous plaît.");
                }
            }, true);
            return;
        }
        if (args.length > 0) { System.err.println("Usage : java Main [--jouer]"); System.exit(1); }
        String ordreFrequence = "easitnrulodcmpvqfbghjxyzwk";
        Joueur auto = dites -> { for (char c : ordreFrequence.toCharArray()) if (!dites.contains(c)) return c; return 'a'; };
        String mot = MOTS[rng.nextInt(MOTS.length)];
        System.out.println("Démo : joueur automatique (lettres par fréquence) sur « " + masque(mot, Set.of()) + " »");
        partie(mot, auto, true);
        int victoires = 0;
        for (String m : MOTS) if (partie(m, auto, false)) victoires++;
        System.out.printf("%nCette stratégie gagne %d partie(s) sur %d.%n", victoires, MOTS.length);
    }
}
