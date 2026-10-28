"""Jour 200 — Bilan des 200 jours : récapitulatif et statistiques du dépôt

Problème : après 200 jours de programmes quotidiens, il est temps de faire le
bilan : combien de lignes écrites dans chaque langage, quels thèmes, quelle
progression, quels jours ont produit les plus gros programmes ? Un bon bilan
se calcule à partir des données réelles du dépôt, pas de mémoire.
Solution : le programme parcourt le dépôt (les dossiers jour-001 à jour-200
et le fichier PLAN.md), compte pour chaque jour les lignes de code utiles
(sans lignes vides ni commentaires), identifie le langage, relie chaque jour
à sa phase et produit : un tableau par langage, un histogramme par phase, le
top 5 des plus longs programmes, la progression de la taille moyenne et une
carte de 200 cases façon « graphe de contributions » GitHub.
On apprend : pathlib pour parcourir des dossiers, les expressions régulières
pour lire un fichier Markdown, le comptage de lignes de code par langage et
la présentation d'un rapport final.
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

EXTENSIONS = {".c": "C", ".py": "Python", ".java": "Java"}
COMMENTAIRES = {"C": ("//", "/*", "*", "*/"), "Java": ("//", "/*", "*", "*/"), "Python": ("#",)}
PHASES = [(1, 50, "Bases utiles"), (51, 100, "Structures et fichiers"), (101, 150, "Embarqué et IoT"), (151, 200, "Projets réels")]


def lignes_utiles(fichier: Path, langue: str) -> int:
    n, docstring = 0, False
    for ligne in fichier.read_text(encoding="utf-8", errors="replace").splitlines():
        s = ligne.strip()
        if langue == "Python" and s.startswith('"""'):
            docstring = not docstring if s.count('"""') == 1 else docstring
            continue
        if not s or docstring or s.startswith(COMMENTAIRES[langue]):
            continue
        n += 1
    return n


def trouver_depot(depart: Path) -> Path | None:
    for d in [depart, *depart.parents]:
        if (d / "PLAN.md").is_file():
            return d
    return None


def main() -> int:
    if len(sys.argv) > 2:
        print("Usage : python3 bilan.py [chemin du dépôt]", file=sys.stderr)
        return 1
    depot = Path(sys.argv[1]) if len(sys.argv) == 2 else trouver_depot(Path(__file__).resolve().parent)
    if depot is None:
        print("Dépôt introuvable : lancez ce programme depuis le dépôt code-quotidien,")
        print("ou indiquez son chemin : python3 bilan.py /chemin/vers/code-quotidien")
        return 0
    if not (depot / "PLAN.md").is_file():
        print(f"Pas de PLAN.md dans {depot}", file=sys.stderr)
        return 1
    titres = dict(re.findall(r"^\| (\d{3}) \| \[.\] \| \w+ \| (.+?) \|$", (depot / "PLAN.md").read_text(encoding="utf-8"), re.M))
    jours = {}
    for dossier in sorted(depot.glob("jour-[0-9][0-9][0-9]")):
        for f in dossier.iterdir():
            if f.suffix in EXTENSIONS:
                n = int(dossier.name[-3:])
                jours[n] = (EXTENSIONS[f.suffix], lignes_utiles(f, EXTENSIONS[f.suffix]))
    if not jours:
        print("Aucun programme trouvé.")
        return 0
    total = sum(l for _, l in jours.values())
    print(f"=== Bilan du dépôt code-quotidien : {len(jours)} jour(s) publiés, {total:_} lignes de code ===\n".replace("_", " "))
    par_langue = defaultdict(list)
    for langue, l in jours.values():
        par_langue[langue].append(l)
    print(f"{'Langage':8} {'Programmes':>10} {'Lignes':>8} {'Moyenne':>8}")
    for langue, ls in sorted(par_langue.items()):
        print(f"{langue:8} {len(ls):10d} {sum(ls):8d} {sum(ls) / len(ls):8.0f}")
    print("\nLignes par phase :")
    for debut, fin, nom in PHASES:
        l = sum(v[1] for j, v in jours.items() if debut <= j <= fin)
        print(f"   {nom:24} {l:6d} {'█' * (l // 150)}")
    print("\nLes 5 programmes les plus longs :")
    for j, (langue, l) in sorted(jours.items(), key=lambda x: -x[1][1])[:5]:
        print(f"   jour {j:03d} ({langue:6}) {l:4d} lignes — {titres.get(f'{j:03d}', '?')}")
    print("\nCarte des 200 jours (█ publié, · à venir) :")
    for ligne in range(0, 200, 40):
        print("   " + "".join("█" if j in jours else "·" for j in range(ligne + 1, ligne + 41)))
    print("\nBravo pour ces 200 jours de régularité !" if len(jours) == 200 else f"\nEncore {200 - len(jours)} jour(s) à publier.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
