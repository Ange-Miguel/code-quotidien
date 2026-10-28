/*
 * Jour 072 — Tri topologique : ordre des cours avec prérequis
 *
 * Problème : dans un cursus, certains cours exigent d'en avoir suivi d'autres
 * (pas de microcontrôleurs sans électronique numérique ni programmation C).
 * Dans quel ordre suivre les cours, et lesquels peut-on suivre en parallèle
 * le même semestre ?
 * Solution : algorithme de Kahn. On compte pour chaque cours le nombre de
 * prérequis non encore suivis (degré entrant). Les cours à 0 forment le
 * premier semestre ; on les « retire », ce qui diminue le degré de leurs
 * successeurs, et on recommence. S'il reste des cours, il y a un cycle.
 * On apprend : le tri topologique, les degrés entrants, le traitement par
 * niveaux (semestres) et la détection de dépendances impossibles.
 */
import java.util.*;

public class Main {
    static List<List<String>> planifier(Map<String, List<String>> prerequis) {
        Map<String, Integer> degre = new TreeMap<>();
        Map<String, List<String>> debloque = new HashMap<>();
        prerequis.forEach((cours, avant) -> {
            degre.merge(cours, avant.size(), Integer::sum);
            for (String p : avant) {
                degre.putIfAbsent(p, 0);
                debloque.computeIfAbsent(p, k -> new ArrayList<>()).add(cours);
            }
        });
        List<List<String>> semestres = new ArrayList<>();
        List<String> courant = degre.entrySet().stream().filter(e -> e.getValue() == 0).map(Map.Entry::getKey).toList();
        int places = 0;
        while (!courant.isEmpty()) {
            semestres.add(courant);
            places += courant.size();
            TreeSet<String> suivant = new TreeSet<>();
            for (String c : courant)
                for (String d : debloque.getOrDefault(c, List.of()))
                    if (degre.merge(d, -1, Integer::sum) == 0) suivant.add(d);
            courant = new ArrayList<>(suivant);
        }
        if (places < degre.size()) {
            List<String> bloques = degre.entrySet().stream().filter(e -> e.getValue() > 0).map(Map.Entry::getKey).toList();
            throw new IllegalStateException("prérequis circulaires entre : " + bloques);
        }
        return semestres;
    }

    public static void main(String[] args) {
        Map<String, List<String>> cursus = new LinkedHashMap<>();
        cursus.put("Algorithmique", List.of());
        cursus.put("Électricité", List.of());
        cursus.put("Mathématiques", List.of());
        cursus.put("Programmation C", List.of("Algorithmique"));
        cursus.put("Électronique numérique", List.of("Électricité", "Mathématiques"));
        cursus.put("Architecture des ordinateurs", List.of("Électronique numérique"));
        cursus.put("Microcontrôleurs", List.of("Programmation C", "Architecture des ordinateurs"));
        cursus.put("Réseaux", List.of("Algorithmique"));
        cursus.put("Systèmes temps réel", List.of("Microcontrôleurs"));
        cursus.put("Projet IoT", List.of("Systèmes temps réel", "Réseaux"));
        if (args.length > 0) {
            // Ajout d'une dépendance : "Cours>Prérequis"
            for (String a : args) {
                String[] p = a.split(">");
                if (p.length != 2) { System.err.println("Usage : java Main \"Cours>Prérequis\" ..."); System.exit(1); }
                cursus.computeIfAbsent(p[0], k -> new ArrayList<>());
                List<String> l = new ArrayList<>(cursus.get(p[0]));
                l.add(p[1]);
                cursus.put(p[0], l);
            }
        }
        try {
            List<List<String>> semestres = planifier(cursus);
            for (int i = 0; i < semestres.size(); i++)
                System.out.printf("Semestre %d : %s%n", i + 1, String.join(", ", semestres.get(i)));
        } catch (IllegalStateException e) {
            System.out.println("Impossible : " + e.getMessage());
            if (args.length > 0) System.exit(2);
        }
        if (args.length == 0) {
            System.out.println("\nAvec une erreur dans la maquette (Algorithmique exige le Projet IoT) :");
            cursus.put("Algorithmique", List.of("Projet IoT"));
            try { planifier(cursus); } catch (IllegalStateException e) { System.out.println("Impossible : " + e.getMessage()); }
        }
    }
}
