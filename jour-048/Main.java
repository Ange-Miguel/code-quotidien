/*
 * Jour 048 — Inversion des mots d'une phrase
 *
 * Problème : un classique des entretiens techniques : inverser l'ordre des mots
 * d'une phrase (« le chat dort » → « dort chat le »), en gérant les espaces
 * multiples, et le faire en place, sans tableau supplémentaire.
 * Solution : trois versions. (1) Simple avec split et une liste inversée.
 * (2) Avec une pile (Deque). (3) En place sur un tableau de caractères :
 * on inverse toute la chaîne, puis chaque mot individuellement. On vérifie
 * que les trois donnent le même résultat. Bonus : inverser les lettres de
 * chaque mot en gardant l'ordre des mots.
 * On apprend : les piles, l'inversion en place avec deux indices, la
 * complexité mémoire O(1) et la comparaison de plusieurs implémentations.
 */
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Collections;
import java.util.Deque;
import java.util.List;

public class Main {
    static String avecListe(String phrase) {
        List<String> mots = Arrays.asList(phrase.trim().split("\\s+"));
        Collections.reverse(mots);
        return String.join(" ", mots);
    }

    static String avecPile(String phrase) {
        Deque<String> pile = new ArrayDeque<>();
        for (String mot : phrase.trim().split("\\s+")) pile.push(mot);
        return String.join(" ", pile);
    }

    static void inverser(char[] t, int debut, int fin) {
        while (debut < fin) {
            char c = t[debut];
            t[debut++] = t[fin];
            t[fin--] = c;
        }
    }

    static String enPlace(String phrase) {
        char[] t = phrase.trim().replaceAll("\\s+", " ").toCharArray();
        inverser(t, 0, t.length - 1);
        int debut = 0;
        for (int i = 0; i <= t.length; i++) {
            if (i == t.length || t[i] == ' ') {
                inverser(t, debut, i - 1);
                debut = i + 1;
            }
        }
        return new String(t);
    }

    static String lettresDeChaqueMot(String phrase) {
        StringBuilder sb = new StringBuilder();
        for (String mot : phrase.trim().split("\\s+"))
            sb.append(new StringBuilder(mot).reverse()).append(' ');
        return sb.toString().trim();
    }

    public static void main(String[] args) {
        String[] phrases = args.length > 0 ? new String[]{String.join(" ", args)}
                : new String[]{"le zem attend devant le campus", "  Calavi   est   ma   ville  ", "un"};
        for (String p : phrases) {
            String a = avecListe(p), b = avecPile(p), c = enPlace(p);
            if (!a.equals(b) || !b.equals(c)) throw new IllegalStateException("résultats différents");
            System.out.printf("« %s »%n   mots inversés   : « %s »%n   lettres inversées : « %s »%n",
                    p, c, lettresDeChaqueMot(p));
        }
    }
}
