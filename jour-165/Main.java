/*
 * Jour 165 — Gestion de devis client pour prestations graphiques
 *
 * Problème : un graphiste ou motion designer freelance doit chiffrer
 * rapidement des demandes variées (logo, affiche, vidéo animée…), sans
 * oublier les révisions, l'urgence ou les droits d'utilisation, et suivre ce
 * que devient chaque devis (envoyé, accepté, refusé, expiré).
 * Solution : un catalogue de prestations avec prix de base et options ; un
 * devis est construit à partir d'un brief (prestations, quantités, urgence,
 * révisions supplémentaires) et reçoit un numéro, une date de validité
 * (30 jours) et un acompte de 50 %. Un cycle de vie (enum avec transitions
 * autorisées) empêche par exemple d'accepter un devis expiré. On calcule le
 * taux de conversion des devis. Tarifs donnés à titre d'exemple.
 * On apprend : la modélisation d'un document commercial, une machine à états
 * avec transitions contrôlées et les statistiques de suivi commercial.
 */
import java.time.LocalDate;
import java.util.*;

public class Main {
    record Prestation(String nom, int prix) {}
    static final Map<String, Prestation> CATALOGUE = new LinkedHashMap<>();
    static {
        CATALOGUE.put("logo", new Prestation("Création de logo (3 pistes)", 75_000));
        CATALOGUE.put("charte", new Prestation("Charte graphique simple", 60_000));
        CATALOGUE.put("affiche", new Prestation("Affiche A3", 25_000));
        CATALOGUE.put("post", new Prestation("Visuel réseaux sociaux", 8_000));
        CATALOGUE.put("motion30", new Prestation("Animation motion design 30 s", 150_000));
    }

    enum Statut {
        BROUILLON, ENVOYE, ACCEPTE, REFUSE, EXPIRE;
        boolean peutAller(Statut s) {
            return switch (this) {
                case BROUILLON -> s == ENVOYE;
                case ENVOYE -> s == ACCEPTE || s == REFUSE || s == EXPIRE;
                default -> false;
            };
        }
    }

    static int compteur = 0;

    static final class Devis {
        final String numero, client; final LocalDate date; final Map<String, Integer> lignes; final boolean urgent; final int revisionsEnPlus;
        Statut statut = Statut.BROUILLON;
        Devis(String client, LocalDate date, Map<String, Integer> lignes, boolean urgent, int revisions) {
            this.numero = String.format("DEV-%d-%03d", date.getYear(), ++compteur);
            this.client = client; this.date = date; this.lignes = lignes; this.urgent = urgent; this.revisionsEnPlus = revisions;
        }
        int total() {
            int t = lignes.entrySet().stream().mapToInt(e -> CATALOGUE.get(e.getKey()).prix() * e.getValue()).sum();
            t += revisionsEnPlus * 10_000;
            return urgent ? t * 130 / 100 : t;
        }
        void passer(Statut s, LocalDate jour) {
            if (s == Statut.ACCEPTE && jour.isAfter(date.plusDays(30))) s = Statut.EXPIRE;
            if (!statut.peutAller(s)) throw new IllegalStateException(numero + " : " + statut + " → " + s + " interdit");
            statut = s;
        }
        void imprimer() {
            System.out.printf("%s — %s — %s (valable jusqu'au %s)%n", numero, client, date, date.plusDays(30));
            lignes.forEach((k, q) -> System.out.printf("   %-34s %d × %,8d%n", CATALOGUE.get(k).nom(), q, CATALOGUE.get(k).prix()));
            if (revisionsEnPlus > 0) System.out.printf("   %-34s %d × %,8d%n", "Révisions supplémentaires", revisionsEnPlus, 10_000);
            if (urgent) System.out.println("   Majoration urgence (délai < 72 h)       + 30 %");
            System.out.printf("   TOTAL %,d FCFA — acompte à la commande %,d FCFA%n%n", total(), total() / 2);
        }
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        LocalDate j = LocalDate.of(2026, 10, 1);
        List<Devis> devis = List.of(
                new Devis("Association exemple", j, new LinkedHashMap<>(Map.of("motion30", 1, "post", 3)), false, 1),
                new Devis("Boutique de pagnes", j.plusDays(3), new LinkedHashMap<>(Map.of("logo", 1, "charte", 1)), false, 0),
                new Devis("Restaurant maquis", j.plusDays(10), new LinkedHashMap<>(Map.of("affiche", 2, "post", 5)), true, 0),
                new Devis("Startup IoT", j.plusDays(12), new LinkedHashMap<>(Map.of("logo", 1)), false, 2));
        devis.get(0).imprimer();
        devis.forEach(d -> d.passer(Statut.ENVOYE, d.date));
        devis.get(0).passer(Statut.ACCEPTE, j.plusDays(5));
        devis.get(1).passer(Statut.REFUSE, j.plusDays(9));
        devis.get(2).passer(Statut.ACCEPTE, j.plusDays(11));
        devis.get(3).passer(Statut.ACCEPTE, j.plusDays(50));      // trop tard : devient EXPIRÉ
        try { devis.get(1).passer(Statut.ACCEPTE, j.plusDays(12)); }
        catch (IllegalStateException e) { System.out.println("Refusé par le système : " + e.getMessage() + "\n"); }
        int gagne = 0, envoye = 0;
        for (Devis d : devis) {
            System.out.printf("%s  %-20s %,9d FCFA  %s%n", d.numero, d.client, d.total(), d.statut);
            envoye += d.total();
            if (d.statut == Statut.ACCEPTE) gagne += d.total();
        }
        long acceptes = devis.stream().filter(d -> d.statut == Statut.ACCEPTE).count();
        System.out.printf("%nTaux de conversion : %d/%d devis, %,d sur %,d FCFA (%.0f %%)%n", acceptes, devis.size(), gagne, envoye, 100.0 * gagne / envoye);
    }
}
