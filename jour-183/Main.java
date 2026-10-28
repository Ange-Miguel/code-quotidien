/*
 * Jour 183 — Répartition équitable des tâches ménagères
 *
 * Problème : dans une colocation ou une famille, les tâches (vaisselle,
 * balayage, courses, eau, linge) sont rarement réparties équitablement. Les
 * tâches pénibles retombent toujours sur les mêmes, et on se dispute.
 * Solution : chaque tâche a un « poids » (pénibilité × durée) et une
 * fréquence par semaine. On répartit les occurrences de la semaine en
 * donnant à chaque fois la tâche la plus lourde restante à la personne la
 * moins chargée qui est disponible ce jour-là (algorithme LPT, « plus longue
 * tâche d'abord », avec une file de priorité). On tient compte des absences
 * et de préférences (une personne qui déteste une tâche la reçoit en dernier
 * recours). On affiche le planning et la charge de chacun.
 * On apprend : l'équilibrage de charge, l'algorithme LPT, PriorityQueue et
 * la prise en compte de contraintes simples.
 */
import java.util.*;

public class Main {
    record Tache(String nom, int poids, int parSemaine) {}
    record Personne(String nom, Set<Integer> absences, Set<String> deteste) {}
    static final String[] JOURS = {"Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"};

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        List<Tache> taches = List.of(new Tache("Vaisselle", 3, 7), new Tache("Balayer", 2, 4), new Tache("Puiser l'eau", 4, 3),
                new Tache("Courses au marché", 5, 2), new Tache("Linge", 6, 1), new Tache("Poubelles", 1, 3));
        List<Personne> coloc = List.of(new Personne("Ange", Set.of(2), Set.of("Linge")), new Personne("Kofi", Set.of(5, 6), Set.of()),
                new Personne("Awa", Set.of(), Set.of("Puiser l'eau")), new Personne("Serge", Set.of(0, 1), Set.of()));
        Map<String, Integer> charge = new LinkedHashMap<>();
        coloc.forEach(p -> charge.put(p.nom(), 0));
        List<String[]> occurrences = new ArrayList<>();      // {tâche, jour}
        for (Tache t : taches)
            for (int k = 0; k < t.parSemaine(); k++) occurrences.add(new String[]{t.nom(), String.valueOf(k * 7 / t.parSemaine())});
        Map<String, Integer> poids = new HashMap<>();
        taches.forEach(t -> poids.put(t.nom(), t.poids()));
        occurrences.sort(Comparator.comparingInt((String[] o) -> -poids.get(o[0])));   // plus lourdes d'abord (LPT)
        String[][] planning = new String[7][];
        Map<Integer, List<String>> parJour = new TreeMap<>();
        for (String[] o : occurrences) {
            int jour = Integer.parseInt(o[1]);
            PriorityQueue<Personne> file = new PriorityQueue<>(Comparator
                    .comparing((Personne p) -> p.deteste().contains(o[0]))       // ceux qui détestent en dernier
                    .thenComparingInt(p -> charge.get(p.nom())));
            coloc.stream().filter(p -> !p.absences().contains(jour)).forEach(file::add);
            Personne choisi = file.poll();
            if (choisi == null) continue;
            charge.merge(choisi.nom(), poids.get(o[0]), Integer::sum);
            parJour.computeIfAbsent(jour, j -> new ArrayList<>()).add(o[0] + " (" + choisi.nom() + ")");
        }
        System.out.println("Planning de la semaine :");
        parJour.forEach((j, l) -> System.out.printf("   %s : %s%n", JOURS[j], String.join(", ", l)));
        int total = charge.values().stream().mapToInt(Integer::intValue).sum();
        System.out.println("\nCharge de chacun (points de pénibilité) :");
        charge.forEach((n, c) -> System.out.printf("   %-6s %3d  %s%n", n, c, "▇".repeat(c / 2)));
        int min = Collections.min(charge.values()), max = Collections.max(charge.values());
        System.out.printf("Écart entre le plus et le moins chargé : %d points sur %d au total (%.0f %%)%n", max - min, total, 100.0 * (max - min) / total);
        if (planning.length != 7) throw new IllegalStateException();
    }
}
