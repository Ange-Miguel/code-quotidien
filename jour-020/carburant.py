"""Jour 020 — Calculateur de consommation de carburant

Problème : un conducteur de zem ou de voiture veut connaître sa consommation
réelle (L/100 km) et ce que lui coûte chaque kilomètre, pour fixer ses prix ou
comparer deux motos.
Solution : à partir de pleins successifs (kilométrage au compteur et litres
mis), on calcule la consommation de chaque trajet, la moyenne globale et le
coût par kilomètre selon le prix du litre.
On apprend : les dataclasses, le calcul sur des paires d'éléments successifs
(zip), les moyennes pondérées et la détection d'incohérences.
"""
import sys
from dataclasses import dataclass

PRIX_LITRE = 680  # FCFA, valeur d'exemple modifiable


@dataclass
class Plein:
    compteur_km: float
    litres: float


def analyser(pleins: list[Plein], prix: float = PRIX_LITRE) -> None:
    if len(pleins) < 2:
        raise ValueError("il faut au moins deux pleins")
    total_km = total_l = 0.0
    print(f"{'Trajet':>8} {'km':>7} {'litres':>7} {'L/100km':>8} {'FCFA/km':>8}")
    for i, (avant, apres) in enumerate(zip(pleins, pleins[1:]), 1):
        km = apres.compteur_km - avant.compteur_km
        if km <= 0 or apres.litres <= 0:
            raise ValueError(f"trajet {i} incohérent (compteur ou litres)")
        conso = apres.litres / km * 100
        print(f"{i:>8} {km:7.0f} {apres.litres:7.1f} {conso:8.2f} {apres.litres * prix / km:8.1f}")
        total_km += km
        total_l += apres.litres
    moyenne = total_l / total_km * 100
    print(f"\nTotal : {total_km:.0f} km, {total_l:.1f} L → moyenne {moyenne:.2f} L/100 km")
    print(f"Coût moyen : {total_l * prix / total_km:.1f} FCFA/km (litre à {prix} FCFA)")
    print(f"Autonomie estimée avec 10 L : {10 / moyenne * 100:.0f} km")


def main() -> int:
    if len(sys.argv) == 1:
        pleins = [Plein(12000, 0), Plein(12180, 4.1), Plein(12395, 4.8), Plein(12560, 3.9)]
        analyser(pleins)
        return 0
    try:
        valeurs = [float(v) for v in sys.argv[1:]]
        if len(valeurs) % 2:
            raise ValueError("donner des paires compteur litres")
        analyser([Plein(valeurs[i], valeurs[i + 1]) for i in range(0, len(valeurs), 2)])
    except ValueError as e:
        print(f"Erreur : {e}. Usage : python3 carburant.py km1 l1 km2 l2 ...", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
