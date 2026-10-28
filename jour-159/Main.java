/*
 * Jour 159 — Mise en relation mentor / mentoré par compétences
 *
 * Problème : une association ou une plateforme de mentorat a des mentors
 * (professionnels expérimentés) et des mentorés (étudiants, jeunes
 * entrepreneurs). Former les bons binômes à la main, en tenant compte des
 * compétences recherchées, des disponibilités et de la capacité de chaque
 * mentor, devient vite impossible.
 * Solution : chaque personne a un ensemble de compétences et de créneaux. Le
 * score d'un binôme combine la similarité de Jaccard entre les compétences
 * recherchées et offertes (|A ∩ B| / |A ∪ B|), le nombre de créneaux communs
 * et un bonus si la ville est la même. On affecte ensuite les mentorés par
 * ordre de « difficulté » (ceux qui ont le moins d'options d'abord), en
 * respectant la capacité de chaque mentor. Données fictives.
 * On apprend : la similarité de Jaccard, les opérations d'ensembles (retainAll,
 * addAll), un score composite et l'affectation sous contrainte de capacité.
 */
import java.util.*;
import java.util.stream.Collectors;

public class Main {
    record Mentor(String nom, Set<String> competences, Set<String> creneaux, String ville, int capacite) {}
    record Mentore(String nom, Set<String> besoins, Set<String> creneaux, String ville) {}

    static double jaccard(Set<String> a, Set<String> b) {
        Set<String> inter = new HashSet<>(a); inter.retainAll(b);
        Set<String> union = new HashSet<>(a); union.addAll(b);
        return union.isEmpty() ? 0 : (double) inter.size() / union.size();
    }

    static double score(Mentor m, Mentore e) {
        Set<String> communs = new HashSet<>(m.creneaux()); communs.retainAll(e.creneaux());
        if (communs.isEmpty()) return 0;                                  // aucun moment pour se voir
        double s = 0.7 * jaccard(m.competences(), e.besoins()) + 0.2 * Math.min(1, communs.size() / 2.0);
        return s + (m.ville().equals(e.ville()) ? 0.1 : 0);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        List<Mentor> mentors = List.of(
                new Mentor("Mme A. (développeuse)", Set.of("java", "web", "carrière", "freelance"), Set.of("sam-matin", "mer-soir"), "Cotonou", 2),
                new Mentor("M. B. (ingénieur IoT)", Set.of("c", "électronique", "iot", "startup"), Set.of("sam-matin", "jeu-soir"), "Calavi", 2),
                new Mentor("Mme C. (entrepreneure)", Set.of("startup", "financement", "marketing", "vente"), Set.of("mar-soir", "sam-aprem"), "Cotonou", 1));
        List<Mentore> mentores = List.of(
                new Mentore("Ange", Set.of("iot", "startup", "financement"), Set.of("sam-matin", "sam-aprem"), "Calavi"),
                new Mentore("Awa", Set.of("web", "freelance"), Set.of("mer-soir"), "Cotonou"),
                new Mentore("Kofi", Set.of("électronique", "c"), Set.of("jeu-soir", "sam-matin"), "Calavi"),
                new Mentore("Inès", Set.of("marketing", "vente"), Set.of("mar-soir"), "Porto-Novo"),
                new Mentore("Serge", Set.of("java", "carrière"), Set.of("dim-matin"), "Cotonou"));

        Map<Mentore, Long> options = mentores.stream().collect(Collectors.toMap(e -> e, e -> mentors.stream().filter(m -> score(m, e) > 0.15).count()));
        List<Mentore> ordre = new ArrayList<>(mentores);
        ordre.sort(Comparator.comparing(options::get));                     // les plus contraints d'abord
        Map<Mentor, Integer> places = new HashMap<>();
        mentors.forEach(m -> places.put(m, m.capacite()));
        for (Mentore e : ordre) {
            Optional<Mentor> meilleur = mentors.stream().filter(m -> places.get(m) > 0 && score(m, e) > 0.15)
                    .max(Comparator.comparingDouble(m -> score(m, e)));
            if (meilleur.isPresent()) {
                Mentor m = meilleur.get();
                places.merge(m, -1, Integer::sum);
                Set<String> communs = new TreeSet<>(m.competences()); communs.retainAll(e.besoins());
                System.out.printf("%-6s → %-24s score %.2f  (compétences communes : %s)%n", e.nom(), m.nom(), score(m, e), communs);
            } else {
                System.out.printf("%-6s → aucun mentor disponible (créneaux ou compétences incompatibles)%n", e.nom());
            }
        }
        System.out.println("\nPlaces restantes : " + places.entrySet().stream().map(x -> x.getKey().nom() + " " + x.getValue()).toList());
    }
}
