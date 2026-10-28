/*
 * Jour 126 — Ordonnanceur coopératif de tâches (super-loop)
 *
 * Problème : un microcontrôleur sans système d'exploitation doit faire
 * plusieurs choses « en même temps » : lire un capteur toutes les 100 ms,
 * faire clignoter une LED toutes les 500 ms, envoyer les données toutes les
 * 2 s, surveiller un bouton. Avec des delay(), tout se bloque.
 * Solution : un ordonnanceur coopératif. Chaque tâche a une période et la
 * date de sa prochaine exécution. La boucle principale (super-loop) regarde
 * l'horloge et lance les tâches dues, qui s'exécutent rapidement et rendent
 * la main. On simule une horloge en millisecondes, on mesure la gigue
 * (retard) de chaque tâche, et on montre l'effet d'une tâche trop longue qui
 * retarde toutes les autres : c'est la limite du modèle coopératif.
 * On apprend : le multitâche coopératif, les tâches périodiques, la gestion
 * du temps sans blocage et la mesure de la gigue d'exécution.
 */
import java.util.*;

public class Main {
    static long maintenant = 0;   // horloge simulée en ms

    static class Tache {
        final String nom; final long periode; final int dureeMs; final Runnable action;
        long prochaine, executions, retardMax, retardTotal;
        Tache(String nom, long periode, int dureeMs, Runnable action) {
            this.nom = nom; this.periode = periode; this.dureeMs = dureeMs; this.action = action;
        }
    }

    static void superLoop(List<Tache> taches, long dureeTotale, boolean trace) {
        for (Tache t : taches) { t.prochaine = 0; t.executions = t.retardMax = t.retardTotal = 0; }
        maintenant = 0;
        while (maintenant < dureeTotale) {
            boolean active = false;
            for (Tache t : taches) {
                if (maintenant >= t.prochaine) {
                    long retard = maintenant - t.prochaine;
                    t.retardMax = Math.max(t.retardMax, retard);
                    t.retardTotal += retard;
                    if (trace && t.executions < 2) System.out.printf("   t=%5d ms  %-16s (retard %d ms)%n", maintenant, t.nom, retard);
                    t.action.run();
                    maintenant += t.dureeMs;             // la tâche consomme du temps processeur
                    t.executions++;
                    t.prochaine += t.periode;            // pas de dérive : on ajoute la période
                    active = true;
                }
            }
            if (!active) maintenant++;                   // rien à faire : on avance l'horloge (ou on dort)
        }
    }

    static void rapport(List<Tache> taches) {
        System.out.printf("   %-16s %8s %10s %12s %12s%n", "Tâche", "Période", "Exécutions", "Retard moy.", "Retard max");
        for (Tache t : taches)
            System.out.printf("   %-16s %6d ms %10d %9.1f ms %9d ms%n", t.nom, t.periode, t.executions,
                    (double) t.retardTotal / Math.max(1, t.executions), t.retardMax);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        int[] led = {0};
        List<Tache> taches = new ArrayList<>(List.of(
                new Tache("lire capteur", 100, 2, () -> { }),
                new Tache("clignoter LED", 500, 1, () -> led[0] ^= 1),
                new Tache("lire bouton", 20, 1, () -> { }),
                new Tache("envoyer données", 2000, 15, () -> { })));
        System.out.println("10 secondes simulées, tâches courtes :");
        superLoop(taches, 10_000, true);
        rapport(taches);
        System.out.println("\nOn ajoute une tâche mal écrite qui bloque 300 ms (calcul ou delay) :");
        taches.add(new Tache("calcul bloquant", 1000, 300, () -> { }));
        superLoop(taches, 10_000, false);
        rapport(taches);
        System.out.println("\nLe bouton peut être raté pendant 300 ms : il faut découper les tâches longues.");
    }
}
