/*
 * Jour 060 — Tri à bulles, sélection et insertion comparés
 *
 * Problème : trier est l'une des opérations les plus fréquentes en
 * informatique. Les trois tris « simples » sont faciles à écrire, mais leurs
 * performances diffèrent selon les données (aléatoires, presque triées,
 * inversées).
 * Solution : on implémente les trois tris en comptant les comparaisons et les
 * échanges, on les lance sur les mêmes jeux de données et on compare avec
 * Arrays.sort. Le tri par insertion est très rapide sur des données presque
 * triées, le tri par sélection fait toujours le même nombre de comparaisons.
 * On apprend : les algorithmes de tri en O(n²), la mesure expérimentale de la
 * complexité, les cas favorables et défavorables, et la vérification d'un tri.
 */
import java.util.Arrays;
import java.util.Locale;
import java.util.Random;

public class Main {
    static long comparaisons, echanges;

    static void echanger(int[] t, int i, int j) { int x = t[i]; t[i] = t[j]; t[j] = x; echanges++; }

    static void bulles(int[] t) {
        for (int fin = t.length - 1; fin > 0; fin--) {
            boolean echange = false;
            for (int i = 0; i < fin; i++) {
                comparaisons++;
                if (t[i] > t[i + 1]) { echanger(t, i, i + 1); echange = true; }
            }
            if (!echange) return;   // déjà trié : on s'arrête
        }
    }

    static void selection(int[] t) {
        for (int i = 0; i < t.length - 1; i++) {
            int min = i;
            for (int j = i + 1; j < t.length; j++) { comparaisons++; if (t[j] < t[min]) min = j; }
            if (min != i) echanger(t, i, min);
        }
    }

    static void insertion(int[] t) {
        for (int i = 1; i < t.length; i++) {
            int cle = t[i], j = i - 1;
            while (j >= 0) {
                comparaisons++;
                if (t[j] <= cle) break;
                t[j + 1] = t[j]; echanges++; j--;
            }
            t[j + 1] = cle;
        }
    }

    interface Tri { void trier(int[] t); }

    static void mesurer(String nom, Tri tri, int[] donnees) {
        int[] copie = donnees.clone();
        int[] reference = donnees.clone();
        Arrays.sort(reference);
        comparaisons = echanges = 0;
        long t0 = System.nanoTime();
        tri.trier(copie);
        long duree = System.nanoTime() - t0;
        if (!Arrays.equals(copie, reference)) throw new IllegalStateException(nom + " : tri incorrect");
        System.out.printf("   %-10s %,12d comparaisons %,12d échanges %8.2f ms%n", nom, comparaisons, echanges, duree / 1e6);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        int n = 3000;
        if (args.length == 1) {
            try {
                n = Integer.parseInt(args[0]);
                if (n < 10 || n > 50_000) throw new NumberFormatException();
            } catch (NumberFormatException e) {
                System.err.println("Usage : java Main [taille entre 10 et 50000]");
                System.exit(1);
            }
        }
        Random r = new Random(60);
        int[] aleatoire = r.ints(n, 0, 100_000).toArray();
        int[] presqueTrie = aleatoire.clone();
        Arrays.sort(presqueTrie);
        for (int k = 0; k < n / 100; k++) echanger(presqueTrie, r.nextInt(n), r.nextInt(n));
        int[] inverse = Arrays.stream(presqueTrie).map(x -> -x).sorted().map(x -> -x).toArray();

        String[] noms = {"aléatoires", "presque triées", "inversées"};
        int[][] jeux = {aleatoire, presqueTrie, inverse};
        for (int i = 0; i < 3; i++) {
            System.out.printf("%,d données %s :%n", n, noms[i]);
            mesurer("Bulles", Main::bulles, jeux[i]);
            mesurer("Sélection", Main::selection, jeux[i]);
            mesurer("Insertion", Main::insertion, jeux[i]);
        }
    }
}
