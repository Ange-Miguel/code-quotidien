/*
 * Jour 096 — Planificateur d'emploi du temps sans conflits
 *
 * Problème : placer les cours d'une semaine dans des créneaux en respectant
 * les contraintes : un enseignant ne peut pas être à deux endroits à la fois,
 * une classe ne peut pas avoir deux cours en même temps, et certaines salles
 * (laboratoire d'électronique) sont uniques.
 * Solution : backtracking avec heuristique : on place d'abord les cours les
 * plus contraints (ceux qui demandent une ressource rare), on essaie chaque
 * créneau, on vérifie qu'aucune ressource (enseignant, classe, salle) n'est
 * déjà utilisée sur ce créneau, et on revient en arrière si on est bloqué.
 * On apprend : la modélisation de contraintes, les Set de ressources
 * occupées par créneau, le backtracking et l'ordre de traitement des
 * variables les plus contraintes.
 */
import java.util.*;

public class Main {
    record Cours(String nom, String enseignant, String classe, String salle) {}

    static final String[] JOURS = {"Lun", "Mar", "Mer", "Jeu", "Ven"};
    static final String[] HEURES = {"08h", "10h", "14h"};
    static final int CRENEAUX = JOURS.length * HEURES.length;

    final List<Cours> cours;
    final int[] placement;
    final List<Set<String>> occupe = new ArrayList<>();
    long essais = 0;

    Main(List<Cours> cours) {
        this.cours = cours;
        placement = new int[cours.size()];
        for (int i = 0; i < CRENEAUX; i++) occupe.add(new HashSet<>());
    }

    boolean placer(int i) {
        if (i == cours.size()) return true;
        Cours c = cours.get(i);
        List<String> res = List.of("E:" + c.enseignant(), "C:" + c.classe(), "S:" + c.salle());
        for (int k = 0; k < CRENEAUX; k++) {
            int t = (i * 4 + k) % CRENEAUX;          // départ décalé : répartit les cours sur la semaine
            essais++;
            if (res.stream().anyMatch(occupe.get(t)::contains)) continue;
            occupe.get(t).addAll(res);
            placement[i] = t;
            if (placer(i + 1)) return true;
            occupe.get(t).removeAll(res);            // retour arrière
        }
        return false;
    }

    void afficher(String classe) {
        System.out.println("\nEmploi du temps " + classe);
        System.out.println(String.format("      %-18s%-18s%-18s%-18s%-18s", (Object[]) JOURS).stripTrailing());
        for (int h = 0; h < HEURES.length; h++) {
            StringBuilder sb = new StringBuilder(String.format("%-6s", HEURES[h]));
            for (int j = 0; j < JOURS.length; j++) {
                String txt = "—";
                for (int i = 0; i < cours.size(); i++)
                    if (placement[i] == j * HEURES.length + h && cours.get(i).classe().equals(classe)) txt = cours.get(i).nom();
                sb.append(String.format("%-18.17s", txt));
            }
            System.out.println(sb.toString().stripTrailing());
        }
    }

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        List<Cours> liste = new ArrayList<>();
        String[][] programme = {
                {"Électronique TP", "M. Houngbo", "SE2", "Labo"}, {"Électronique TP", "M. Houngbo", "SE2", "Labo"},
                {"Électronique TP", "M. Houngbo", "GL2", "Labo"}, {"Prog. C", "Mme Dossou", "SE2", "A1"},
                {"Prog. C", "Mme Dossou", "GL2", "A1"}, {"Prog. C", "Mme Dossou", "GL2", "A2"},
                {"Maths", "M. Zinsou", "SE2", "A2"}, {"Maths", "M. Zinsou", "GL2", "A2"},
                {"Maths", "M. Zinsou", "SE2", "A1"}, {"Réseaux", "M. Kpade", "SE2", "Labo"},
                {"Réseaux", "M. Kpade", "GL2", "A1"}, {"Anglais", "Mme Agossa", "SE2", "A2"},
                {"Anglais", "Mme Agossa", "GL2", "A2"}, {"Microcontrôleurs", "M. Houngbo", "SE2", "Labo"}};
        for (String[] p : programme) liste.add(new Cours(p[0], p[1], p[2], p[3]));
        // Heuristique : les cours en labo (ressource rare) d'abord
        liste.sort(Comparator.comparing((Cours c) -> !c.salle().equals("Labo")));
        Main planif = new Main(liste);
        if (!planif.placer(0)) { System.out.println("Aucun emploi du temps possible."); return; }
        System.out.printf("%d cours placés sans conflit (%d essais)%n", liste.size(), planif.essais);
        planif.afficher("SE2");
        planif.afficher("GL2");
    }
}
