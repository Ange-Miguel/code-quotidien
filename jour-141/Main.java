/*
 * Jour 141 — Détection de mouvement simulée et règles d'alarme
 *
 * Problème : un système d'alarme maison avec détecteurs de mouvement (PIR) et
 * contacts de porte doit sonner pour un vrai intrus, mais pas pour le chat,
 * une porte qui claque au vent ou le propriétaire qui rentre et désarme.
 * Trop de fausses alarmes, et plus personne n'y prête attention.
 * Solution : une machine à états d'alarme (DÉSARMÉE, ARMÉE, TEMPORISATION
 * D'ENTRÉE, ALARME) et des règles de confirmation : la porte d'entrée lance un
 * délai de 30 s pour taper le code ; un détecteur intérieur seul ne déclenche
 * que s'il est confirmé par un second détecteur ou un deuxième passage dans
 * les 60 s (filtre anti-animal) ; un code faux trois fois déclenche
 * l'alarme. On rejoue trois scénarios : retour du propriétaire, chat, intrus.
 * On apprend : les machines à états avec temporisations, la corrélation
 * d'événements dans une fenêtre de temps et la réduction des fausses alarmes.
 */
import java.util.*;

public class Main {
    enum Etat { DESARMEE, ARMEE, TEMPORISATION, ALARME }
    record Evenement(int seconde, String source, String detail) {}

    static final String CODE = "2580";

    static List<String> rejouer(List<Evenement> evenements) {
        List<String> journal = new ArrayList<>();
        Etat etat = Etat.ARMEE;
        int finTempo = -1, essais = 0;
        Map<String, Integer> derniersPassages = new HashMap<>();
        for (Evenement e : evenements) {
            if (etat == Etat.TEMPORISATION && e.seconde() > finTempo) {
                etat = Etat.ALARME;
                journal.add(String.format("t=%3ds  délai d'entrée écoulé sans code → ALARME", finTempo));
            }
            String avant = etat.name();
            switch (e.source()) {
                case "porte" -> { if (etat == Etat.ARMEE) { etat = Etat.TEMPORISATION; finTempo = e.seconde() + 30; } }
                case "clavier" -> {
                    if (e.detail().equals(CODE)) { etat = Etat.DESARMEE; essais = 0; }
                    else if (++essais >= 3 && etat != Etat.DESARMEE) etat = Etat.ALARME;
                }
                default -> {   // détecteur de mouvement
                    if (etat == Etat.ARMEE) {
                        boolean confirme = derniersPassages.entrySet().stream()
                                .anyMatch(p -> e.seconde() - p.getValue() <= 60 && (!p.getKey().equals(e.source()) || e.seconde() - p.getValue() >= 5));
                        derniersPassages.put(e.source(), e.seconde());
                        if (confirme) etat = Etat.ALARME;
                    }
                }
            }
            String detail = e.source().equals("clavier") ? "code " + (e.detail().equals(CODE) ? "correct" : "faux") : e.detail();
            journal.add(String.format("t=%3ds  %-16s %-26s %s%s", e.seconde(), e.source(), detail,
                    avant.equals(etat.name()) ? "" : avant + " → ", etat == Etat.ALARME && !avant.equals("ALARME") ? "ALARME 🔔" : etat.name()));
        }
        return journal;
    }

    public static void main(String[] args) {
        if (args.length > 0) { System.err.println("Ce programme est une démonstration : lancez-le sans argument."); System.exit(1); }
        Map<String, List<Evenement>> scenarios = new LinkedHashMap<>();
        scenarios.put("Retour du propriétaire", List.of(new Evenement(0, "porte", "ouverture"),
                new Evenement(4, "pir-couloir", "mouvement"), new Evenement(12, "clavier", "1234"), new Evenement(18, "clavier", "2580")));
        scenarios.put("Le chat passe deux fois très vite", List.of(new Evenement(100, "pir-salon", "mouvement"),
                new Evenement(102, "pir-salon", "mouvement"), new Evenement(400, "pir-salon", "mouvement")));
        scenarios.put("Intrus par la fenêtre", List.of(new Evenement(200, "pir-salon", "mouvement"),
                new Evenement(214, "pir-couloir", "mouvement")));
        scenarios.put("Porte forcée, pas de code", List.of(new Evenement(0, "porte", "ouverture"),
                new Evenement(20, "pir-couloir", "mouvement"), new Evenement(45, "pir-salon", "mouvement")));
        scenarios.forEach((nom, evts) -> {
            System.out.println(nom + " (alarme armée) :");
            rejouer(evts).forEach(l -> System.out.println("   " + l));
            System.out.println();
        });
    }
}
