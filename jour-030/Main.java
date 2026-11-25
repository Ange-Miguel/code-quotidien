/*
 * Jour 030 — Chronomètre avec tours intermédiaires
 *
 * Problème : pour un entraînement sportif, un test de vitesse ou la mesure du
 * temps d'exécution d'un programme, on veut un chronomètre qui enregistre des
 * tours et donne le meilleur, le pire et la moyenne.
 * Solution : une classe Chronometre basée sur System.nanoTime (horloge
 * monotone, insensible aux changements d'heure). L'horloge est injectable via
 * une interface LongSupplier : la démo utilise une horloge simulée pour un
 * résultat reproductible, le mode --reel mesure les appuis sur Entrée.
 * On apprend : l'injection de dépendance, les interfaces fonctionnelles, les
 * horloges monotones et les statistiques simples sur une liste.
 */
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;
import java.util.function.LongSupplier;

public class Main {
    static class Chronometre {
        private final LongSupplier horloge;
        private long dernier = -1;
        final List<Double> tours = new ArrayList<>();

        Chronometre(LongSupplier horloge) { this.horloge = horloge; }

        void demarrer() { dernier = horloge.getAsLong(); tours.clear(); }

        double tour() {
            if (dernier < 0) throw new IllegalStateException("chronomètre non démarré");
            long maintenant = horloge.getAsLong();
            double secondes = (maintenant - dernier) / 1e9;
            dernier = maintenant;
            tours.add(secondes);
            return secondes;
        }

