"""Jour 122 — Détection de coupure de courant et calcul de durée (journal)

Problème : les coupures d'électricité perturbent commerces, cliniques et
ateliers. Pour se plaindre au fournisseur, dimensionner un onduleur ou un
groupe électrogène, il faut des chiffres : combien de coupures, quand,
combien de temps au total ?
Solution : une petite carte (ESP32 sur batterie) note toutes les minutes la
tension du secteur. On détecte une coupure quand la tension tombe sous
150 V pendant au moins 2 relevés (pour ignorer les micro-coupures), et la
fin quand elle repasse au-dessus de 180 V (hystérésis). On reconstitue la
liste des coupures, on calcule les statistiques par jour et par tranche
horaire, et la disponibilité en pourcentage.
On apprend : la détection d'événements dans une série temporelle, la
fusion de micro-coupures, datetime et les statistiques de disponibilité.
"""
import random
import sys
from collections import Counter
from datetime import datetime, timedelta

SEUIL_BAS, SEUIL_HAUT, CONFIRMATION = 150, 180, 2
JOURS = ["lun", "mar", "mer", "jeu", "ven", "sam", "dim"]


def generer_releves(debut: datetime, jours: int, rng: random.Random) -> list[tuple[datetime, float]]:
    coupures = []
    for j in range(jours):
        for _ in range(rng.randint(0, 3)):
            heure = rng.choice([7, 12, 13, 19, 20, 21, 2]) * 60 + rng.randint(0, 59)
            coupures.append((j * 1440 + heure, rng.choice([1, 15, 45, 90, 180])))
    releves = []
    for m in range(jours * 1440):
        coupe = any(d <= m < d + duree for d, duree in coupures)
        tension = rng.gauss(5, 3) if coupe else rng.gauss(222, 6)
        releves.append((debut + timedelta(minutes=m), max(0.0, tension)))
    return releves


def detecter(releves) -> list[tuple[datetime, datetime]]:
    coupures, en_coupure, compteur, debut = [], False, 0, None
    for t, v in releves:
        if not en_coupure:
            compteur = compteur + 1 if v < SEUIL_BAS else 0
            if compteur == 1:
                debut = t
            if compteur >= CONFIRMATION:
                en_coupure = True
        elif v > SEUIL_HAUT:
            coupures.append((debut, t))
            en_coupure, compteur = False, 0
    if en_coupure:
        coupures.append((debut, releves[-1][0]))
    return coupures


def main() -> int:
    jours = 7
    if len(sys.argv) == 2:
        try:
            jours = int(sys.argv[1])
            if not 1 <= jours <= 60:
                raise ValueError
        except ValueError:
            print("Usage : python3 coupures.py [nombre de jours 1..60]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 coupures.py [jours]", file=sys.stderr)
        return 1
    debut = datetime(2026, 10, 19)
    releves = generer_releves(debut, jours, random.Random(122))
    coupures = detecter(releves)
    total = sum((f - d for d, f in coupures), timedelta())
    print(f"{len(releves)} relevés sur {jours} jours → {len(coupures)} coupure(s) d'au moins {CONFIRMATION} min\n")
    for d, f in coupures:
        print(f"   {JOURS[d.weekday()]} {d:%d/%m %H:%M} → {f:%H:%M}  ({int((f - d).total_seconds() // 60):3d} min)")
    print(f"\nDurée totale sans courant : {total} | disponibilité {100 * (1 - total / timedelta(days=jours)):.2f} %")
    if coupures:
        plus_longue = max(coupures, key=lambda c: c[1] - c[0])
        print(f"Plus longue coupure : {plus_longue[1] - plus_longue[0]} le {plus_longue[0]:%d/%m}")
        tranches = Counter(d.hour // 6 for d, _ in coupures)
        noms = ["nuit (0-6 h)", "matin (6-12 h)", "après-midi (12-18 h)", "soirée (18-24 h)"]
        print("Répartition :", ", ".join(f"{noms[k]} {tranches[k]}" for k in range(4)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
