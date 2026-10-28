/*
 * Jour 180 — Recommandation de produits « achetés ensemble »
 *
 * Problème : une boutique veut suggérer au client « souvent acheté avec ce
 * produit » (une coque avec un téléphone, un câble avec un chargeur), pour
 * augmenter le panier moyen. Mais comment savoir quels produits vont
 * vraiment ensemble, et pas seulement ceux qui se vendent beaucoup ?
 * Solution : l'analyse d'associations sur l'historique des paniers. Pour une
 * paire A → B : support = part des paniers qui contiennent A et B ;
 * confiance = parmi les paniers avec A, part qui contiennent aussi B ;
 * lift = confiance / part des paniers avec B. Un lift > 1 signifie que A
 * « attire » vraiment B ; un produit très populaire a une forte confiance
 * mais un lift proche de 1. On ne garde que les règles avec un support
 * minimal. Paniers fictifs générés avec des affinités connues.
 * On apprend : les mesures support / confiance / lift, le comptage de paires
 * avec des Map, et le piège des produits populaires.
 */
import java.util.*;

public class Main {
    static List<Set<String>> genererPaniers(int n, long graine) {
        Random r = new Random(graine);
        List<Set<String>> paniers = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            Set<String> p = new TreeSet<>();
            if (r.nextDouble() < 0.35) p.add("sachet d'eau");                    // très populaire, sans lien
            if (r.nextDouble() < 0.25) { p.add("téléphone"); if (r.nextDouble() < 0.7) p.add("coque"); if (r.nextDouble() < 0.5) p.add("verre trempé"); }
            if (r.nextDouble() < 0.30) { p.add("chargeur"); if (r.nextDouble() < 0.6) p.add("câble USB-C"); }
            if (r.nextDouble() < 0.15) { p.add("lampe solaire"); if (r.nextDouble() < 0.4) p.add("piles"); }
            if (r.nextDouble() < 0.10) p.add("écouteurs");
            if (p.isEmpty()) p.add("sachet d'eau");
            paniers.add(p);
        }
        return paniers;
    }

    record Regle(String a, String b, double support, double confiance, double lift) {}

    static List<Regle> regles(List<Set<String>> paniers, double supportMin) {
        Map<String, Integer> seul = new HashMap<>();
        Map<String, Integer> paire = new HashMap<>();
        for (Set<String> p : paniers) {
            List<String> l = new ArrayList<>(p);
            for (String x : l) seul.merge(x, 1, Integer::sum);
            for (int i = 0; i < l.size(); i++) for (int j = 0; j < l.size(); j++) if (i != j) paire.merge(l.get(i) + "→" + l.get(j), 1, Integer::sum);
        }
        double n = paniers.size();
        List<Regle> res = new ArrayList<>();
        paire.forEach((k, v) -> {
            String[] ab = k.split("→");
            double support = v / n, confiance = (double) v / seul.get(ab[0]), lift = confiance / (seul.get(ab[1]) / n);
            if (support >= supportMin) res.add(new Regle(ab[0], ab[1], support, confiance, lift));
        });
        res.sort(Comparator.comparingDouble(Regle::lift).reversed());
        return res;
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        List<Set<String>> paniers = genererPaniers(2000, 180);
        List<Regle> toutes = regles(paniers, 0.02);
        System.out.printf("%d paniers analysés, %d règles avec un support ≥ 2 %%%n%n", paniers.size(), toutes.size());
        System.out.printf("%-14s    %-15s %8s %10s %6s%n", "Si le client prend", "proposer", "support", "confiance", "lift");
        for (Regle r : toutes.subList(0, Math.min(8, toutes.size())))
            System.out.printf("%-17s → %-15s %7.1f%% %9.0f%% %6.2f%n", r.a(), r.b(), r.support() * 100, r.confiance() * 100, r.lift());
        String produit = args.length > 0 ? String.join(" ", args) : "téléphone";
        System.out.println("\n« Souvent acheté avec " + produit + " » :");
        List<Regle> pour = toutes.stream().filter(r -> r.a().equals(produit) && r.lift() > 1.2).limit(3).toList();
        if (pour.isEmpty()) System.out.println("   (pas assez de données)");
        pour.forEach(r -> System.out.printf("   %s (%.0f %% des acheteurs, lift %.1f)%n", r.b(), r.confiance() * 100, r.lift()));
        toutes.stream().filter(r -> r.a().equals("chargeur") && r.b().equals("sachet d'eau")).findFirst().ifPresent(r ->
                System.out.printf("%nPiège : chargeur → sachet d'eau a %.0f %% de confiance mais un lift de %.2f : simple popularité.%n", r.confiance() * 100, r.lift()));
    }
}
