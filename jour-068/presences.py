"""Jour 068 — Suivi des présences d'une classe

Problème : un délégué ou un enseignant note les présences sur papier ; à la
fin du semestre, impossible de savoir rapidement qui a dépassé le nombre
d'absences autorisé ou quels jours la classe est la plus vide.
Solution : les présences sont stockées dans une matrice étudiants × séances
(P = présent, A = absent, R = retard, J = absence justifiée). On calcule le
taux de présence par étudiant et par séance, on alerte au-delà de 20 %
d'absences non justifiées, et on trouve le jour de la semaine le plus touché.
On apprend : les dictionnaires de listes, les compréhensions, Counter, le
regroupement par jour de la semaine et la production d'alertes.
"""
import random
import sys
from collections import Counter
from datetime import date, timedelta

JOURS = ["lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi", "dimanche"]
SEUIL_ALERTE = 0.20


def generer(etudiants: list[str], seances: list[date], rng: random.Random) -> dict[str, list[str]]:
    profils = {nom: rng.uniform(0.02, 0.3) for nom in etudiants}
    registre = {}
    for nom in etudiants:
        ligne = []
        for s in seances:
            p = profils[nom] * (1.8 if s.weekday() == 4 else 1)   # le vendredi est plus touché
            r = rng.random()
            ligne.append("A" if r < p else "J" if r < p + 0.03 else "R" if r < p + 0.1 else "P")
        registre[nom] = ligne
    return registre


def rapport(registre: dict[str, list[str]], seances: list[date]) -> None:
    n = len(seances)
    print(f"{'Étudiant':18} {'Prés.':>6} {'Abs.':>5} {'Just.':>5} {'Ret.':>5}  Taux")
    for nom, ligne in sorted(registre.items()):
        c = Counter(ligne)
        taux_abs = c["A"] / n
        alerte = "  ⚠ dépasse 20 % d'absences" if taux_abs > SEUIL_ALERTE else ""
        print(f"{nom:18} {c['P'] + c['R']:6d} {c['A']:5d} {c['J']:5d} {c['R']:5d}  {1 - taux_abs:5.0%}{alerte}")
    absences_par_jour = Counter()
    seances_par_jour = Counter(s.weekday() for s in seances)
    for ligne in registre.values():
        for s, etat in zip(seances, ligne):
            if etat == "A":
                absences_par_jour[s.weekday()] += 1
    print("\nAbsences moyennes par séance selon le jour :")
    for j in sorted(seances_par_jour):
        moyenne = absences_par_jour[j] / seances_par_jour[j]
        print(f"   {JOURS[j]:9} {moyenne:4.1f} {'▇' * round(moyenne * 4)}")
    pire = max(seances_par_jour, key=lambda j: absences_par_jour[j] / seances_par_jour[j])
    print(f"Jour le plus touché : {JOURS[pire]}")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    rng = random.Random(68)
    etudiants = ["Adjovi Mireille", "Agossa Inès", "Akpanon Ange", "Dossou Carine", "Houngbo Serge",
                 "Kpade Yves", "Zinsou Paul"]
    debut = date(2026, 9, 7)
    seances = [debut + timedelta(days=d) for d in range(56) if (debut + timedelta(days=d)).weekday() in (0, 2, 4)]
    print(f"{len(seances)} séances (lundi, mercredi, vendredi) du {seances[0]} au {seances[-1]}\n")
    rapport(generer(etudiants, seances, rng), seances)
    return 0


if __name__ == "__main__":
    sys.exit(main())
