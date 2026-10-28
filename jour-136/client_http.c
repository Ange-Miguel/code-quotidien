/*
 * Jour 136 — Client HTTP qui envoie des mesures à une API
 *
 * Problème : une carte (Raspberry Pi, passerelle Linux) doit envoyer ses
 * mesures à un serveur web par une requête HTTP POST, comme le font les
 * plateformes IoT. Comprendre ce qui circule réellement sur le réseau aide à
 * déboguer quand « ça ne marche pas ».
 * Solution : on écrit la requête HTTP à la main avec les sockets POSIX :
 * ligne de requête, en-têtes (Host, Content-Type, Content-Length, Connection)
 * et corps JSON, puis on lit et analyse la réponse (code de statut). Pour que
 * la démo fonctionne sans Internet, le programme lance avec fork() un mini
 * serveur local qui reçoit la requête, l'affiche et répond « 201 Created ».
 * On apprend : socket, connect, send/recv, le format exact d'une requête HTTP,
 * le calcul de Content-Length et fork() pour créer un processus.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

static int envoyer_mesure(int port, double temperature, double humidite) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    if (s < 0 || connect(s, (struct sockaddr *)&adr, sizeof adr) < 0) { perror("connexion"); if (s >= 0) close(s); return -1; }
    char corps[128], requete[512];
    int lc = snprintf(corps, sizeof corps, "{\"capteur\":\"serre-01\",\"temperature\":%.1f,\"humidite\":%.0f}", temperature, humidite);
    int lr = snprintf(requete, sizeof requete,
                      "POST /api/mesures HTTP/1.1\r\nHost: 127.0.0.1:%d\r\nContent-Type: application/json\r\n"
                      "Content-Length: %d\r\nConnection: close\r\n\r\n%s", port, lc, corps);
    if (send(s, requete, (size_t)lr, 0) != lr) { perror("envoi"); close(s); return -1; }
    char reponse[1024];
    ssize_t n = recv(s, reponse, sizeof reponse - 1, 0);
    close(s);
    if (n <= 0) return -1;
    reponse[n] = '\0';
    int code = 0;
    sscanf(reponse, "HTTP/1.%*d %d", &code);
    char *fin_ligne = strstr(reponse, "\r\n");
    if (fin_ligne) *fin_ligne = '\0';
    printf("CLIENT  ← %s\n", reponse);
    return code;
}

static void mini_serveur(int ecoute, int requetes) {
    for (int k = 0; k < requetes; k++) {
        int c = accept(ecoute, NULL, NULL);
        if (c < 0) break;
        char tampon[1024];
        ssize_t n = recv(c, tampon, sizeof tampon - 1, 0);
        if (n > 0) {
            tampon[n] = '\0';
            printf("SERVEUR reçu %zd octets :\n", n);
            for (char *ligne = strtok(tampon, "\r\n"); ligne; ligne = strtok(NULL, "\r\n")) printf("        | %s\n", ligne);
            fflush(stdout);
            const char *rep = strstr(tampon, "POST") ? "HTTP/1.1 201 Created\r\nContent-Length: 0\r\nConnection: close\r\n\r\n"
                                                    : "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
            if (send(c, rep, strlen(rep), 0) < 0) perror("réponse");
        }
        close(c);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 2) {           /* envoi vers un serveur local réel sur le port donné */
        char *fin;
        long port = strtol(argv[1], &fin, 10);
        if (*fin || port < 1 || port > 65535) { fprintf(stderr, "Usage : %s [port d'un serveur local]\n", argv[0]); return 1; }
        return envoyer_mesure((int)port, 28.4, 71) >= 200 ? 0 : 1;
    }
    if (argc > 2) { fprintf(stderr, "Usage : %s [port]\n", argv[0]); return 1; }
    int ecoute = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    socklen_t lg = sizeof adr;
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (ecoute < 0 || bind(ecoute, (struct sockaddr *)&adr, sizeof adr) < 0 || listen(ecoute, 4) < 0 ||
        getsockname(ecoute, (struct sockaddr *)&adr, &lg) < 0) { perror("serveur"); return 2; }
    int port = ntohs(adr.sin_port);
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) { mini_serveur(ecoute, 2); close(ecoute); fflush(stdout); _exit(0); }
    close(ecoute);
    double mesures[][2] = {{28.4, 71}, {29.1, 68}};
    for (int i = 0; i < 2; i++) {
        int code = envoyer_mesure(port, mesures[i][0], mesures[i][1]);
        printf("CLIENT  mesure %d : %s (code %d)\n\n", i + 1, code == 201 ? "acceptée" : "refusée", code);
        fflush(stdout);
    }
    waitpid(pid, NULL, 0);
    return 0;
}
