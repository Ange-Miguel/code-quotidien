"""Jour 152 — Covoiturage étudiant : appariement passager / conducteur

Problème : chaque matin, des étudiants de Godomey, Calavi ou Cotonou vont au
campus. Certains ont une moto et une place libre, d'autres paient un zem.
Comment former automatiquement les meilleures paires conducteur / passager,
en respectant les horaires et sans faire de grands détours ?
Solution : pour chaque paire possible, on calcule un « coût » : détour du
conducteur pour aller chercher le passager (distance GPS), écart entre les
heures souhaitées ; les paires impossibles (détour > 2 km, écart > 20 min)
sont exclues. Puis on cherche l'affectation qui minimise le coût total : un
algorithme glouton (meilleure paire d'abord) puis une amélioration par
échanges deux à deux, et on compare à la solution exacte par force brute.
Positions et horaires sont fictifs.
On apprend : la modélisation d'un problème d'affectation, une fonction de
coût, le glouton, la recherche locale par échanges et itertools.permutations.
"""
import itertools
import math
import sys

CAMPUS = (6.4170, 2.3427)


def distance(a, b) -> float:
    la1, lo1, la2, lo2 = map(math.radians, (*a, *b))
    h = math.sin((la2 - la1) / 2) ** 2 + math.cos(la1) * math.cos(la2) * math.sin((lo2 - lo1) / 2) ** 2
    return 2 * 6371 * math.asin(math.sqrt(h))


CONDUCTEURS = {"Ange": ((6.3912, 2.3301), 7 * 60 + 30), "Kofi": ((6.4527, 2.3496), 7 * 60 + 45),
               "Serge": ((6.4485, 2.3557), 7 * 60 + 15), "Yves": ((6.3700, 2.3900), 7 * 60 + 40)}
PASSAGERS = {"Awa": ((6.3950, 2.3350), 7 * 60 + 35), "Inès": ((6.4470, 2.3530), 7 * 60 + 20),
             "Carine": ((6.4500, 2.3480), 7 * 60 + 50), "Paul": ((6.3740, 2.3850), 7 * 60 + 30)}


def cout(conducteur: str, passager: str) -> float | None:
    (pc, hc), (pp, hp) = CONDUCTEURS[conducteur], PASSAGERS[passager]
    detour = distance(pc, pp) + distance(pp, CAMPUS) - distance(pc, CAMPUS)
    ecart = abs(hc - hp)
    if detour > 2 or ecart > 20:
        return None
    return detour * 10 + ecart                       # 1 km de détour « vaut » 10 min d'écart


def total(paires) -> float:
    return sum(cout(c, p) for c, p in paires)


def glouton() -> list[tuple[str, str]]:
    possibles = sorted((cout(c, p), c, p) for c in CONDUCTEURS for p in PASSAGERS if cout(c, p) is not None)
    pris_c, pris_p, paires = set(), set(), []
    for _, c, p in possibles:
        if c not in pris_c and p not in pris_p:
            paires.append((c, p)); pris_c.add(c); pris_p.add(p)
    return paires


def ameliorer(paires: list[tuple[str, str]]) -> list[tuple[str, str]]:
    ameliore = True
    while ameliore:
        ameliore = False
        for i, j in itertools.combinations(range(len(paires)), 2):
            (c1, p1), (c2, p2) = paires[i], paires[j]
            if cout(c1, p2) is not None and cout(c2, p1) is not None and cout(c1, p2) + cout(c2, p1) < cout(c1, p1) + cout(c2, p2) - 1e-9:
                paires[i], paires[j] = (c1, p2), (c2, p1)
                ameliore = True
    return paires


def optimal() -> list[tuple[str, str]]:
    meilleur, meilleures = math.inf, []
    conducteurs = list(CONDUCTEURS)
    for ordre in itertools.permutations(PASSAGERS):
        paires = [(c, p) for c, p in zip(conducteurs, ordre) if cout(c, p) is not None]
        score = (-len(paires), total(paires))           # d'abord le plus de paires, puis le moindre coût
        if score < (-len(meilleures), meilleur):
            meilleur, meilleures = total(paires), paires
    return meilleures


def afficher(titre: str, paires) -> None:
    print(f"{titre} : {len(paires)} paire(s), coût total {total(paires):.1f}")
    for c, p in sorted(paires):
        (pc, hc), (pp, hp) = CONDUCTEURS[c], PASSAGERS[p]
        detour = distance(pc, pp) + distance(pp, CAMPUS) - distance(pc, CAMPUS)
        print(f"   {c:6} prend {p:7} détour {detour * 1000:5.0f} m, départ {hc // 60}h{hc % 60:02d} (souhait {hp // 60}h{hp % 60:02d})")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    g = glouton()
    afficher("Glouton", g)
    afficher("Glouton + échanges", ameliorer(list(g)))
    afficher("Optimal (force brute)", optimal())
    return 0


if __name__ == "__main__":
    sys.exit(main())
