/*
 * Jour 036 — Détection des nombres premiers (crible d'Ératosthène)
 *
 * Problème : les nombres premiers sont au cœur de la cryptographie (RSA) et des
 * tables de hachage. Il faut savoir les trouver efficacement.
 * Solution : le crible d'Ératosthène marque comme composés tous les multiples
 * de chaque nombre premier, en partant de son carré. Pour un test isolé, on
 * divise seulement jusqu'à la racine carrée. On compare le temps des deux
 * méthodes et on vérifie le nombre de premiers sous 1 000 000 (78 498).
 * On apprend : les tableaux de booléens, la complexité O(n log log n),
 * BitSet, la mesure du temps d'exécution et l'optimisation par la racine.
 */
import java.util.BitSet;
import java.util.Locale;

public class Main {
    static BitSet crible(int n) {
        BitSet compose = new BitSet(n + 1);
        compose.set(0);
        compose.set(1);
        for (long i = 2; i * i <= n; i++)
            if (!compose.get((int) i))
                for (long j = i * i; j <= n; j += i) compose.set((int) j);
        return compose;
    }

    static boolean estPremier(long n) {
        if (n < 2) return false;
        if (n % 2 == 0) return n == 2;
        for (long d = 3; d * d <= n; d += 2)
            if (n % d == 0) return false;
        return true;
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) {
            for (String a : args) {
                try {
                    long n = Long.parseLong(a);
                    System.out.printf("%,d est %s%n", n, estPremier(n) ? "premier" : "composé");
                } catch (NumberFormatException e) {
                    System.err.println("Nombre invalide : " + a);
                    System.exit(1);
                }
            }
            return;
        }
        int n = 1_000_000;
        long t0 = System.nanoTime();
        BitSet compose = crible(n);
        long t1 = System.nanoTime();
        int nb = n + 1 - compose.cardinality();
        System.out.printf("Crible jusqu'à %,d : %,d nombres premiers en %.1f ms%n", n, nb, (t1 - t0) / 1e6);
        if (nb != 78_498) throw new IllegalStateException("résultat inattendu");

        int compteur = 0;
        for (int i = 2; i <= n; i++) if (estPremier(i)) compteur++;
        long t2 = System.nanoTime();
        System.out.printf("Test un par un : %,d premiers en %.1f ms%n", compteur, (t2 - t1) / 1e6);

        System.out.print("Les 20 premiers : ");
        for (int i = 2, vus = 0; vus < 20; i++) if (!compose.get(i)) { System.out.print(i + " "); vus++; }
        System.out.println();
        for (long x : new long[]{97, 1_000_003, 2_147_483_647L, 600_851_475_143L})
            System.out.printf("%,d : %s%n", x, estPremier(x) ? "premier" : "composé");
    }
}
