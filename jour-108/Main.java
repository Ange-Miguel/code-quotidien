/*
 * Jour 108 — Machine à états d'un feu tricolore
 *
 * Problème : un carrefour avec feux pour voitures et bouton d'appel piéton.
 * Les règles de sécurité sont strictes : jamais de vert en même temps pour
 * les voitures et les piétons, durées minimales, et un mode « orange
 * clignotant » en cas de panne. C'est un exemple typique de programme
 * embarqué.
 * Solution : une machine à états avec une énumération d'états, chacun portant
 * sa durée et l'état des lampes. Une boucle avance d'une seconde à la fois ;
 * les événements (bouton piéton, panne, réparation) sont traités par une
 * méthode de transition. Une assertion vérifie à chaque seconde l'invariant
 * de sécurité.
 * On apprend : les enums riches (avec attributs et méthodes), la machine à
 * états temporisée, les invariants de sécurité et la simulation pas à pas.
 */
import java.util.Map;

public class Main {
    enum Etat {
        VERT_VOITURES(20, "VERT", "rouge"), ORANGE(3, "ORANGE", "rouge"), ROUGE_SECURITE(2, "rouge", "rouge"),
        VERT_PIETONS(10, "rouge", "VERT"), PIETONS_FIN(3, "rouge", "vert clignotant"),
        CLIGNOTANT(0, "orange clignotant", "éteint");

        final int duree; final String voitures, pietons;
        Etat(int duree, String voitures, String pietons) { this.duree = duree; this.voitures = voitures; this.pietons = pietons; }
    }

    Etat etat = Etat.VERT_VOITURES;
    int ecoule = 0;
    boolean appelPieton = false;

    void evenement(String e) {
        switch (e) {
            case "bouton" -> appelPieton = true;
            case "panne" -> changer(Etat.CLIGNOTANT);
            case "reparation" -> { if (etat == Etat.CLIGNOTANT) changer(Etat.ROUGE_SECURITE); }
            default -> throw new IllegalArgumentException("événement inconnu : " + e);
        }
    }

    void changer(Etat nouveau) {
        etat = nouveau;
        ecoule = 0;
    }

    void seconde() {
        ecoule++;
        switch (etat) {
            case VERT_VOITURES -> { if (ecoule >= etat.duree && appelPieton) changer(Etat.ORANGE); }   // vert minimal 20 s
            case ORANGE -> { if (ecoule >= etat.duree) changer(Etat.ROUGE_SECURITE); }
            case ROUGE_SECURITE -> { if (ecoule >= etat.duree) changer(appelPieton ? Etat.VERT_PIETONS : Etat.VERT_VOITURES); }
            case VERT_PIETONS -> { appelPieton = false; if (ecoule >= etat.duree) changer(Etat.PIETONS_FIN); }
            case PIETONS_FIN -> { if (ecoule >= etat.duree) changer(Etat.VERT_VOITURES); }
            case CLIGNOTANT -> { }
        }
        if (etat.voitures.equals("VERT") && etat.pietons.contains("ert"))   // invariant de sécurité
            throw new IllegalStateException("DANGER : vert voitures et piétons simultanés");
    }

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        Main feu = new Main();
        Map<Integer, String> scenario = Map.of(5, "bouton", 50, "bouton", 58, "bouton", 75, "panne", 85, "reparation");
        Etat precedent = null;
        for (int t = 0; t <= 100; t++) {
            if (scenario.containsKey(t)) {
                System.out.printf("t=%3ds  événement : %s%n", t, scenario.get(t));
                feu.evenement(scenario.get(t));
            }
            if (feu.etat != precedent) {
                System.out.printf("t=%3ds  %-15s voitures : %-18s piétons : %s%n", t, feu.etat, feu.etat.voitures, feu.etat.pietons);
                precedent = feu.etat;
            }
            feu.seconde();
        }
        System.out.println("Simulation terminée : invariant de sécurité respecté à chaque seconde.");
    }
}
