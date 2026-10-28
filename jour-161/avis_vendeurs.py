"""Jour 161 — Système de notation et avis de vendeurs

Problème : sur une marketplace, un vendeur avec une seule note de 5/5 apparaît
« meilleur » qu'un vendeur noté 4,7/5 sur 300 ventes. C'est trompeur : peu
d'avis donnent peu de certitude. Et de faux avis groupés peuvent gonfler une
note.
Solution : on classe les vendeurs avec une moyenne bayésienne : on mélange
la moyenne du vendeur avec la moyenne générale du site, comme si chaque
vendeur avait reçu C avis « moyens » d'office. Avec beaucoup d'avis, sa vraie
moyenne domine ; avec peu, il reste près de la moyenne du site. On donne
plus de poids aux avis récents et vérifiés (achat confirmé), et on signale
les rafales d'avis 5/5 venant de comptes tout neufs. Données fictives.
On apprend : la moyenne bayésienne, la pondération des données, la
détection simple de fraude et le tri par score composite.
"""
import sys
from collections import defaultdict
from dataclasses import dataclass


@dataclass
class Avis:
    vendeur: str
    note: int
    jours: int            # ancienneté de l'avis
    verifie: bool         # achat confirmé sur la plateforme
    age_compte: int       # ancienneté du compte de l'acheteur, en jours


def poids(a: Avis) -> float:
    recence = 1.0 if a.jours < 90 else 0.6 if a.jours < 365 else 0.3
    return recence * (1.0 if a.verifie else 0.4)


def generer() -> list[Avis]:
    avis = []
    for i in range(300):
        avis.append(Avis("Boutique Kpota", 5 if i % 10 < 7 else 4 if i % 10 < 9 else 2, i % 400, True, 400))
    avis.append(Avis("Tech Express", 5, 3, True, 200))
    for i in range(40):
        avis.append(Avis("Phone Calavi", [5, 4, 4, 5, 3][i % 5], i * 8, i % 3 != 0, 300))
    for i in range(25):                      # rafale suspecte : 5/5 de comptes créés la veille
        avis.append(Avis("Super Deal", 5, 1, False, 1))
    for i in range(6):
        avis.append(Avis("Super Deal", [1, 2, 1, 3, 2, 1][i], 30 + i, True, 500))
    return avis


def classement(avis: list[Avis], c: float = 20) -> list[tuple]:
    par_vendeur = defaultdict(list)
    for a in avis:
        par_vendeur[a.vendeur].append(a)
    moyenne_site = sum(a.note * poids(a) for a in avis) / sum(poids(a) for a in avis)
    resultats = []
    for v, liste in par_vendeur.items():
        suspects = [a for a in liste if a.age_compte < 7 and not a.verifie and a.note == 5]
        fiables = [a for a in liste if a not in suspects]
        w = sum(poids(a) for a in fiables)
        brute = sum(a.note for a in liste) / len(liste)
        bayes = (c * moyenne_site + sum(a.note * poids(a) for a in fiables)) / (c + w)
        resultats.append((bayes, v, brute, len(liste), len(suspects)))
    return sorted(resultats, reverse=True), moyenne_site


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    res, moyenne_site = classement(generer())
    print(f"Moyenne pondérée du site : {moyenne_site:.2f}/5\n")
    print(f"{'Rang':4} {'Vendeur':15} {'Score':>6} {'Moy. brute':>11} {'Avis':>5}  Remarque")
    for rang, (score, v, brute, n, suspects) in enumerate(res, 1):
        remarque = f"⚠ {suspects} avis suspects ignorés" if suspects else ("peu d'avis : score prudent" if n < 5 else "")
        print(f"{rang:4} {v:15} {score:6.2f} {brute:11.2f} {n:5d}  {remarque}")
    print("\nTri naïf par moyenne brute :", ", ".join(v for _, v, *_ in sorted(res, key=lambda r: -r[2])))
    return 0


if __name__ == "__main__":
    sys.exit(main())
