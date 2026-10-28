/*
 * Jour 135 — Serveur HTTP minimal qui renvoie l'état d'un capteur
 *
 * Problème : on veut consulter la température et l'humidité de sa serre ou de
 * sa chambre froide depuis le navigateur du téléphone, sur le réseau local,
 * sans installer d'application.
 * Solution : un petit serveur HTTP avec com.sun.net.httpserver (inclus dans le
 * JDK). Deux routes : « / » renvoie une page HTML lisible sur mobile, et
 * « /api/capteur » renvoie l'état en JSON pour une application. Les mesures
 * évoluent dans un thread séparé ; un AtomicReference garantit qu'on lit
 * toujours un état cohérent. En démo, le programme démarre le serveur sur un
 * port libre, s'interroge lui-même avec HttpClient puis s'arrête ; avec
 * --serveur il reste allumé sur le port 8080.
 * On apprend : le protocole HTTP (méthode, chemin, code de statut, en-têtes),
 * HttpServer et HttpClient, le JSON fait main et le partage d'état entre threads.
 */
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.http.*;
import java.nio.charset.StandardCharsets;
import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.Locale;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicReference;

public class Main {
    record Etat(double temperature, double humidite, String heure) {
        String json() { return String.format(Locale.ROOT, "{\"temperature\":%.1f,\"humidite\":%.0f,\"heure\":\"%s\"}", temperature, humidite, heure); }
    }

    static final AtomicReference<Etat> ETAT = new AtomicReference<>(new Etat(28.0, 65, "--"));

    static void repondre(HttpExchange ex, int code, String type, String corps) throws IOException {
        byte[] b = corps.getBytes(StandardCharsets.UTF_8);
        ex.getResponseHeaders().set("Content-Type", type + "; charset=utf-8");
        ex.sendResponseHeaders(code, b.length);
        try (OutputStream os = ex.getResponseBody()) { os.write(b); }
    }

    static HttpServer demarrer(int port) throws IOException {
        HttpServer serveur = HttpServer.create(new InetSocketAddress("127.0.0.1", port), 0);
        serveur.createContext("/api/capteur", ex -> {
            if (!ex.getRequestMethod().equals("GET")) { repondre(ex, 405, "text/plain", "Méthode non autorisée"); return; }
            repondre(ex, 200, "application/json", ETAT.get().json());
        });
        serveur.createContext("/", ex -> {
            if (!ex.getRequestURI().getPath().equals("/")) { repondre(ex, 404, "text/plain", "Page introuvable"); return; }
            Etat e = ETAT.get();
            repondre(ex, 200, "text/html", String.format(Locale.FRANCE, "<!doctype html><meta name=viewport content='width=device-width'>"
                    + "<h1>Serre</h1><p>Température : <b>%.1f °C</b></p><p>Humidité : <b>%.0f %%</b></p><p>Mis à jour à %s</p>",
                    e.temperature(), e.humidite(), e.heure()));
        });
        serveur.setExecutor(Executors.newFixedThreadPool(2));
        serveur.start();
        return serveur;
    }

    public static void main(String[] args) throws Exception {
        ScheduledExecutorService capteur = Executors.newSingleThreadScheduledExecutor(r -> { Thread t = new Thread(r); t.setDaemon(true); return t; });
        capteur.scheduleAtFixedRate(() -> {
            Etat e = ETAT.get();
            ETAT.set(new Etat(e.temperature() + (Math.random() - 0.5) * 0.4, Math.min(95, Math.max(30, e.humidite() + (Math.random() - 0.5) * 2)),
                    LocalTime.now().truncatedTo(ChronoUnit.SECONDS).toString()));
        }, 0, 200, TimeUnit.MILLISECONDS);

        if (args.length == 1 && args[0].equals("--serveur")) {
            demarrer(8080);
            System.out.println("Serveur sur http://127.0.0.1:8080/ (Ctrl+C pour arrêter)");
            return;
        }
        if (args.length > 0) { System.err.println("Usage : java Main [--serveur]"); System.exit(1); }
        HttpServer serveur = demarrer(0);
        int port = serveur.getAddress().getPort();
        System.out.println("Serveur démarré sur un port libre ; interrogation automatique :");
        HttpClient client = HttpClient.newBuilder().proxy(HttpClient.Builder.NO_PROXY).build();
        Thread.sleep(300);
        for (String chemin : new String[]{"/api/capteur", "/", "/inconnu"}) {
            HttpResponse<String> r = client.send(HttpRequest.newBuilder(URI.create("http://127.0.0.1:" + port + chemin)).build(),
                    HttpResponse.BodyHandlers.ofString());
            String corps = r.body().length() > 70 ? r.body().substring(r.body().indexOf("<h1>"), r.body().indexOf("<h1>") + 60) + "…" : r.body();
            System.out.printf("GET %-13s → %d %s%n   %s%n", chemin, r.statusCode(), r.headers().firstValue("Content-Type").orElse(""), corps);
        }
        HttpResponse<String> post = client.send(HttpRequest.newBuilder(URI.create("http://127.0.0.1:" + port + "/api/capteur"))
                .POST(HttpRequest.BodyPublishers.ofString("{}")).build(), HttpResponse.BodyHandlers.ofString());
        System.out.printf("POST /api/capteur → %d %s%n", post.statusCode(), post.body());
        serveur.stop(0);
        ((ExecutorService) serveur.getExecutor()).shutdown();
        System.out.println("Serveur arrêté.");
    }
}
