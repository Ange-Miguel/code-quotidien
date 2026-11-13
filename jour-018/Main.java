/*
 * Jour 018 — Calculateur d'âge exact (années, mois, jours)
 *
 * Problème : pour un formulaire, un concours ou un dossier administratif, il
 * faut connaître son âge exact à une date donnée, et le nombre de jours avant
 * le prochain anniversaire.
 * Solution : on utilise java.time : Period.between donne l'écart en années,
 * mois et jours ; ChronoUnit.DAYS donne le total de jours vécus. Le cas du
 * 29 février est géré automatiquement par la bibliothèque.
 * On apprend : l'API java.time (LocalDate, Period, ChronoUnit), le parsing de
 * dates au format ISO et la gestion des dates impossibles.
 */
import java.time.LocalDate;
import java.time.Period;
import java.time.format.DateTimeParseException;
import java.time.temporal.ChronoUnit;
import java.util.Locale;

public class Main {
    static void analyser(LocalDate naissance, LocalDate reference) {
        if (naissance.isAfter(reference)) throw new IllegalArgumentException("date de naissance dans le futur");
        Period age = Period.between(naissance, reference);
        long jours = ChronoUnit.DAYS.between(naissance, reference);

        LocalDate prochain = naissance.withYear(reference.getYear());
        if (!prochain.isAfter(reference)) prochain = naissance.withYear(reference.getYear() + 1);
        long attente = ChronoUnit.DAYS.between(reference, prochain);

        System.out.printf("Né(e) le %s, au %s :%n", naissance, reference);
        System.out.printf("  Âge : %d an(s), %d mois, %d jour(s)%n", age.getYears(), age.getMonths(), age.getDays());
        System.out.printf("  Jours vécus : %,d%n", jours);
        System.out.printf("  Prochain anniversaire : %s (dans %d jour(s))%n%n", prochain, attente);
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        try {
            if (args.length == 0) {
                LocalDate ref = LocalDate.of(2026, 10, 28);
                analyser(LocalDate.of(2005, 3, 14), ref);
                analyser(LocalDate.of(2004, 2, 29), ref);
                analyser(LocalDate.of(1960, 8, 1), ref);
            } else {
                LocalDate ref = args.length > 1 ? LocalDate.parse(args[1]) : LocalDate.now();
                analyser(LocalDate.parse(args[0]), ref);
            }
        } catch (DateTimeParseException | IllegalArgumentException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.err.println("Usage : java Main <naissance AAAA-MM-JJ> [date de référence]");
            System.exit(1);
        }
    }
}
