"""Jour 197 — Simulation de trafic à un carrefour

Problème : à un carrefour encombré (avec beaucoup de motos et de voitures),
les feux ont des durées fixes : la rue principale attend parfois au rouge
alors que la rue secondaire est vide. Peut-on réduire l'attente avec des
feux « intelligents » qui s'adaptent au trafic, grâce à des capteurs ?
Solution : simulation seconde par seconde. Les véhicules arrivent au hasard
(processus de Poisson) sur chaque voie avec un débit différent ; au vert,
un véhicule passe toutes les 2 secondes. Trois stratégies : cycle fixe
30 s / 30 s, cycle proportionnel au trafic moyen, et feu adaptatif qui donne
le vert à la file la plus longue (avec un vert minimal de 10 s et un
maximal de 60 s, plus 3 s d'orange). On mesure l'attente moyenne et la
longueur maximale des files.
On apprend : la simulation à pas de temps, les arrivées aléatoires
(processus de Poisson), les files d'attente et la comparaison de stratégies
de commande.
"""
import random
import sys
from collections import deque

DUREE = 2 * 3600            # 2 heures de pointe
DEBITS = {"Nord-Sud (principale)": 0.30, "Est-Ouest (secondaire)": 0.10}   # véhicules par seconde


def simuler(strategie: str, graine: int = 197) -> dict:
    rng = random.Random(graine)
    files = {v: deque() for v in DEBITS}
    voies = list(DEBITS)
    vert, depuis, orange, prochain_depart = 0, 0, 0, 0
    attentes, maxi = [], {v: 0 for v in voies}
    for t in range(DUREE):
        for v, debit in DEBITS.items():
            if rng.random() < debit:
                files[v].append(t)
        if orange:
            orange -= 1
            if orange == 0:
                vert, depuis = 1 - vert, 0
        else:
            depuis += 1
            if strategie == "fixe":
                changer = depuis >= 30
            elif strategie == "proportionnel":
                changer = depuis >= (45 if vert == 0 else 15)
            else:   # adaptatif
                autre = 1 - vert
                changer = depuis >= 60 or (depuis >= 10 and len(files[voies[autre]]) > len(files[voies[vert]]) + 2)
            if changer:
                orange = 3
            elif files[voies[vert]] and t >= prochain_depart:
                attentes.append(t - files[voies[vert]].popleft())
                prochain_depart = t + 2
        for v in voies:
            maxi[v] = max(maxi[v], len(files[v]))
    return {"attente": sum(attentes) / len(attentes), "max": maxi, "passes": len(attentes)}


def main() -> int:
    graine = 197
    if len(sys.argv) == 2:
        try:
            graine = int(sys.argv[1])
        except ValueError:
            print("Usage : python3 trafic_carrefour.py [graine]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 trafic_carrefour.py [graine]", file=sys.stderr)
        return 1
    print("2 h de pointe : " + ", ".join(f"{v} {d * 3600:.0f} véh/h" for v, d in DEBITS.items()) + "\n")
    print(f"{'Stratégie':28} {'Attente moy.':>12} {'File max N-S':>13} {'File max E-O':>13} {'Passés':>7}")
    noms = {"fixe": "Cycle fixe 30 s / 30 s", "proportionnel": "Cycle proportionnel 45/15", "adaptatif": "Feu adaptatif (capteurs)"}
    for s, nom in noms.items():
        r = simuler(s, graine)
        m = list(r["max"].values())
        print(f"{nom:28} {r['attente']:10.0f} s {m[0]:13d} {m[1]:13d} {r['passes']:7d}")
    print("\nLe feu adaptatif donne le vert là où les véhicules attendent vraiment.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
