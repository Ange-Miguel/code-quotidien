"""Jour 098 — Gestion de parking : places libres et durée de stationnement

Problème : un parking de marché ou d'hôpital veut savoir en temps réel
combien de places sont libres, combien de temps chaque véhicule est resté, et
combien facturer (tarif dégressif, première demi-heure gratuite, forfait
journalier plafonné).
Solution : une classe Parking garde les places dans un dictionnaire
{numéro de place: (immatriculation, heure d'entrée)}. L'entrée attribue la
plus petite place libre (tas heapq), la sortie calcule la durée et le tarif.
On rejoue une journée d'entrées et de sorties et on produit les statistiques
(taux d'occupation, recette, durée moyenne).
On apprend : heapq pour trouver vite la plus petite place libre, datetime et
timedelta, les règles de tarification et la validation d'une plaque.
"""
import heapq
import re
import sys
from datetime import datetime, timedelta

PLAQUE = re.compile(r"^[A-Z]{2} \d{4} RB$")    # format simplifié, exemple
GRATUIT = timedelta(minutes=30)


def tarif(duree: timedelta) -> int:
    if duree <= GRATUIT:
        return 0
    heures = -(-int((duree - GRATUIT).total_seconds()) // 3600)     # arrondi à l'heure supérieure
    prix = sum(200 if h < 2 else 100 for h in range(heures))       # 200/h les 2 premières, puis 100
    return min(prix, 1500)                                         # plafond journalier


class Parking:
    def __init__(self, places: int):
        self.libres = list(range(1, places + 1))
        heapq.heapify(self.libres)
        self.occupees: dict[int, tuple[str, datetime]] = {}
        self.capacite = places
        self.recette = 0
        self.durees: list[timedelta] = []

    def entrer(self, plaque: str, heure: datetime) -> int | None:
        if not PLAQUE.match(plaque):
            raise ValueError(f"plaque invalide : {plaque}")
        if any(p == plaque for p, _ in self.occupees.values()):
            raise ValueError(f"{plaque} est déjà dans le parking")
        if not self.libres:
            return None
        place = heapq.heappop(self.libres)
        self.occupees[place] = (plaque, heure)
        return place

    def sortir(self, plaque: str, heure: datetime) -> tuple[int, timedelta, int]:
        for place, (p, entree) in self.occupees.items():
            if p == plaque:
                del self.occupees[place]
                heapq.heappush(self.libres, place)
                duree = heure - entree
                montant = tarif(duree)
                self.recette += montant
                self.durees.append(duree)
                return place, duree, montant
        raise ValueError(f"{plaque} introuvable")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    p = Parking(4)
    j = datetime(2026, 10, 28)
    evenements = [("E", "AB 1234 RB", 7, 50), ("E", "CD 5678 RB", 8, 5), ("E", "EF 9012 RB", 8, 30),
                  ("E", "GH 3456 RB", 9, 0), ("E", "IJ 7890 RB", 9, 10), ("S", "CD 5678 RB", 9, 20),
                  ("E", "IJ 7890 RB", 9, 25), ("E", "zz 0000", 9, 30), ("S", "AB 1234 RB", 17, 45),
                  ("S", "EF 9012 RB", 11, 0), ("S", "GH 3456 RB", 9, 25), ("S", "IJ 7890 RB", 12, 40)]
    for type_, plaque, h, m in sorted(evenements, key=lambda e: (e[2], e[3])):
        heure = j.replace(hour=h, minute=m)
        try:
            if type_ == "E":
                place = p.entrer(plaque, heure)
                print(f"{heure:%H:%M} entrée {plaque} → " + (f"place {place}" if place else "COMPLET, refusé"))
            else:
                place, duree, montant = p.sortir(plaque, heure)
                print(f"{heure:%H:%M} sortie {plaque} (place {place}) après {duree} → {montant} FCFA")
        except ValueError as e:
            print(f"{heure:%H:%M} refus : {e}")
        print(f"        places libres : {len(p.libres)}/{p.capacite}")
    moyenne = sum(p.durees, timedelta()) / len(p.durees)
    print(f"\nRecette : {p.recette} FCFA | {len(p.durees)} sorties | durée moyenne {moyenne}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
