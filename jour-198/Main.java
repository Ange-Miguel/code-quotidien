/*
 * Jour 198 — Simulation d'une file de supermarché (caisses optimales)
 *
 * Problème : un supermarché veut savoir combien de caisses ouvrir selon
 * l'heure : trop peu, les clients attendent et partent ; trop, on paie des
 * caissiers inoccupés. Faut-il aussi une file unique (comme à la banque) ou
 * une file par caisse ?
 * Solution : simulation à événements discrets. Les arrivées suivent un
 * processus de Poisson dont le débit varie au cours de la journée (pics à
 * midi et en fin de journée) ; le temps de passage en caisse dépend du nombre
 * d'articles. On compare, pour 2 à 6 caisses : temps d'attente moyen et 95e
 * centile, taux d'occupation des caissiers et coût total (salaires + coût
 * estimé de l'attente des clients), puis file unique contre files séparées.
 * On apprend : la simulation à événements discrets avec une file de
 * priorité d'événements, les lois exponentielles, les centiles et
 * l'optimisation d'un coût.
 */
import java.util.*;

public class Main {
    record Resultat(double attenteMoy, double attente95, double occupation, double cout) {}

    static double exponentielle(Random r, double moyenne) { return -moyenne * Math.log(1 - r.nextDouble()); }

    static double debitParMinute(double minute) {          // ouverture 8 h → 20 h
        double h = 8 + minute / 60;
        return 0.6 + 1.4 * Math.exp(-Math.pow(h - 12.5, 2) / 1.5) + 1.8 * Math.exp(-Math.pow(h - 18, 2) / 1.2);
    }

    static Resultat simuler(int caisses, boolean fileUnique, long graine) {
        Random r = new Random(graine);
        List<Double> arrivees = new ArrayList<>(), services = new ArrayList<>();
        double t = 0;
        while (true) {
            t += exponentielle(r, 1 / 2.4);                                   // débit max, puis amincissement
            if (t > 720) break;
            if (r.nextDouble() < debitParMinute(t) / 2.4) {
                arrivees.add(t);
                int articles = 1 + (int) exponentielle(r, 12);
                services.add(0.5 + articles * 0.12);                          // minutes
            }
        }
        double[] libre = new double[caisses];
        List<Double> attentes = new ArrayList<>();
        double occupe = 0;
        for (int i = 0; i < arrivees.size(); i++) {
            double a = arrivees.get(i), s = services.get(i);
            int c;
            if (fileUnique) {                                                // la première caisse libre
                c = 0;
                for (int k = 1; k < caisses; k++) if (libre[k] < libre[c]) c = k;
            } else c = r.nextInt(caisses);                                   // chaque client choisit une file au hasard
            double debut = Math.max(a, libre[c]);
            attentes.add(debut - a);
            libre[c] = debut + s;
            occupe += s;
        }
        Collections.sort(attentes);
        double moy = attentes.stream().mapToDouble(Double::doubleValue).average().orElse(0);
        double p95 = attentes.get((int) (attentes.size() * 0.95));
        double cout = caisses * 12 * 1_500 + attentes.stream().mapToDouble(Double::doubleValue).sum() * 50;   // FCFA, exemple
        double fin = Math.max(720, Arrays.stream(libre).max().orElse(720));
        return new Resultat(moy, p95, occupe / (caisses * fin), cout);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        long graine = 198;
        if (args.length == 1) {
            try { graine = Long.parseLong(args[0]); } catch (NumberFormatException e) { System.err.println("Usage : java Main [graine]"); System.exit(1); }
        } else if (args.length > 1) { System.err.println("Usage : java Main [graine]"); System.exit(1); }
        System.out.println("Journée de 12 h, ~900 clients, pics à 12 h 30 et 18 h (file unique)\n");
        System.out.printf("%7s %13s %13s %11s %16s%n", "Caisses", "Attente moy.", "95 % < (min)", "Occupation", "Coût total");
        int meilleur = 0;
        double coutMin = Double.MAX_VALUE;
        for (int c = 2; c <= 8; c++) {
            Resultat r = simuler(c, true, graine);
            System.out.printf("%7d %10.1f min %13.1f %10.0f%% %,13.0f FCFA%n", c, r.attenteMoy(), r.attente95(), r.occupation() * 100, r.cout());
            if (r.cout() < coutMin) { coutMin = r.cout(); meilleur = c; }
        }
        System.out.printf("%nNombre de caisses le plus économique (salaires + attente) : %d%n%n", meilleur);
        Resultat unique = simuler(meilleur, true, graine), separees = simuler(meilleur, false, graine);
        System.out.printf("Avec %d caisses : file unique %.1f min d'attente moyenne (95 %% < %.1f min)%n", meilleur, unique.attenteMoy(), unique.attente95());
        System.out.printf("                 files séparées %.1f min d'attente moyenne (95 %% < %.1f min)%n", separees.attenteMoy(), separees.attente95());
        System.out.println("La file unique évite qu'un client attende derrière un gros chariot alors qu'une caisse est libre.");
    }
}
