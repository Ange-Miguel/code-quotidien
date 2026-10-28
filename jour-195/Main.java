/*
 * Jour 195 — Jeu de la vie de Conway
 *
 * Problème : comment des règles très simples, appliquées localement, peuvent-
 * elles produire des comportements complexes : structures stables,
 * oscillateurs, « vaisseaux » qui se déplacent ? Le jeu de la vie (John
 * Conway, 1970) est l'exemple le plus célèbre d'automate cellulaire, utilisé
 * pour illustrer l'émergence en biologie, en physique et en informatique.
 * Solution : une grille de cellules vivantes ou mortes. À chaque génération :
 * une cellule vivante avec 2 ou 3 voisines survit ; une cellule morte avec
 * exactement 3 voisines naît ; toutes les autres meurent ou restent mortes.
 * On calcule la génération suivante dans une seconde grille (les deux
 * tableaux sont échangés), sur un tore (les bords se rejoignent). On détecte
 * un état déjà vu (cycle) grâce au hachage des grilles.
 * On apprend : les automates cellulaires, le double tampon, l'arithmétique
 * modulaire pour un monde torique et la détection de cycle par hachage.
 */
import java.util.*;

public class Main {
    final int h, l;
    boolean[][] grille, suivante;

    Main(int h, int l) { this.h = h; this.l = l; grille = new boolean[h][l]; suivante = new boolean[h][l]; }

    void placer(int ligne, int col, String... motif) {
        for (int i = 0; i < motif.length; i++)
            for (int j = 0; j < motif[i].length(); j++)
                if (motif[i].charAt(j) == '#') grille[(ligne + i) % h][(col + j) % l] = true;
    }

    int voisines(int i, int j) {
        int n = 0;
        for (int di = -1; di <= 1; di++)
            for (int dj = -1; dj <= 1; dj++)
                if ((di != 0 || dj != 0) && grille[(i + di + h) % h][(j + dj + l) % l]) n++;   // tore
        return n;
    }

    void generation() {
        for (int i = 0; i < h; i++)
            for (int j = 0; j < l; j++) {
                int n = voisines(i, j);
                suivante[i][j] = grille[i][j] ? (n == 2 || n == 3) : n == 3;
            }
        boolean[][] t = grille; grille = suivante; suivante = t;          // double tampon
    }

    int population() { int n = 0; for (boolean[] r : grille) for (boolean c : r) if (c) n++; return n; }

    String empreinte() { return Arrays.deepToString(grille); }

    void afficher(String titre) {
        System.out.println(titre + " (population " + population() + ")");
        for (boolean[] r : grille) {
            StringBuilder sb = new StringBuilder("   ");
            for (boolean c : r) sb.append(c ? "█" : "·");
            System.out.println(sb);
        }
    }

    public static void main(String[] args) {
        if (args.length > 1) { System.err.println("Usage : java Main [graine pour une grille aléatoire]"); System.exit(1); }
        if (args.length == 1) {
            try {
                Main m = new Main(20, 40);
                Random r = new Random(Long.parseLong(args[0]));
                for (int i = 0; i < 20; i++) for (int j = 0; j < 40; j++) m.grille[i][j] = r.nextDouble() < 0.3;
                Map<String, Integer> vus = new HashMap<>();
                for (int g = 0; g < 2000; g++) {
                    Integer avant = vus.put(m.empreinte(), g);
                    if (avant != null) { System.out.printf("Cycle de période %d atteint à la génération %d, population %d%n", g - avant, g, m.population()); return; }
                    m.generation();
                }
                System.out.println("Pas de cycle en 2000 générations, population " + m.population());
                return;
            } catch (NumberFormatException e) { System.err.println("Graine invalide : " + args[0]); System.exit(1); }
        }
        Main m = new Main(12, 30);
        m.placer(1, 1, ".#.", "..#", "###");                  // planeur : se déplace en diagonale
        m.placer(2, 14, "###");                                // clignotant : période 2
        m.placer(6, 22, "##", "##");                           // bloc : stable
        m.placer(7, 4, ".##", "##.", ".#.");                   // R-pentomino : longue évolution
        m.afficher("Génération 0");
        Map<String, Integer> vus = new HashMap<>();
        for (int g = 1; g <= 400; g++) {
            m.generation();
            if (g == 4 || g == 30) m.afficher("\nGénération " + g);
            Integer avant = vus.put(m.empreinte(), g);
            if (avant != null) { System.out.printf("%nÉtat déjà vu : cycle de période %d détecté à la génération %d.%n", g - avant, g); return; }
        }
        System.out.println("\nAucun cycle détecté en 400 générations, population " + m.population());
    }
}
