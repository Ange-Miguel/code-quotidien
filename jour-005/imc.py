"""Jour 005 — Calcul de l'IMC avec interprétation

Problème : on veut savoir rapidement si son poids est adapté à sa taille, par
exemple avant de commencer un programme de sport.
Solution : IMC = poids (kg) / taille² (m). On compare le résultat aux seuils de
l'OMS et on calcule la plage de poids « normal » pour cette taille.
On apprend : les fonctions, les tuples, la recherche dans une table de seuils
et la validation de valeurs réalistes.
"""
import sys

SEUILS = [
    (16.5, "Dénutrition"),
    (18.5, "Maigreur"),
    (25.0, "Corpulence normale"),
    (30.0, "Surpoids"),
    (35.0, "Obésité modérée"),
    (40.0, "Obésité sévère"),
    (float("inf"), "Obésité morbide"),
]


def imc(poids: float, taille: float) -> float:
    if not 20 <= poids <= 400:
        raise ValueError("poids attendu entre 20 et 400 kg")
    if not 0.8 <= taille <= 2.6:
        raise ValueError("taille attendue en mètres, entre 0,80 et 2,60")
    return poids / taille ** 2


def interpretation(valeur: float) -> str:
    for limite, libelle in SEUILS:
        if valeur < limite:
            return libelle
    return SEUILS[-1][1]


def rapport(poids: float, taille: float) -> None:
    v = imc(poids, taille)
    mini, maxi = 18.5 * taille ** 2, 25 * taille ** 2
    print(f"{poids:.1f} kg pour {taille:.2f} m → IMC {v:.1f} : {interpretation(v)}")
    print(f"   Poids normal pour cette taille : {mini:.1f} à {maxi:.1f} kg")


def main() -> int:
    if len(sys.argv) == 1:
        for p, t in [(68, 1.75), (52, 1.80), (95, 1.70)]:
            rapport(p, t)
        print("\nRappel : l'IMC est un indicateur, pas un diagnostic médical.")
        return 0
    if len(sys.argv) != 3:
        print("Usage : python3 imc.py <poids_kg> <taille_m>", file=sys.stderr)
        return 1
    try:
        rapport(float(sys.argv[1].replace(",", ".")), float(sys.argv[2].replace(",", ".")))
    except ValueError as e:
        print(f"Erreur : {e}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
