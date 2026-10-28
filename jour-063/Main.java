/*
 * Jour 063 — Table de hachage : dictionnaire français → anglais
 *
 * Problème : comment un dictionnaire, un cache ou un annuaire retrouve-t-il un
 * mot parmi des millions en temps quasi constant ? Grâce aux tables de
 * hachage, qu'on utilise tous les jours avec HashMap sans les connaître.
 * Solution : on écrit notre propre table de hachage : une fonction de hachage
 * (polynomiale, comme String.hashCode) transforme le mot en indice de case ;
 * les collisions sont gérées par chaînage (liste dans chaque case) ; quand le
 * taux de remplissage dépasse 0,75, on double la taille et on redistribue.
 * On apprend : le hachage, les collisions, le facteur de charge, le
 * redimensionnement et les classes génériques en Java.
 */
import java.util.LinkedList;
import java.util.Locale;

public class Main {
    static class TableHachage<V> {
        private record Entree<V>(String cle, V valeur) {}
        private LinkedList<Entree<V>>[] cases;
        private int taille, collisions;

        @SuppressWarnings({"unchecked", "rawtypes"})
        TableHachage(int capacite) { cases = new LinkedList[capacite]; }

        private int indice(String cle, int capacite) {
            int h = 0;
            for (char c : cle.toCharArray()) h = 31 * h + c;   // hachage polynomial
            return Math.floorMod(h, capacite);
        }

        void mettre(String cle, V valeur) {
            if (taille + 1 > cases.length * 0.75) redimensionner();
            int i = indice(cle, cases.length);
            if (cases[i] == null) cases[i] = new LinkedList<>();
            else if (cases[i].removeIf(e -> e.cle().equals(cle))) taille--;
            else collisions++;
            cases[i].add(new Entree<>(cle, valeur));
            taille++;
        }

        V obtenir(String cle) {
            LinkedList<Entree<V>> l = cases[indice(cle, cases.length)];
            if (l != null) for (Entree<V> e : l) if (e.cle().equals(cle)) return e.valeur();
            return null;
        }

        @SuppressWarnings({"unchecked", "rawtypes"})
        private void redimensionner() {
            LinkedList<Entree<V>>[] anciennes = cases;
            cases = new LinkedList[anciennes.length * 2];
            System.out.printf("   (redimensionnement : %d → %d cases)%n", anciennes.length, cases.length);
            taille = 0;
            collisions = 0;
            for (LinkedList<Entree<V>> l : anciennes) if (l != null) for (Entree<V> e : l) mettre(e.cle(), e.valeur());
        }

        String statistiques() {
            int vides = 0, plusLongue = 0;
            for (LinkedList<Entree<V>> l : cases) {
                if (l == null) vides++;
                else plusLongue = Math.max(plusLongue, l.size());
            }
            return String.format("%d mots, %d cases (%d vides), chaîne la plus longue : %d", taille, cases.length, vides, plusLongue);
        }
    }

    static final String[][] MOTS = {
            {"bonjour", "hello"}, {"merci", "thank you"}, {"marché", "market"}, {"ordinateur", "computer"},
            {"capteur", "sensor"}, {"batterie", "battery"}, {"tension", "voltage"}, {"courant", "current"},
            {"carte", "board"}, {"logiciel", "software"}, {"réseau", "network"}, {"mémoire", "memory"},
            {"école", "school"}, {"étudiant", "student"}, {"prix", "price"}, {"livraison", "delivery"},
            {"commande", "order"}, {"maison", "house"}, {"lumière", "light"}, {"eau", "water"}};

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        TableHachage<String> dico = new TableHachage<>(4);
        System.out.println("Insertion de " + MOTS.length + " mots :");
        for (String[] m : MOTS) dico.mettre(m[0], m[1]);
        System.out.println("   " + dico.statistiques());
        String[] recherches = args.length > 0 ? args : new String[]{"capteur", "livraison", "réseau", "zemidjan"};
        boolean introuvable = false;
        for (String mot : recherches) {
            String t = dico.obtenir(mot.toLowerCase(Locale.ROOT));
            System.out.printf("%-12s → %s%n", mot, t == null ? "(absent du dictionnaire)" : t);
            introuvable |= t == null;
        }
        if (introuvable && args.length > 0) System.exit(1);
    }
}
