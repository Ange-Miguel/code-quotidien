/*
 * Jour 174 — Générateur de code-barres 2D (matrice simplifiée, façon QR)
 *
 * Problème : comment un QR code range-t-il du texte dans une grille de carrés
 * noirs et blancs, et comment le lecteur retrouve-t-il l'orientation et
 * détecte-t-il les erreurs ? Le vrai format QR (avec correction Reed-Solomon)
 * est long ; on construit ici une version pédagogique qui reprend ses idées.
 * ATTENTION : ce n'est pas un vrai QR code, un téléphone ne pourra pas le lire.
 * Solution : la grille 21×21 contient trois motifs de repérage dans les coins
 * (comme le QR), des lignes de synchronisation, puis les données : longueur,
 * octets du texte et un octet de parité XOR, écrits bit par bit en zigzag. Un
 * masque (damier) évite les grandes zones uniformes. Le décodeur relit la
 * grille, retire le masque, vérifie la parité et signale une grille abîmée.
 * On apprend : les tableaux 2D de booléens, l'écriture et la lecture de bits,
 * les masques XOR, le parcours en zigzag et le principe des codes 2D.
 */
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class Main {
    static final int N = 21;

    static boolean reserve(int l, int c) {
        boolean coin = (l < 8 && c < 8) || (l < 8 && c >= N - 8) || (l >= N - 8 && c < 8);
        return coin || l == 6 || c == 6;
    }

    static List<int[]> ordreZigzag() {
        List<int[]> cases = new ArrayList<>();
        boolean monte = true;
        for (int c = N - 1; c > 0; c -= 2) {
            if (c == 6) c--;
            for (int k = 0; k < N; k++) {
                int l = monte ? N - 1 - k : k;
                for (int d = 0; d < 2; d++) if (!reserve(l, c - d)) cases.add(new int[]{l, c - d});
            }
            monte = !monte;
        }
        return cases;
    }

    static boolean masque(int l, int c) { return (l + c) % 2 == 0; }

    static boolean[][] encoder(String texte) {
        byte[] octets = texte.getBytes(StandardCharsets.UTF_8);
        List<int[]> cases = ordreZigzag();
        if ((octets.length + 2) * 8 > cases.size()) throw new IllegalArgumentException("texte trop long (" + octets.length + " octets)");
        boolean[][] g = new boolean[N][N];
        for (int[] o : new int[][]{{0, 0}, {0, N - 7}, {N - 7, 0}})            // motifs de repérage 7×7
            for (int i = 0; i < 7; i++) for (int j = 0; j < 7; j++)
                g[o[0] + i][o[1] + j] = i == 0 || i == 6 || j == 0 || j == 6 || (i >= 2 && i <= 4 && j >= 2 && j <= 4);
        for (int i = 8; i < N - 8; i++) { g[6][i] = i % 2 == 0; g[i][6] = i % 2 == 0; }   // synchronisation
        byte parite = 0;
        List<Byte> flux = new ArrayList<>(List.of((byte) octets.length));
        for (byte b : octets) { flux.add(b); parite ^= b; }
        flux.add(parite);
        int k = 0;
        for (byte b : flux)
            for (int bit = 7; bit >= 0; bit--, k++) { int[] p = cases.get(k); g[p[0]][p[1]] = (((b >> bit) & 1) == 1) ^ masque(p[0], p[1]); }
        for (; k < cases.size(); k++) { int[] p = cases.get(k); g[p[0]][p[1]] = masque(p[0], p[1]) ^ (k % 3 == 0); }   // remplissage
        return g;
    }

    static String decoder(boolean[][] g) {
        List<int[]> cases = ordreZigzag();
        int[] pos = {0};
        java.util.function.IntSupplier octet = () -> {
            int v = 0;
            for (int bit = 0; bit < 8; bit++) { int[] p = cases.get(pos[0]++); v = (v << 1) | ((g[p[0]][p[1]] ^ masque(p[0], p[1])) ? 1 : 0); }
            return v;
        };
        int longueur = octet.getAsInt();
        if ((longueur + 2) * 8 > cases.size()) return null;
        byte[] donnees = new byte[longueur];
        byte parite = 0;
        for (int i = 0; i < longueur; i++) { donnees[i] = (byte) octet.getAsInt(); parite ^= donnees[i]; }
        return (byte) octet.getAsInt() == parite ? new String(donnees, StandardCharsets.UTF_8) : null;
    }

    static void afficher(boolean[][] g) {
        for (boolean[] ligne : g) {
            StringBuilder sb = new StringBuilder("  ");
            for (boolean b : ligne) sb.append(b ? "██" : "  ");
            System.out.println(sb);
        }
    }

    public static void main(String[] args) {
        String texte = args.length > 0 ? String.join(" ", args) : "Tél +229 01 66 54 61 44";
        boolean[][] g;
        try { g = encoder(texte); } catch (IllegalArgumentException e) { System.err.println("Erreur : " + e.getMessage()); System.exit(1); return; }
        System.out.println("Matrice 21×21 pour « " + texte + " » (format pédagogique, pas un vrai QR code) :");
        afficher(g);
        System.out.println("Décodage : « " + decoder(g) + " »");
        g[12][15] = !g[12][15];                                               // une tache sur l'étiquette
        String abime = decoder(g);
        System.out.println("Après une case abîmée : " + (abime == null ? "parité incorrecte → lecture refusée" : "« " + abime + " »"));
        System.out.println("(Le vrai QR code utilise Reed-Solomon pour corriger les erreurs, pas seulement les détecter.)");
    }
}
