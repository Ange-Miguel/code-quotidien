"""Jour 137 — Socket TCP : échange de messages capteur ↔ passerelle

Problème : plusieurs capteurs d'un même site (atelier, ferme) envoient leurs
mesures à une passerelle locale, qui peut aussi leur répondre (accusé de
réception, nouvelle consigne). TCP garantit l'ordre et la livraison, mais
c'est un flux d'octets : il faut découper soi-même les messages.
Solution : un serveur TCP (la passerelle) qui accepte plusieurs clients en
parallèle grâce aux threads, et un protocole simple : chaque message est une
ligne JSON terminée par « \\n ». Les capteurs se connectent, s'identifient,
envoient leurs mesures et reçoivent un accusé de réception ; la passerelle
renvoie une nouvelle période de mesure si la température est élevée. Tout
fonctionne sur la machine locale (127.0.0.1), sur un port choisi par le système.
On apprend : socket, bind, listen, accept, connect, le découpage d'un flux en
messages (makefile/readline), les threads et le verrouillage d'une structure
partagée.
"""
import json
import socket
import sys
import threading


class Passerelle:
    def __init__(self):
        self.serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.serveur.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.serveur.bind(("127.0.0.1", 0))
        self.serveur.listen()
        self.port = self.serveur.getsockname()[1]
        self.mesures: dict[str, list[float]] = {}
        self.verrou = threading.Lock()

    def traiter_client(self, conn: socket.socket) -> None:
        with conn, conn.makefile("rw", encoding="utf-8", newline="\n") as flux:
            identite = None
            for ligne in flux:
                try:
                    msg = json.loads(ligne)
                except json.JSONDecodeError:
                    flux.write(json.dumps({"erreur": "JSON invalide"}) + "\n"); flux.flush()
                    continue
                if msg.get("type") == "bonjour":
                    identite = msg["id"]
                    reponse = {"ok": True, "periode_s": 60}
                elif msg.get("type") == "mesure" and identite:
                    with self.verrou:
                        self.mesures.setdefault(identite, []).append(msg["t"])
                    reponse = {"ok": True, "periode_s": 10 if msg["t"] > 35 else 60}
                else:
                    reponse = {"erreur": "identifiez-vous d'abord"}
                flux.write(json.dumps(reponse) + "\n")
                flux.flush()

    def demarrer(self, nb_clients: int) -> threading.Thread:
        def boucle():
            fils = []
            for _ in range(nb_clients):
                conn, _ = self.serveur.accept()
                t = threading.Thread(target=self.traiter_client, args=(conn,))
                t.start()
                fils.append(t)
            for t in fils:
                t.join()
            self.serveur.close()
        t = threading.Thread(target=boucle)
        t.start()
        return t


def capteur(port: int, ident: str, temperatures: list[float], sortie: list[str]) -> None:
    with socket.create_connection(("127.0.0.1", port), timeout=5) as s, s.makefile("rw", encoding="utf-8", newline="\n") as f:
        def echanger(msg) -> dict:
            f.write((msg if isinstance(msg, str) else json.dumps(msg)) + "\n")
            f.flush()
            return json.loads(f.readline())
        r = echanger({"type": "bonjour", "id": ident})
        sortie.append(f"{ident} connecté, période {r['periode_s']} s")
        for t in temperatures:
            r = echanger({"type": "mesure", "t": t})
            sortie.append(f"{ident} envoie {t} °C → ok, prochaine mesure dans {r['periode_s']} s")
        r = echanger("{pas du json")
        sortie.append(f"{ident} envoie un message abîmé → {r}")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    p = Passerelle()
    serveur = p.demarrer(nb_clients=3)
    print(f"Passerelle à l'écoute sur 127.0.0.1:{p.port}\n")
    journaux = {i: [] for i in ("capteur-serre", "capteur-four", "capteur-stock")}
    donnees = {"capteur-serre": [29.5, 30.1], "capteur-four": [33.0, 36.4, 38.2], "capteur-stock": [24.0]}
    fils = [threading.Thread(target=capteur, args=(p.port, i, donnees[i], journaux[i])) for i in journaux]
    for t in fils:
        t.start()
    for t in fils:
        t.join()
    serveur.join()
    for i, lignes in journaux.items():
        print("\n".join("   " + l for l in lignes))
    print(f"\nMesures reçues par la passerelle : {p.mesures}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
