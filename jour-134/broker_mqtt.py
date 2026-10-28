"""Jour 134 — Broker MQTT minimal en mémoire avec topics à jokers

Problème : dans MQTT, un abonné peut écouter plusieurs sujets à la fois avec
des jokers : « maison/+/temperature » (toutes les pièces) ou « maison/# »
(tout ce qui concerne la maison). Le broker (le serveur central) doit savoir
à qui livrer chaque message, avec une qualité de service et des messages
retenus.
Solution : un broker en mémoire. Les abonnements sont stockés avec leur
filtre ; la correspondance sujet / filtre suit exactement les règles MQTT :
« + » remplace un niveau, « # » (seulement à la fin) remplace tous les
niveaux restants, y compris aucun. On teste les règles avec des cas connus,
puis on simule une maison avec plusieurs capteurs et applications.
On apprend : les règles de correspondance des topics MQTT, la découpe par
niveaux, la validation des filtres et la conception d'un broker simple.
"""
import sys
from collections import defaultdict


def filtre_valide(filtre: str) -> bool:
    niveaux = filtre.split("/")
    for i, n in enumerate(niveaux):
        if "#" in n and (n != "#" or i != len(niveaux) - 1):
            return False
        if "+" in n and n != "+":
            return False
    return bool(filtre)


def correspond(filtre: str, sujet: str) -> bool:
    f, s = filtre.split("/"), sujet.split("/")
    for i, niveau in enumerate(f):
        if niveau == "#":
            return True
        if i >= len(s) or (niveau != "+" and niveau != s[i]):
            return False
    return len(f) == len(s)


class Broker:
    def __init__(self):
        self.abonnements = defaultdict(set)     # client → filtres
        self.retenus: dict[str, str] = {}
        self.boites: dict[str, list[tuple[str, str]]] = defaultdict(list)

    def abonner(self, client: str, filtre: str) -> None:
        if not filtre_valide(filtre):
            raise ValueError(f"filtre invalide : {filtre}")
        self.abonnements[client].add(filtre)
        for sujet, msg in self.retenus.items():
            if correspond(filtre, sujet):
                self.boites[client].append((sujet, msg))

    def publier(self, sujet: str, message: str, retenir: bool = False) -> int:
        if "+" in sujet or "#" in sujet:
            raise ValueError("pas de joker dans un sujet de publication")
        if retenir:
            self.retenus[sujet] = message
        destinataires = [c for c, filtres in self.abonnements.items() if any(correspond(f, sujet) for f in filtres)]
        for c in destinataires:
            self.boites[c].append((sujet, message))   # un seul exemplaire même si plusieurs filtres
        return len(destinataires)


TESTS = [("maison/+/temperature", "maison/salon/temperature", True),
         ("maison/+/temperature", "maison/salon/humidite", False),
         ("maison/#", "maison", True), ("maison/#", "maison/cuisine/gaz/alarme", True),
         ("+/+", "a/b", True), ("+/+", "a/b/c", False), ("#", "n/importe/quoi", True),
         ("maison/salon", "maison/salon/temperature", False)]


def main() -> int:
    for f, s, attendu in TESTS:
        assert correspond(f, s) == attendu, (f, s)
    assert not filtre_valide("maison/#/salon") and not filtre_valide("mai+son")
    if len(sys.argv) == 3:
        if not filtre_valide(sys.argv[1]):
            print(f"Filtre invalide : {sys.argv[1]}", file=sys.stderr)
            return 1
        print(f"{sys.argv[1]} {'correspond à' if correspond(sys.argv[1], sys.argv[2]) else 'ne correspond pas à'} {sys.argv[2]}")
        return 0
    if len(sys.argv) != 1:
        print("Usage : python3 broker_mqtt.py [filtre sujet]", file=sys.stderr)
        return 1
    print(f"{len(TESTS)} tests de correspondance réussis\n")
    b = Broker()
    b.publier("maison/portail/etat", "fermé", retenir=True)
    b.abonner("tableau-de-bord", "maison/#")
    b.abonner("climatisation", "maison/+/temperature")
    b.abonner("securite", "maison/+/mouvement")
    b.abonner("securite", "maison/portail/#")
    b.abonner("historique", "#")
    for sujet, msg in [("maison/salon/temperature", "28.4"), ("maison/chambre/temperature", "26.1"),
                       ("maison/entree/mouvement", "1"), ("maison/portail/etat", "ouvert"), ("ferme/serre/humidite", "64")]:
        print(f"PUBLICATION {sujet:28} « {msg} » → {b.publier(sujet, msg)} abonné(s)")
    print()
    for client, msgs in b.boites.items():
        print(f"{client:16} a reçu {len(msgs)} message(s) : " + ", ".join(s.split("/", 1)[-1] for s, _ in msgs))
    try:
        b.abonner("pirate", "maison/#/salon")
    except ValueError as e:
        print(f"\nAbonnement refusé : {e}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
