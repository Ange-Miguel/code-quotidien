/*
 * Jour 105 — Filtre médian contre les valeurs aberrantes d'un capteur
 *
 * Problème : un capteur à ultrasons (HC-SR04) ou un capteur de distance renvoie
 * parfois une valeur complètement fausse (0 cm, 400 cm) à cause d'un écho
 * parasite. La moyenne glissante du jour 104 est « contaminée » par ces pics.
 * Solution : le filtre médian prend la valeur du milieu des N dernières
 * mesures triées : une valeur aberrante isolée ne peut jamais être choisie.
 * On l'implémente sur une fenêtre glissante de taille impaire, on le compare
 * à la moyenne sur un signal avec pics, et on ajoute un rejet par seuil
 * (écart à la médiane supérieur à k × MAD, l'écart absolu médian).
 * On apprend : la médiane, sa robustesse face aux valeurs aberrantes, la
 * fenêtre glissante avec ArrayDeque et une statistique robuste (MAD).
 */
import java.util.*;

public class Main {
    static class FiltreMedian {
        private final ArrayDeque<Double> fenetre = new ArrayDeque<>();
        private final int taille;
        FiltreMedian(int taille) {
            if (taille < 1 || taille % 2 == 0) throw new IllegalArgumentException("taille impaire attendue");
            this.taille = taille;
        }
        double filtrer(double x) {
            fenetre.addLast(x);
            if (fenetre.size() > taille) fenetre.removeFirst();
            return mediane(new ArrayList<>(fenetre));
        }
    }

    static double mediane(List<Double> l) {
        List<Double> t = new ArrayList<>(l);
        Collections.sort(t);
        int n = t.size();
        return n % 2 == 1 ? t.get(n / 2) : (t.get(n / 2 - 1) + t.get(n / 2)) / 2;
    }

    static double moyenneFenetre(Deque<Double> f) { return f.stream().mapToDouble(d -> d).average().orElse(0); }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        int taille = 5;
        if (args.length == 1) {
            try { taille = Integer.parseInt(args[0]); new FiltreMedian(taille); }
            catch (IllegalArgumentException e) { System.err.println("Usage : java Main [taille impaire]"); System.exit(1); }
        }
        Random r = new Random(105);
        double[] mesures = new double[30];
        for (int i = 0; i < mesures.length; i++) mesures[i] = 120 + 0.4 * i + r.nextGaussian() * 1.5;   // cuve qui se vide
        mesures[6] = 0; mesures[13] = 400; mesures[21] = 2; mesures[22] = 398;          // échos parasites

        FiltreMedian fm = new FiltreMedian(taille);
        ArrayDeque<Double> fenetreMoy = new ArrayDeque<>();
        System.out.printf("Distance (cm) — fenêtre de %d mesures%n  t    brute  moyenne  médiane%n", taille);
        double erreurMoy = 0, erreurMed = 0;
        for (int i = 0; i < mesures.length; i++) {
            fenetreMoy.addLast(mesures[i]);
            if (fenetreMoy.size() > taille) fenetreMoy.removeFirst();
            double moy = moyenneFenetre(fenetreMoy), med = fm.filtrer(mesures[i]), vrai = 120 + 0.4 * i;
            if (i >= taille) { erreurMoy += Math.abs(moy - vrai); erreurMed += Math.abs(med - vrai); }
            System.out.printf("%3d %8.1f %8.1f %8.1f%s%n", i, mesures[i], moy, med, Math.abs(mesures[i] - vrai) > 50 ? "  ← écho parasite" : "");
        }
        System.out.printf("%nErreur moyenne : moyenne glissante %.1f cm, médiane %.1f cm%n",
                erreurMoy / (mesures.length - taille), erreurMed / (mesures.length - taille));

        List<Double> valeurs = new ArrayList<>();
        for (double m : mesures) valeurs.add(m);
        double med = mediane(valeurs);
        List<Double> ecarts = valeurs.stream().map(v -> Math.abs(v - med)).toList();
        double mad = mediane(ecarts);
        long rejetees = valeurs.stream().filter(v -> Math.abs(v - med) > 5 * mad).count();
        System.out.printf("Rejet par seuil (écart > 5 × MAD = %.1f cm) : %d valeur(s) aberrante(s) détectée(s)%n", 5 * mad, rejetees);
    }
}
