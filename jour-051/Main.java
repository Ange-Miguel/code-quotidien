/*
 * Jour 051 — Carnet d'adresses enregistré dans un fichier
 *
 * Problème : garder les coordonnées de ses clients, fournisseurs et
 * camarades dans un fichier qu'on peut sauvegarder, lire et rechercher, au
 * lieu de les perdre avec un téléphone.
 * Solution : chaque contact est un record (nom, téléphone, e-mail, ville). Le
 * carnet est enregistré au format CSV avec échappement des guillemets et des
 * virgules, relu au démarrage, trié par nom et interrogeable par recherche
 * partielle insensible à la casse.
 * On apprend : la persistance en CSV, l'échappement de caractères, les
 * TreeMap pour garder un ordre trié et la gestion des fichiers temporaires.
 */
import java.io.IOException;
import java.nio.file.*;
import java.util.*;

public class Main {
    record Contact(String nom, String telephone, String email, String ville) {}

    private final TreeMap<String, Contact> contacts = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static String echapper(String champ) {
        if (champ.contains(",") || champ.contains("\"")) return "\"" + champ.replace("\"", "\"\"") + "\"";
        return champ;
    }

    static List<String> decouper(String ligne) {
        List<String> champs = new ArrayList<>();
        StringBuilder sb = new StringBuilder();
        boolean entreGuillemets = false;
        for (int i = 0; i < ligne.length(); i++) {
            char c = ligne.charAt(i);
            if (c == '"') {
                if (entreGuillemets && i + 1 < ligne.length() && ligne.charAt(i + 1) == '"') { sb.append('"'); i++; }
                else entreGuillemets = !entreGuillemets;
            } else if (c == ',' && !entreGuillemets) {
                champs.add(sb.toString()); sb.setLength(0);
            } else sb.append(c);
        }
        champs.add(sb.toString());
        return champs;
    }

    void ajouter(Contact c) {
        if (c.nom().isBlank()) throw new IllegalArgumentException("nom obligatoire");
        contacts.put(c.nom(), c);
    }

    void sauver(Path fichier) throws IOException {
        List<String> lignes = new ArrayList<>(List.of("nom,telephone,email,ville"));
        for (Contact c : contacts.values())
            lignes.add(String.join(",", echapper(c.nom()), echapper(c.telephone()), echapper(c.email()), echapper(c.ville())));
        Files.write(fichier, lignes);
    }

    void charger(Path fichier) throws IOException {
        List<String> lignes = Files.readAllLines(fichier);
        for (String l : lignes.subList(1, lignes.size())) {
            List<String> f = decouper(l);
            if (f.size() == 4) ajouter(new Contact(f.get(0), f.get(1), f.get(2), f.get(3)));
        }
    }

    List<Contact> rechercher(String motif) {
        String m = motif.toLowerCase(Locale.ROOT);
        return contacts.values().stream()
                .filter(c -> (c.nom() + " " + c.ville() + " " + c.email()).toLowerCase(Locale.ROOT).contains(m)).toList();
    }

    static void afficher(Collection<Contact> liste) {
        for (Contact c : liste) System.out.printf("  %-24s %-20s %-28s %s%n", c.nom(), c.telephone(), c.email(), c.ville());
    }

    public static void main(String[] args) throws IOException {
        Locale.setDefault(Locale.FRANCE);
        if (args.length > 0) {
            Path fichier = Path.of(args[0]);
            if (!Files.isRegularFile(fichier)) {
                System.err.println("Fichier introuvable : " + args[0] + ". Usage : java Main <carnet.csv> [recherche]");
                System.exit(1);
            }
            Main carnet = new Main();
            carnet.charger(fichier);
            afficher(args.length > 1 ? carnet.rechercher(args[1]) : carnet.contacts.values());
            return;
        }
        Path tmp = Files.createTempFile("carnet", ".csv");
        try {
            Main carnet = new Main();
            carnet.ajouter(new Contact("Zinsou, Paul", "+229 01 66 00 11 22", "paul@exemple.bj", "Cotonou"));
            carnet.ajouter(new Contact("Adjovi Mireille", "+229 01 97 33 44 55", "mireille@exemple.bj", "Abomey-Calavi"));
            carnet.ajouter(new Contact("Boutique \"Le Bon Prix\"", "+229 01 52 00 00 00", "contact@bonprix.bj", "Godomey"));
            carnet.sauver(tmp);
            System.out.println("Fichier CSV écrit :");
            Files.readAllLines(tmp).forEach(l -> System.out.println("  " + l));
            Main relu = new Main();
            relu.charger(tmp);
            System.out.println("\nCarnet relu (trié) :");
            afficher(relu.contacts.values());
            System.out.println("\nRecherche « calavi » :");
            afficher(relu.rechercher("calavi"));
        } finally {
            Files.deleteIfExists(tmp);
        }
    }
}
