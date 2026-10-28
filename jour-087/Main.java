/*
 * Jour 087 — Renommage en masse de fichiers photos par date
 *
 * Problème : des centaines de photos nommées « IMG_4521.jpg », « DSC0012.JPG »
 * ou « photo (3).jpeg » sont impossibles à classer. On veut des noms comme
 * « 2026-10-14_09-31-05.jpg », triés naturellement par date.
 * Solution : pour chaque image, on lit sa date de dernière modification (les
 * vraies photos ont aussi une date EXIF, non traitée ici), on construit le
 * nouveau nom, on ajoute un suffixe _2, _3… en cas de collision, et on
 * affiche le plan de renommage. Par défaut, c'est une simulation : les
 * fichiers ne sont renommés qu'avec l'option --appliquer.
 * On apprend : java.nio.file (Files, Path, attributs), le formatage de dates,
 * la gestion des collisions de noms et le mode « simulation » avant toute
 * action destructive.
 */
import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.FileTime;
import java.time.*;
import java.time.format.DateTimeFormatter;
import java.util.*;
import java.util.stream.Stream;

public class Main {
    static final DateTimeFormatter FORMAT = DateTimeFormatter.ofPattern("yyyy-MM-dd_HH-mm-ss");
    static final Set<String> EXTENSIONS = Set.of("jpg", "jpeg", "png", "heic", "mp4");

    static String extension(Path p) {
        String n = p.getFileName().toString();
        int i = n.lastIndexOf('.');
        return i < 0 ? "" : n.substring(i + 1).toLowerCase(Locale.ROOT);
    }

    static Map<Path, Path> planifier(Path dossier) throws IOException {
        Map<Path, Path> plan = new LinkedHashMap<>();
        Set<String> pris = new HashSet<>();
        List<Path> fichiers;
        try (Stream<Path> s = Files.list(dossier)) {
            fichiers = s.filter(Files::isRegularFile).filter(p -> EXTENSIONS.contains(extension(p))).sorted().toList();
        }
        for (Path p : fichiers) {
            LocalDateTime date = LocalDateTime.ofInstant(Files.getLastModifiedTime(p).toInstant(), ZoneId.of("Africa/Porto-Novo"));
            String ext = extension(p).equals("jpeg") ? "jpg" : extension(p);
            String base = FORMAT.format(date), nom = base + "." + ext;
            for (int k = 2; pris.contains(nom); k++) nom = base + "_" + k + "." + ext;
            pris.add(nom);
            if (!nom.equals(p.getFileName().toString())) plan.put(p, p.resolveSibling(nom));
        }
        return plan;
    }

    static void executer(Path dossier, boolean appliquer) throws IOException {
        Map<Path, Path> plan = planifier(dossier);
        for (var e : plan.entrySet()) {
            System.out.printf("  %-22s → %s%n", e.getKey().getFileName(), e.getValue().getFileName());
            if (appliquer) Files.move(e.getKey(), e.getValue());
        }
        System.out.printf("%d fichier(s) %s%n", plan.size(), appliquer ? "renommé(s)" : "à renommer (simulation : ajoutez --appliquer)");
    }

    public static void main(String[] args) throws IOException {
        if (args.length >= 1) {
            Path d = Path.of(args[0]);
            if (!Files.isDirectory(d)) { System.err.println("Dossier introuvable : " + args[0]); System.exit(1); }
            executer(d, args.length > 1 && args[1].equals("--appliquer"));
            return;
        }
        Path d = Files.createTempDirectory("photos");
        try {
            Instant base = LocalDateTime.of(2026, 10, 14, 9, 31, 5).atZone(ZoneId.of("Africa/Porto-Novo")).toInstant();
            String[] noms = {"IMG_4521.jpg", "DSC0012.JPG", "photo (3).jpeg", "IMG_4522.jpg", "video.mp4", "notes.txt"};
            int[] minutes = {0, 125, 2000, 0, 3500, 10};
            for (int i = 0; i < noms.length; i++) {
                Path f = Files.writeString(d.resolve(noms[i]), "contenu " + i);
                Files.setLastModifiedTime(f, FileTime.from(base.plusSeconds(60L * minutes[i])));
            }
            System.out.println("Simulation :");
            executer(d, false);
            System.out.println("\nApplication :");
            executer(d, true);
            try (Stream<Path> s = Files.list(d)) {
                System.out.println("Contenu final : " + s.map(p -> p.getFileName().toString()).sorted().toList());
            }
        } finally {
            try (Stream<Path> s = Files.walk(d)) {
                for (Path p : s.sorted(Comparator.reverseOrder()).toList()) Files.deleteIfExists(p);
            }
        }
    }
}
