/*
 * Jour 069 — Calcul d'itinéraire le plus court entre villes (Dijkstra)
 *
 * Problème : quel est le trajet routier le plus court entre deux villes du
 * Bénin, en passant éventuellement par d'autres villes ? C'est ce que font
 * les GPS et les applications de livraison.
 * Solution : les villes sont les sommets d'un graphe pondéré, les routes ses
 * arêtes (distances approximatives, données d'exemple). L'algorithme de
 * Dijkstra explore les villes dans l'ordre de leur distance au départ, à
 * l'aide d'une file de priorité, et mémorise le prédécesseur de chaque ville
 * pour reconstruire le chemin.
 * On apprend : les graphes pondérés en liste d'adjacence, l'algorithme de
 * Dijkstra, PriorityQueue et la reconstruction d'un chemin.
 */
import java.util.*;

public class Main {
    static final Map<String, Map<String, Integer>> ROUTES = new TreeMap<>();

    static void route(String a, String b, int km) {
        ROUTES.computeIfAbsent(a, k -> new TreeMap<>()).put(b, km);
        ROUTES.computeIfAbsent(b, k -> new TreeMap<>()).put(a, km);
    }

    static {
        // Distances routières approximatives, pour l'exemple
        route("Cotonou", "Porto-Novo", 32);   route("Cotonou", "Abomey-Calavi", 18);
        route("Cotonou", "Ouidah", 42);       route("Ouidah", "Lokossa", 65);
        route("Abomey-Calavi", "Allada", 35); route("Allada", "Bohicon", 80);
        route("Lokossa", "Abomey", 85);       route("Bohicon", "Abomey", 9);
        route("Bohicon", "Dassa-Zoumé", 75);  route("Dassa-Zoumé", "Savè", 35);
        route("Dassa-Zoumé", "Parakou", 190); route("Savè", "Parakou", 160);
        route("Parakou", "Djougou", 135);     route("Parakou", "Kandi", 210);
        route("Djougou", "Natitingou", 80);   route("Porto-Novo", "Pobè", 70);
        route("Pobè", "Kétou", 40);           route("Kétou", "Savè", 115);
    }

    record Etape(String ville, int distance) {}

    static void itineraire(String depart, String arrivee) {
        if (!ROUTES.containsKey(depart) || !ROUTES.containsKey(arrivee))
            throw new IllegalArgumentException("ville inconnue. Villes : " + ROUTES.keySet());
        Map<String, Integer> dist = new HashMap<>();
        Map<String, String> precedent = new HashMap<>();
        PriorityQueue<Etape> file = new PriorityQueue<>(Comparator.comparingInt(Etape::distance));
        dist.put(depart, 0);
        file.add(new Etape(depart, 0));
        int explorees = 0;
        while (!file.isEmpty()) {
            Etape e = file.poll();
            if (e.distance() > dist.get(e.ville())) continue;   // entrée périmée
            explorees++;
            if (e.ville().equals(arrivee)) break;
            for (var voisin : ROUTES.get(e.ville()).entrySet()) {
                int d = e.distance() + voisin.getValue();
                if (d < dist.getOrDefault(voisin.getKey(), Integer.MAX_VALUE)) {
                    dist.put(voisin.getKey(), d);
                    precedent.put(voisin.getKey(), e.ville());
                    file.add(new Etape(voisin.getKey(), d));
                }
            }
        }
        LinkedList<String> chemin = new LinkedList<>();
        for (String v = arrivee; v != null; v = precedent.get(v)) chemin.addFirst(v);
        System.out.printf("%s → %s : %d km (%d villes explorées)%n   %s%n", depart, arrivee,
                dist.get(arrivee), explorees, String.join(" → ", chemin));
    }

    public static void main(String[] args) {
        try {
            if (args.length == 2) {
                itineraire(args[0], args[1]);
            } else if (args.length == 0) {
                itineraire("Cotonou", "Parakou");
                itineraire("Abomey-Calavi", "Natitingou");
                itineraire("Porto-Novo", "Abomey");
                itineraire("Ouidah", "Kandi");
            } else {
                throw new IllegalArgumentException("deux villes attendues");
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.exit(1);
        }
    }
}
