"""Jour 131 — Encodage JSON léger d'une mesure de capteur

Problème : un objet connecté envoie ses mesures à un serveur ou à une
application. JSON est le format universel, mais sur une liaison LoRa ou
GSM facturée à l'octet, chaque caractère compte : '{"temperature": 28.4567}'
gaspille beaucoup par rapport à '{"t":28.5}'.
Solution : on construit la même mesure de trois façons : JSON lisible
(indenté), JSON compact (séparateurs sans espaces, clés courtes, valeurs
arrondies à la précision réelle du capteur) et un format binaire avec le
module struct. On vérifie que chaque format se relit sans perte de l'info
utile, et on compare les tailles et le coût d'envoi d'un an de mesures.
On apprend : le module json (dumps, separators, sort_keys), la précision
significative, struct.pack et le compromis lisibilité / taille.
"""
import json
import struct
import sys
import time

CLES_COURTES = {"capteur": "id", "temperature": "t", "humidite": "h", "batterie": "b", "horodatage": "ts"}


def mesure_complete() -> dict:
    return {"capteur": "ferme-calavi-03", "temperature": 28.456789, "humidite": 71.234,
            "batterie": 3.71234, "horodatage": 1793174400}


def json_lisible(m: dict) -> str:
    return json.dumps(m, indent=2, ensure_ascii=False)


def json_compact(m: dict) -> str:
    arrondi = {"temperature": 1, "humidite": 0, "batterie": 2}   # précision réelle des capteurs
    court = {}
    for cle, valeur in m.items():
        if cle in arrondi:
            valeur = round(valeur, arrondi[cle])
            valeur = int(valeur) if arrondi[cle] == 0 else valeur
        court[CLES_COURTES[cle]] = valeur
    return json.dumps(court, separators=(",", ":"))


def binaire(m: dict) -> bytes:
    # id numérique sur 2 octets, t et h en centièmes (int16), batterie en mV (uint16), horodatage uint32
    return struct.pack("<HhhHI", 3, round(m["temperature"] * 100), round(m["humidite"] * 100),
                       round(m["batterie"] * 1000), m["horodatage"])


def relire_binaire(b: bytes) -> dict:
    ident, t, h, bat, ts = struct.unpack("<HhhHI", b)
    return {"id": ident, "t": t / 100, "h": h / 100, "b": bat / 1000, "ts": ts}


def main() -> int:
    if len(sys.argv) == 2:
        try:
            m = json.loads(sys.argv[1])
            if not isinstance(m, dict) or not set(m) <= set(CLES_COURTES):
                raise ValueError("clés attendues : " + ", ".join(CLES_COURTES))
            print(json_compact(m))
        except (ValueError, TypeError) as e:
            print(f"Erreur : {e}", file=sys.stderr)
            return 1
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 json_leger.py ['{\"temperature\": 28.4}']", file=sys.stderr)
        return 1
    m = mesure_complete()
    formats = [("JSON lisible", json_lisible(m).encode()), ("JSON compact", json_compact(m).encode()),
               ("Binaire (struct)", binaire(m))]
    print("Même mesure, trois formats :\n")
    print(json_lisible(m))
    print(json_compact(m))
    print(binaire(m).hex(" "))
    assert json.loads(json_compact(m))["t"] == 28.5
    assert abs(relire_binaire(binaire(m))["t"] - 28.46) < 1e-9
    print(f"\n{'Format':18} {'Octets':>6} {'Gain':>6} {'1 an, 1 mesure/10 min':>24}")
    base = len(formats[0][1])
    for nom, donnees in formats:
        an = len(donnees) * 6 * 24 * 365
        print(f"{nom:18} {len(donnees):6d} {1 - len(donnees) / base:6.0%} {an / 1024:20.0f} Ko")
    print(f"\nHorodatage relu : {time.strftime('%Y-%m-%d %H:%M UTC', time.gmtime(m['horodatage']))}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
