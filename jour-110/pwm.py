"""Jour 110 — Simulation d'un PWM : rapport cyclique et vitesse d'un moteur

Problème : un microcontrôleur ne sait sortir que 0 V ou 5 V. Comment faire
varier la vitesse d'un ventilateur, la luminosité d'une LED ou la position
d'un servomoteur ? Avec la modulation de largeur d'impulsion (PWM) :
on alterne très vite 0 et 5 V, et c'est la proportion du temps à 5 V
(le rapport cyclique) qui compte.
Solution : on génère le signal PWM pour plusieurs rapports cycliques, on
calcule la tension moyenne (rapport × 5 V), puis on simule un moteur à courant
continu (modèle du premier ordre : il « lisse » le signal grâce à son
inertie) et on observe sa vitesse se stabiliser. On calcule aussi les
registres d'un timer (prédiviseur, valeur TOP) pour obtenir une fréquence
donnée sur un microcontrôleur à 16 MHz.
On apprend : le PWM, le rapport cyclique, la tension moyenne, le filtrage
naturel par l'inertie et le réglage des timers d'un microcontrôleur.
"""
import sys

VCC = 5.0
F_CPU = 16_000_000


def signal_pwm(rapport: float, periodes: int = 2, pas: int = 20) -> str:
    haut = round(rapport * pas)
    return ("█" * haut + "_" * (pas - haut)) * periodes


def vitesse_moteur(rapport: float, periodes: int = 60, pas: int = 50, tau: float = 300.0) -> list[float]:
    """Vitesse en tr/min d'un moteur (max 3000 à 5 V), modèle du premier ordre."""
    v, vitesses = 0.0, []
    for _ in range(periodes):
        for k in range(pas):
            tension = VCC if k < rapport * pas else 0.0
            v += (tension / VCC * 3000 - v) / tau
        vitesses.append(v)
    return vitesses


def reglage_timer(frequence: float) -> tuple[int, int, float]:
    """Mode Fast PWM 16 bits : f = F_CPU / (prediv × (TOP + 1))."""
    for prediv in (1, 8, 64, 256, 1024):
        top = round(F_CPU / (prediv * frequence)) - 1
        if 0 < top <= 65535:
            return prediv, top, F_CPU / (prediv * (top + 1))
    raise ValueError("fréquence impossible avec ce timer")


def main() -> int:
    rapports = [0.0, 0.25, 0.5, 0.75, 1.0]
    if len(sys.argv) == 2:
        try:
            r = float(sys.argv[1]) / 100
            if not 0 <= r <= 1:
                raise ValueError
            rapports = [r]
        except ValueError:
            print("Usage : python3 pwm.py [rapport cyclique en %, 0..100]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 pwm.py [rapport %]", file=sys.stderr)
        return 1
    print(f"{'Rapport':>7}  {'Signal (2 périodes)':40}  {'V moy':>6}  {'Vitesse':>8}")
    for r in rapports:
        v = vitesse_moteur(r)
        print(f"{r:7.0%}  {signal_pwm(r):40}  {r * VCC:5.2f}V  {v[-1]:6.0f} tr/min")
    v = vitesse_moteur(0.5)
    print("\nDémarrage à 50 % (le moteur lisse le signal grâce à son inertie) :")
    for i in range(0, 60, 8):
        print(f"   période {i:2d} : {v[i]:6.0f} tr/min {'▇' * int(v[i] / 100)}")
    print("\nRéglage du timer 1 (16 bits) d'un microcontrôleur à 16 MHz :")
    for f in (50, 1000, 20_000):
        prediv, top, reel = reglage_timer(f)
        print(f"   {f:>6} Hz → prédiviseur {prediv:4d}, TOP = {top:5d} → {reel:.2f} Hz")
    print("   (50 Hz est la fréquence des servomoteurs ; 20 kHz est inaudible pour un moteur)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
