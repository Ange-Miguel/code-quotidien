/*
 * Jour 192 — Mini-interpréteur de commandes (shell)
 *
 * Problème : comment fonctionne un terminal comme bash ou l'invite de
 * commandes de Windows ? Il lit une ligne, la découpe en commande et
 * arguments (en respectant les guillemets), exécute, garde un historique et
 * gère les erreurs. Comprendre cela aide à mieux utiliser le terminal, et à
 * écrire la console de commande d'un appareil embarqué.
 * Solution : un shell sur un système de fichiers virtuel en mémoire (des Map
 * imbriquées) : pwd, ls, cd (avec .. et chemins absolus), mkdir, touch, echo
 * (avec redirection > vers un fichier), cat, rm, history et help. Chaque
 * commande est une entrée d'une table (nom → fonction). Un analyseur découpe
 * la ligne en tenant compte des guillemets. La démo exécute un script ;
 * --interactif lit au clavier.
 * On apprend : l'analyse d'une ligne de commande, les arbres de répertoires,
 * la résolution de chemins relatifs et absolus, et la table de commandes.
 */
import java.util.*;
import java.util.function.Consumer;

public class Main {
    static final class Noeud {
        final String nom; final Noeud parent; final TreeMap<String, Noeud> enfants; String contenu = "";
        Noeud(String nom, Noeud parent, boolean dossier) { this.nom = nom; this.parent = parent; this.enfants = dossier ? new TreeMap<>() : null; }
        boolean dossier() { return enfants != null; }
        String chemin() { return parent == null ? "/" : (parent.parent == null ? "" : parent.chemin()) + "/" + nom; }
    }

    final Noeud racine = new Noeud("", null, true);
    Noeud courant = racine;
    final List<String> historique = new ArrayList<>();
    final Map<String, Consumer<List<String>>> commandes = new LinkedHashMap<>();

    Main() {
        commandes.put("pwd", a -> System.out.println(courant.chemin()));
        commandes.put("ls", a -> { Noeud n = a.isEmpty() ? courant : resoudre(a.get(0)); exiger(n != null && n.dossier(), "dossier introuvable");
            n.enfants.values().forEach(e -> System.out.println(e.nom + (e.dossier() ? "/" : ""))); });
        commandes.put("cd", a -> { Noeud n = resoudre(a.isEmpty() ? "/" : a.get(0)); exiger(n != null && n.dossier(), "dossier introuvable"); courant = n; });
        commandes.put("mkdir", a -> a.forEach(nom -> creer(nom, true)));
        commandes.put("touch", a -> a.forEach(nom -> { if (resoudre(nom) == null) creer(nom, false); }));
        commandes.put("cat", a -> a.forEach(nom -> { Noeud n = resoudre(nom); exiger(n != null && !n.dossier(), nom + " : fichier introuvable"); System.out.print(n.contenu); }));
        commandes.put("rm", a -> a.forEach(nom -> { Noeud n = resoudre(nom); exiger(n != null && n != racine, nom + " : introuvable");
            exiger(!n.dossier() || n.enfants.isEmpty(), nom + " : dossier non vide"); n.parent.enfants.remove(n.nom); }));
        commandes.put("history", a -> { for (int i = 0; i < historique.size(); i++) System.out.printf("%3d  %s%n", i + 1, historique.get(i)); });
        commandes.put("help", a -> System.out.println("Commandes : " + String.join(", ", commandes.keySet()) + " ; echo texte > fichier"));
    }

    static void exiger(boolean condition, String message) { if (!condition) throw new IllegalArgumentException(message); }

    Noeud resoudre(String chemin) {
        Noeud n = chemin.startsWith("/") ? racine : courant;
        for (String part : chemin.split("/")) {
            if (part.isEmpty() || part.equals(".")) continue;
            if (part.equals("..")) { n = n.parent == null ? n : n.parent; continue; }
            if (!n.dossier() || !n.enfants.containsKey(part)) return null;
            n = n.enfants.get(part);
        }
        return n;
    }

    Noeud creer(String chemin, boolean dossier) {
        int i = chemin.lastIndexOf('/');
        Noeud parent = i < 0 ? courant : resoudre(i == 0 ? "/" : chemin.substring(0, i));
        String nom = chemin.substring(i + 1);
        exiger(parent != null && parent.dossier(), "dossier parent introuvable");
        exiger(!nom.isEmpty() && !parent.enfants.containsKey(nom), nom + " existe déjà");
        Noeud n = new Noeud(nom, parent, dossier);
        parent.enfants.put(nom, n);
        return n;
    }

    static List<String> decouper(String ligne) {
        List<String> mots = new ArrayList<>();
        StringBuilder sb = new StringBuilder();
        boolean guillemets = false, mot = false;
        for (char c : ligne.toCharArray()) {
            if (c == '"') { guillemets = !guillemets; mot = true; }
            else if (c == ' ' && !guillemets) { if (mot) mots.add(sb.toString()); sb.setLength(0); mot = false; }
            else { sb.append(c); mot = true; }
        }
        if (guillemets) throw new IllegalArgumentException("guillemet non fermé");
        if (mot) mots.add(sb.toString());
        return mots;
    }

    void executer(String ligne) {
        if (ligne.isBlank()) return;
        historique.add(ligne);
        try {
            List<String> mots = decouper(ligne);
            if (mots.get(0).equals("echo")) {
                int r = mots.indexOf(">");
                String texte = String.join(" ", mots.subList(1, r < 0 ? mots.size() : r)) + "\n";
                if (r < 0) { System.out.print(texte); return; }
                exiger(r + 1 < mots.size(), "fichier de destination manquant");
                Noeud f = resoudre(mots.get(r + 1));
                if (f == null) f = creer(mots.get(r + 1), false);
                exiger(!f.dossier(), "impossible d'écrire dans un dossier");
                f.contenu = texte;
                return;
            }
            Consumer<List<String>> cmd = commandes.get(mots.get(0));
            exiger(cmd != null, mots.get(0) + " : commande introuvable (tapez help)");
            cmd.accept(mots.subList(1, mots.size()));
        } catch (IllegalArgumentException e) {
            System.out.println("erreur : " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        Main shell = new Main();
        if (args.length == 1 && args[0].equals("--interactif")) {
            Scanner in = new Scanner(System.in);
            while (true) { System.out.print(shell.courant.chemin() + " $ "); if (!in.hasNextLine()) return; shell.executer(in.nextLine()); }
        }
        if (args.length > 0) { System.err.println("Usage : java Main [--interactif]"); System.exit(1); }
        String[] script = {"pwd", "mkdir projets cours", "cd projets", "mkdir covoiturage", "echo \"Covoiturage étudiant\" > covoiturage/README",
                "ls covoiturage", "cat covoiturage/README", "cd ../cours", "touch electronique.txt", "ls", "cd /", "ls", "rm projets",
                "cd introuvable", "lancer-fusee", "echo \"guillemet non fermé", "history"};
        for (String l : script) { System.out.println(shell.courant.chemin() + " $ " + l); shell.executer(l); }
    }
}
