/*
 * Jour 090 — Générateur de factures en texte formaté
 *
 * Problème : un freelance (graphisme, motion design, développement) ou une
 * petite boutique doit envoyer des factures propres, numérotées, avec les
 * totaux justes, sans payer un logiciel.
 * Solution : une facture est composée de lignes (désignation, quantité, prix
 * unitaire, remise). Les montants sont en entiers (FCFA) pour éviter les
 * erreurs d'arrondi des nombres à virgule. On calcule sous-total, remise,
 * TVA optionnelle et total, on écrit le montant en lettres simplifié et on
 * produit un document texte aligné, prêt à imprimer ou à convertir en PDF.
 * On apprend : les records, StringBuilder et String.format pour la mise en
 * page, les calculs monétaires en entiers et la numérotation de documents.
 */
import java.time.LocalDate;
import java.util.List;
import java.util.Locale;

public class Main {
    record Ligne(String designation, int quantite, long prixUnitaire, int remisePct) {
        long montant() { return quantite * prixUnitaire * (100 - remisePct) / 100; }
    }

    record Facture(String numero, LocalDate date, String client, List<Ligne> lignes, boolean tva) {}

    static String rendre(Facture f) {
        StringBuilder sb = new StringBuilder();
        String trait = "-".repeat(78) + "\n";
        sb.append("AKPANON Ange-Miguel — Création graphique & développement\n");
        sb.append("Abomey-Calavi, Bénin\n\n");
        sb.append(String.format("FACTURE N° %s%55s%n", f.numero(), "Date : " + f.date()));
        sb.append("Client : ").append(f.client()).append("\n\n").append(trait);
        sb.append(String.format("%-38s %5s %12s %6s %13s%n", "Désignation", "Qté", "P.U.", "Rem.", "Montant"));
        sb.append(trait);
        long sousTotal = 0;
        for (Ligne l : f.lignes()) {
            if (l.quantite() <= 0 || l.prixUnitaire() < 0 || l.remisePct() < 0 || l.remisePct() > 100)
                throw new IllegalArgumentException("ligne invalide : " + l.designation());
            sousTotal += l.montant();
            sb.append(String.format("%-38.38s %5d %,12d %5s%% %,13d%n", l.designation(), l.quantite(), l.prixUnitaire(),
                    l.remisePct(), l.montant()));
        }
        sb.append(trait);
        long tva = f.tva() ? Math.round(sousTotal * 0.18) : 0;
        sb.append(String.format("%64s %,13d%n", "Total hors taxe :", sousTotal));
        if (f.tva()) sb.append(String.format("%64s %,13d%n", "TVA 18 % :", tva));
        sb.append(String.format("%64s %,13d FCFA%n", "NET À PAYER :", sousTotal + tva));
        sb.append(trait).append("Paiement : Mobile Money ou virement, à 15 jours. Merci pour votre confiance !\n");
        return sb.toString();
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) {
            System.err.println("Ce programme est une démonstration : lancez-le sans argument.");
            System.exit(1);
        }
        Facture f = new Facture("2026-0042", LocalDate.of(2026, 10, 28), "Association exemple (Cotonou)", List.of(
                new Ligne("Animation motion design 30 s", 1, 150_000, 0),
                new Ligne("Déclinaisons formats réseaux sociaux", 3, 15_000, 10),
                new Ligne("Affiche A3 événement", 2, 25_000, 0),
                new Ligne("Retouches supplémentaires (heure)", 4, 5_000, 50)), false);
        System.out.print(rendre(f));
    }
}
