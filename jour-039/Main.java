/*
 * Jour 039 — Calcul de factorielle et nombre de combinaisons possibles
 *
 * Problème : combien de façons de choisir 5 numéros sur 90 au loto ? De ranger
 * 8 produits sur une étagère ? De former un bureau de 3 personnes parmi 12 ?
 * Ces nombres explosent vite et dépassent la capacité des types int et long.
 * Solution : factorielle avec BigInteger (précision illimitée), arrangements
 * A(n, k) = n!/(n−k)! et combinaisons C(n, k) calculées sans grande factorielle,
 * en multipliant et divisant pas à pas. On montre où « long » déborde.
 * On apprend : BigInteger, le dépassement de capacité, Math.multiplyExact et
 * le dénombrement (permutations, arrangements, combinaisons).
 */
import java.math.BigInteger;
import java.util.Locale;

public class Main {
    static BigInteger factorielle(int n) {
        BigInteger r = BigInteger.ONE;
        for (int i = 2; i <= n; i++) r = r.multiply(BigInteger.valueOf(i));
        return r;
    }

    static BigInteger combinaisons(int n, int k) {
        if (k < 0 || k > n) return BigInteger.ZERO;
        k = Math.min(k, n - k);
        BigInteger r = BigInteger.ONE;
        for (int i = 1; i <= k; i++)  // r reste entier à chaque étape
            r = r.multiply(BigInteger.valueOf(n - k + i)).divide(BigInteger.valueOf(i));
        return r;
    }

    static BigInteger arrangements(int n, int k) {
        BigInteger r = BigInteger.ONE;
        for (int i = 0; i < k; i++) r = r.multiply(BigInteger.valueOf(n - i));
        return r;
    }

    static int premierDebordementLong() {
        long f = 1;
        for (int i = 2; ; i++) {
            try {
                f = Math.multiplyExact(f, i);
            } catch (ArithmeticException e) {
                return i;
            }
        }
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length == 2) {
            try {
                int n = Integer.parseInt(args[0]), k = Integer.parseInt(args[1]);
                if (n < 0 || n > 5000 || k < 0 || k > n) throw new NumberFormatException();
                System.out.printf("C(%d, %d) = %s%nA(%d, %d) = %s%n", n, k, combinaisons(n, k), n, k, arrangements(n, k));
                return;
            } catch (NumberFormatException e) {
                System.err.println("Usage : java Main <n> <k> avec 0 ≤ k ≤ n ≤ 5000");
                System.exit(1);
            }
        }
        if (args.length != 0) {
            System.err.println("Usage : java Main <n> <k>");
            System.exit(1);
        }
        for (int n : new int[]{5, 10, 20, 25}) System.out.printf("%d! = %s%n", n, factorielle(n));
        System.out.printf("Le type long déborde à %d!%n%n", premierDebordementLong());
        System.out.printf("Loto 5 numéros sur 90 : %,d combinaisons%n", combinaisons(90, 5));
        System.out.printf("Ranger 8 produits sur une étagère : %,d ordres%n", factorielle(8));
        System.out.printf("Président, secrétaire, trésorier parmi 12 : %,d bureaux%n", arrangements(12, 3));
        System.out.printf("Équipe de 3 sans rôle parmi 12 : %,d équipes%n", combinaisons(12, 3));
        if (!combinaisons(52, 5).equals(factorielle(52).divide(factorielle(5).multiply(factorielle(47)))))
            throw new IllegalStateException("erreur de calcul");
    }
}
