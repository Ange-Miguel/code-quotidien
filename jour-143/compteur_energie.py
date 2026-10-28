"""Jour 143 — Compteur d'énergie : kWh cumulés et coût en FCFA

Problème : un compteur d'énergie connecté (module PZEM-004T sur ESP32, par
exemple) mesure la puissance instantanée en watts plusieurs fois par minute.
Mais la facture se paie en kilowattheures : il faut intégrer la puissance
dans le temps, puis présenter la consommation par heure, par jour et le coût.
Solution : l'énergie est l'intégrale de la puissance ; on l'approche par la
méthode des trapèzes entre deux mesures horodatées (pas forcément
régulières : une mesure peut manquer). On cumule par tranche horaire, on
repère l'heure de pointe, on applique un tarif par tranches (jour 008) et on
projette la facture du mois. La journée de mesures est simulée.
On apprend : l'intégration numérique (méthode des trapèzes), la différence
entre puissance et énergie, l'agrégation par heure et la projection mensuelle.
"""
import math
import random
import sys
from collections import defaultdict

TRANCHES = [(20, 79), (250, 113), (math.inf, 129)]   # kWh mensuels → FCFA/kWh (exemple)


def puissance_simulee(seconde: int, rng: random.Random) -> float:
    h = seconde / 3600
    base = 120                                            # réfrigérateur, box, veilles
    if 6 <= h < 8: base += 900                            # chauffe-eau, fer, bouilloire
    if 12 <= h < 15: base += 1100                         # climatiseur
    if 19 <= h < 23: base += 650                          # lampes, télé, ventilateurs
    return max(0.0, base + rng.gauss(0, 40))


def trapezes(mesures: list[tuple[int, float]]) -> dict[int, float]:
    par_heure = defaultdict(float)
    for (t1, p1), (t2, p2) in zip(mesures, mesures[1:]):
        wh = (p1 + p2) / 2 * (t2 - t1) / 3600          # aire du trapèze en Wh
        par_heure[t1 // 3600] += wh
    return par_heure


def cout_mensuel(kwh: float) -> float:
    total, debut = 0.0, 0.0
    for limite, prix in TRANCHES:
        if kwh > debut:
            total += (min(kwh, limite) - debut) * prix
        debut = limite
    return total


def main() -> int:
    intervalle = 30
    if len(sys.argv) == 2:
        try:
            intervalle = int(sys.argv[1])
            if not 1 <= intervalle <= 3600:
                raise ValueError
        except ValueError:
            print("Usage : python3 compteur_energie.py [intervalle de mesure en s, 1..3600]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 compteur_energie.py [intervalle s]", file=sys.stderr)
        return 1
    rng = random.Random(143)
    mesures = [(t, puissance_simulee(t, rng)) for t in range(0, 86400 + 1, intervalle) if rng.random() > 0.02]  # 2 % perdues
    par_heure = trapezes(mesures)
    total_kwh = sum(par_heure.values()) / 1000
    pointe = max(par_heure, key=par_heure.get)
    print(f"{len(mesures)} mesures (une toutes les {intervalle} s, 2 % perdues) sur 24 h\n")
    for h in range(24):
        wh = par_heure[h]
        print(f"   {h:02d}h  {wh:7.0f} Wh {'▇' * round(wh / 50)}{'  ← pointe' if h == pointe else ''}")
    print(f"\nConsommation du jour : {total_kwh:.2f} kWh | puissance moyenne {total_kwh * 1000 / 24:.0f} W")
    mois = total_kwh * 30
    print(f"Projection sur 30 jours : {mois:.0f} kWh → environ {cout_mensuel(mois):,.0f} FCFA hors taxes".replace(",", " "))
    return 0


if __name__ == "__main__":
    sys.exit(main())
