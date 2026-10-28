"""Jour 182 — Emploi du temps d'examens sans conflit (coloration de graphe)

Problème : à la fin du semestre, il faut programmer les examens de toutes
les matières. Deux matières suivies par un même étudiant ne peuvent pas avoir
lieu sur le même créneau. Combien de créneaux faut-il au minimum, et comment
les répartir ?
Solution : c'est un problème de coloration de graphe. Chaque matière est un
sommet ; une arête relie deux matières qui ont au moins un étudiant en
commun. Colorier le graphe (deux sommets reliés ont des couleurs
différentes) revient à attribuer des créneaux. On utilise l'algorithme DSatur
(on colorie d'abord le sommet qui a le plus de couleurs différentes chez ses
voisins), généralement meilleur que le glouton simple, et on vérifie que
l'emploi du temps est valide. Inscriptions fictives.
On apprend : la modélisation par un graphe de conflits, la coloration
gloutonne et DSatur, et la vérification d'une solution.
"""
import itertools
import sys
from collections import defaultdict

INSCRIPTIONS = {
    "Ange": ["Électronique", "Prog. C", "Maths", "Réseaux"], "Awa": ["Électronique", "Maths", "Anglais"],
    "Kofi": ["Prog. C", "Réseaux", "Bases de données"], "Inès": ["Maths", "Statistiques", "Anglais"],
    "Serge": ["Bases de données", "Statistiques", "Gestion"], "Carine": ["Réseaux", "Gestion", "Anglais"],
    "Yves": ["Électronique", "Microcontrôleurs", "Prog. C"], "Paul": ["Microcontrôleurs", "Réseaux", "Gestion"],
}
CRENEAUX = ["lun 8h", "lun 14h", "mar 8h", "mar 14h", "mer 8h", "mer 14h", "jeu 8h"]


def graphe_conflits(inscriptions):
    voisins = defaultdict(set)
    for matieres in inscriptions.values():
        for a, b in itertools.combinations(matieres, 2):
            voisins[a].add(b); voisins[b].add(a)
        for m in matieres:
            voisins[m]
    return voisins


def glouton(voisins, ordre) -> dict[str, int]:
    couleur = {}
    for s in ordre:
        prises = {couleur[v] for v in voisins[s] if v in couleur}
        couleur[s] = next(c for c in itertools.count() if c not in prises)
    return couleur


def dsatur(voisins) -> dict[str, int]:
    couleur = {}
    while len(couleur) < len(voisins):
        restants = [s for s in voisins if s not in couleur]
        s = max(restants, key=lambda x: (len({couleur[v] for v in voisins[x] if v in couleur}), len(voisins[x]), x))
        prises = {couleur[v] for v in voisins[s] if v in couleur}
        couleur[s] = next(c for c in itertools.count() if c not in prises)
    return couleur


def valide(voisins, couleur) -> bool:
    return all(couleur[a] != couleur[b] for a in voisins for b in voisins[a])


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    v = graphe_conflits(INSCRIPTIONS)
    print(f"{len(v)} matières, {sum(len(x) for x in v.values()) // 2} paires en conflit")
    for m in sorted(v):
        print(f"   {m:17} incompatible avec : {', '.join(sorted(v[m]))}")
    g = glouton(v, sorted(v))
    d = dsatur(v)
    assert valide(v, g) and valide(v, d)
    print(f"\nGlouton (ordre alphabétique) : {max(g.values()) + 1} créneaux | DSatur : {max(d.values()) + 1} créneaux\n")
    print("Emploi du temps des examens (DSatur) :")
    for c in range(max(d.values()) + 1):
        print(f"   {CRENEAUX[c]:8} : {', '.join(sorted(m for m in d if d[m] == c))}")
    for etudiant, matieres in INSCRIPTIONS.items():
        assert len({d[m] for m in matieres}) == len(matieres), etudiant
    print("\nVérifié : aucun étudiant n'a deux examens sur le même créneau.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
