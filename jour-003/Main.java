/*
 * Jour 003 — Calculateur de moyenne semestrielle avec coefficients
 *
 * Problème : à l'IFRI comme ailleurs, la moyenne du semestre dépend des
 * coefficients des matières ; la calculer à la main est source d'erreurs.
 * Solution : chaque matière est un objet (nom, note, coefficient). La moyenne
 * pondérée vaut somme(note × coef) / somme(coef). On affiche aussi la mention
 * et les matières sous 10 à rattraper.
 * On apprend : les records Java, les listes, les boucles, la moyenne pondérée
 * et la lecture d'arguments au format nom:note:coef.
 */
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Matiere(String nom, double note, int coef) {}

    static double moyenne(List<Matiere> matieres) {
        double total = 0;
        int coefs = 0;
        for (Matiere m : matieres) {
            total += m.note() * m.coef();
            coefs += m.coef();
        }
        return coefs == 0 ? 0 : total / coefs;
    }

    static String mention(double moy) {
        if (moy >= 16) return "Très bien";
        if (moy >= 14) return "Bien";
        if (moy >= 12) return "Assez bien";
        if (moy >= 10) return "Passable";
        return "Ajourné";
    }

    static Matiere lire(String texte) {
        String[] parts = texte.split(":");
        if (parts.length != 3) throw new IllegalArgumentException("format attendu nom:note:coef → " + texte);
        double note = Double.parseDouble(parts[1]);
        int coef = Integer.parseInt(parts[2]);
        if (note < 0 || note > 20) throw new IllegalArgumentException("note hors de [0, 20] : " + note);
        if (coef <= 0) throw new IllegalArgumentException("coefficient invalide : " + coef);
        return new Matiere(parts[0], note, coef);
    }

    public static void main(String[] args) {
        java.util.Locale.setDefault(java.util.Locale.FRANCE);
        List<Matiere> matieres = new ArrayList<>();
        try {
            if (args.length == 0) {
                matieres.add(new Matiere("Électronique numérique", 13.5, 4));
                matieres.add(new Matiere("Programmation C", 15, 3));
                matieres.add(new Matiere("Mathématiques", 9, 3));
                matieres.add(new Matiere("Réseaux", 12, 2));
                matieres.add(new Matiere("Anglais", 14, 1));
            } else {
                for (String a : args) matieres.add(lire(a));
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Erreur : " + e.getMessage());
            System.err.println("Usage : java Main \"Maths:12.5:3\" \"Réseaux:14:2\" ...");
            System.exit(1);
        }

        System.out.printf("%-25s %6s %5s%n", "Matière", "Note", "Coef");
        for (Matiere m : matieres) System.out.printf("%-25s %6.2f %5d%n", m.nom(), m.note(), m.coef());
        double moy = moyenne(matieres);
        System.out.printf("%nMoyenne semestrielle : %.2f / 20 — %s%n", moy, mention(moy));
        for (Matiere m : matieres)
            if (m.note() < 10) System.out.println("À rattraper : " + m.nom());
    }
}
