/*
 * Jour 186 — Limiteur de débit (rate limiter) pour une API
 *
 * Problème : une API publique (envoi de SMS, consultation de stock,
 * connexion) peut être saturée par un client trop gourmand ou attaquée par
 * un robot qui essaie des milliers de mots de passe. Il faut limiter le nombre
 * de requêtes par client, sans gêner les utilisateurs normaux.
 * Solution : l'algorithme du seau à jetons (token bucket). Chaque client a un
 * seau de capacité C qui se remplit de R jetons par seconde ; chaque requête
 * consomme un jeton ; seau vide = requête refusée (code HTTP 429). Cela
 * autorise de courtes rafales tout en limitant le débit moyen. On compare
 * avec la fenêtre fixe, qui laisse passer le double de requêtes à la
 * frontière entre deux fenêtres. L'horloge est simulée pour la démonstration.
 * On apprend : le seau à jetons, la fenêtre fixe et son défaut, le calcul
 * paresseux du remplissage et la gestion par client avec une Map.
 */
import java.util.*;

public class Main {
    interface Limiteur { boolean autoriser(String client, double t); }

    static class SeauAJetons implements Limiteur {
        final double capacite, parSeconde;
        final Map<String, double[]> seaux = new HashMap<>();   // {jetons, dernier instant}
        SeauAJetons(double capacite, double parSeconde) { this.capacite = capacite; this.parSeconde = parSeconde; }
        public boolean autoriser(String client, double t) {
            double[] s = seaux.computeIfAbsent(client, k -> new double[]{capacite, t});
            s[0] = Math.min(capacite, s[0] + (t - s[1]) * parSeconde);   // remplissage calculé à la demande
            s[1] = t;
            if (s[0] < 1) return false;
            s[0] -= 1;
            return true;
        }
    }

    static class FenetreFixe implements Limiteur {
        final int max; final double duree;
        final Map<String, long[]> compteurs = new HashMap<>();   // {numéro de fenêtre, nombre}
        FenetreFixe(int max, double duree) { this.max = max; this.duree = duree; }
        public boolean autoriser(String client, double t) {
            long fenetre = (long) (t / duree);
            long[] c = compteurs.computeIfAbsent(client, k -> new long[]{fenetre, 0});
            if (c[0] != fenetre) { c[0] = fenetre; c[1] = 0; }
            return ++c[1] <= max;
        }
    }

    static int[] rejouer(Limiteur l, String client, double[] instants) {
        int ok = 0, refus = 0;
        for (double t : instants) if (l.autoriser(client, t)) ok++; else refus++;
        return new int[]{ok, refus};
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        System.out.println("Règle : 10 requêtes par minute en moyenne, rafale de 5 autorisée\n");
        double[] normal = new double[20];
        for (int i = 0; i < 20; i++) normal[i] = i * 7.0;                       // un utilisateur calme : 1 requête / 7 s
        double[] robot = new double[300];
        for (int i = 0; i < 300; i++) robot[i] = i * 0.2;                      // robot : 5 requêtes / s pendant 1 min
        double[] frontiere = new double[20];
        for (int i = 0; i < 20; i++) frontiere[i] = 58 + i * 0.2;              // 20 requêtes autour de t = 60 s
        Object[][] cas = {{"Utilisateur normal", normal}, {"Robot (attaque)", robot}, {"Rafale à la frontière", frontiere}};
        System.out.printf("%-24s %22s %22s%n", "Scénario", "Seau à jetons", "Fenêtre fixe (1 min)");
        for (Object[] c : cas) {
            int[] a = rejouer(new SeauAJetons(5, 10 / 60.0), "client", (double[]) c[1]);
            int[] b = rejouer(new FenetreFixe(10, 60), "client", (double[]) c[1]);
            System.out.printf("%-24s %10d ok / %3d 429 %10d ok / %3d 429%n", c[0], a[0], a[1], b[0], b[1]);
        }
        System.out.println("\nLa fenêtre fixe laisse passer 20 requêtes en 4 secondes autour de t = 60 s (10 + 10) ;");
        System.out.println("le seau à jetons n'en accepte que la rafale prévue, puis le débit moyen.");
        SeauAJetons seau = new SeauAJetons(5, 10 / 60.0);
        System.out.println("\nClients indépendants : " + seau.autoriser("ip-41.85.x.x", 0) + " / " + seau.autoriser("ip-102.64.x.x", 0));
    }
}
