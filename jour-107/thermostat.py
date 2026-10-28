"""Jour 107 — Simulation d'un thermostat avec hystérésis

Problème : un thermostat simple allume la climatisation dès que la
température dépasse 26 °C et l'éteint dès qu'elle passe en dessous. Résultat :
autour de 26 °C, le compresseur s'allume et s'éteint des dizaines de fois par
heure (« battement »), ce qui l'use et consomme beaucoup.
Solution : l'hystérésis : deux seuils au lieu d'un. On allume au-dessus de
26,5 °C et on n'éteint qu'en dessous de 25,5 °C. Entre les deux, on garde
l'état précédent. On simule une pièce (chaleur extérieure, refroidissement
par le climatiseur, bruit du capteur) et on compte les commutations et
l'énergie consommée avec et sans hystérésis.
On apprend : l'hystérésis, la simulation physique pas à pas (modèle
thermique simple), l'effet du bruit sur une décision à seuil et la
comparaison de deux stratégies de commande.
"""
import random
import sys


def simuler(bande: float, minutes: int = 600, consigne: float = 26.0, seed: int = 107) -> dict:
    rng = random.Random(seed)
    temp, clim, commutations, minutes_clim = 29.0, False, 0, 0
    historique = []
    for t in range(minutes):
        exterieur = 32 + 3 * ((t % 600) / 600)                    # l'après-midi se réchauffe
        temp += 0.01 * (exterieur - temp) - (0.12 if clim else 0)  # échanges thermiques + clim
        mesure = temp + rng.gauss(0, 0.15)                         # capteur bruité
        if not clim and mesure > consigne + bande / 2:
            clim, commutations = True, commutations + 1
        elif clim and mesure < consigne - bande / 2:
            clim, commutations = False, commutations + 1
        minutes_clim += clim
        historique.append((t, temp, clim))
    return {"commutations": commutations, "minutes_clim": minutes_clim, "historique": historique,
            "min": min(h[1] for h in historique[60:]), "max": max(h[1] for h in historique[60:])}


def main() -> int:
    bandes = [0.0, 0.5, 1.0, 2.0]
    if len(sys.argv) == 2:
        try:
            b = float(sys.argv[1])
            if not 0 <= b <= 5:
                raise ValueError
            bandes = [b]
        except ValueError:
            print("Usage : python3 thermostat.py [largeur d'hystérésis en °C, 0..5]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 thermostat.py [hystérésis]", file=sys.stderr)
        return 1
    print("Consigne 26 °C, 10 heures simulées, capteur bruité (±0,15 °C)\n")
    print(f"{'Hystérésis':>10} {'Commutations':>13} {'Clim (h)':>9} {'T min':>7} {'T max':>7}")
    for b in bandes:
        r = simuler(b)
        print(f"{b:8.1f} °C {r['commutations']:13d} {r['minutes_clim'] / 60:9.1f} {r['min']:7.2f} {r['max']:7.2f}")
    r = simuler(1.0)
    print("\nExtrait avec hystérésis de 1 °C (█ = clim allumée) :")
    for t, temp, clim in r["historique"][120:180:4]:
        print(f"   {t // 60:02d}h{t % 60:02d}  {temp:5.2f} °C  {'█' if clim else '·'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
