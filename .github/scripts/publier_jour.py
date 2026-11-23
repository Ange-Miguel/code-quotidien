#!/usr/bin/env python3
"""Publie le prochain jour du plan : copie jour-NNN depuis la branche `reserve`,
le construit en plusieurs commits progressifs (le programme bloc par bloc, puis
le README section par section), et coche la ligne dans PLAN.md au dernier commit,
nommé « Jour NNN : titre ».

Sortie (pour GitHub Actions) : écrit `jour=NNN` et `titre=...` dans $GITHUB_OUTPUT,
ou rien si aucun jour n'est à publier aujourd'hui.
"""
import os
import re
import subprocess
import sys
from datetime import datetime, timedelta, timezone

FUSEAU_BENIN = timezone(timedelta(hours=1))  # Africa/Porto-Novo, pas d'heure d'été
LIGNE = re.compile(r"^\| (\d{3}) \| \[( |x)\] \| (\w+) \| (.+?) \|$")
NB_COMMITS = int(os.environ.get("NB_COMMITS", "12"))  # commits visés par jour

FONCTION = [
    re.compile(r"^\s*def\s+(\w+)\s*\("),                                   # Python
    re.compile(r"^\s*(?:public|private|protected|static|final|\s)*class\s+(\w+)"),  # Java
    re.compile(r"^\s*(?:public|private|protected)?\s*(?:static\s+)?[\w<>\[\]]+\s+(\w+)\s*\([^;]*$"),  # Java/C
    re.compile(r"^(?:static\s+)?[\w\*\s]+?\b(\w+)\s*\([^;]*\)\s*\{?\s*$"),  # C
]
MOTS_CLES = {"if", "for", "while", "switch", "return", "else", "do", "catch"}


def git(*args: str) -> str:
    return subprocess.run(["git", *args], check=True, capture_output=True, text=True).stdout.strip()


def sortie(cle: str, valeur: str) -> None:
    chemin = os.environ.get("GITHUB_OUTPUT")
    if chemin:
        with open(chemin, "a", encoding="utf-8") as f:
            f.write(f"{cle}={valeur}\n")
    print(f"{cle}={valeur}")


def deja_publie_aujourdhui() -> bool:
    """Vrai si le dernier commit « Jour NNN » date d'aujourd'hui (heure du Bénin)."""
    log = git("log", "-50", "--format=%ct %s")
    aujourdhui = datetime.now(FUSEAU_BENIN).date()
    for ligne in log.splitlines():
        horodatage, sujet = ligne.split(" ", 1)
        if sujet.startswith("Jour "):
            return datetime.fromtimestamp(int(horodatage), FUSEAU_BENIN).date() == aujourdhui
    return False


def blocs(texte: str) -> list[str]:
    """Découpe un fichier source en blocs séparés par des lignes vides (au niveau racine)."""
    morceaux, courant = [], []
    for ligne in texte.split("\n"):
        courant.append(ligne)
        if ligne.strip() == "" and any(l.strip() for l in courant):
            morceaux.append("\n".join(courant) + "\n")
            courant = []
    if any(l.strip() for l in courant):
        morceaux.append("\n".join(courant))
    elif courant and morceaux:
        morceaux[-1] += "\n".join(courant)
    return morceaux


def sections_readme(texte: str) -> list[tuple[str, str]]:
    """Découpe le README en (titre, contenu) sur les titres « ## »."""
    parties, titre, courant = [], "titre et présentation", []
    for ligne in texte.split("\n"):
        if ligne.startswith("## ") and courant:
            parties.append((titre, "\n".join(courant) + "\n"))
            titre, courant = ligne[3:].strip(), []
        courant.append(ligne)
    parties.append((titre, "\n".join(courant)))
    return parties


def decrire(bloc: str, nom_fichier: str) -> str:
    noms = []
    for ligne in bloc.split("\n"):
        brut = ligne.strip()
        if any(m in ligne.split("(")[0].split() for m in MOTS_CLES):
            continue
        if brut.startswith(("#", "//", "*", "/*")) or any(x in brut for x in ("=", "raise ", "print", "new ", "throw ")):
            continue
        if not brut.startswith("def ") and not brut.endswith(("{", ")", ":")):
            continue
        if len(ligne) - len(ligne.lstrip()) > 4:  # seulement les définitions de premier niveau
            continue
        for motif in FONCTION:
            m = motif.match(ligne)
            if m and m.group(1) not in MOTS_CLES and m.group(1) not in noms:
                noms.append(m.group(1))
                break
    if noms:
        return f"{nom_fichier} : " + ", ".join(noms[:3])
    premiere = next((l.strip() for l in bloc.split("\n") if l.strip()), "")
    if premiere.startswith(("#include", "import", "from ", "package")):
        return f"{nom_fichier} : imports et en-têtes"
    if premiere.startswith(("/*", "//", "#", '"""', "*")):
        return f"{nom_fichier} : commentaires et constantes"
    return f"{nom_fichier} : suite du programme"


def regrouper(unites: list, cible: int) -> list[list]:
    """Regroupe des unités consécutives en au plus `cible` groupes de taille proche."""
    if cible <= 0:
        return []
    if len(unites) <= cible:
        return [[u] for u in unites]
    groupes, n = [], len(unites)
    for k in range(cible):
        debut, fin = k * n // cible, (k + 1) * n // cible
        groupes.append(unites[debut:fin])
    return groupes


def commit(message: str, *chemins: str) -> None:
    git("add", *chemins)
    git("commit", "-q", "-m", message)


def publier_progressivement(jour: str, titre: str, dossier: str, plan_final: str) -> None:
    fichiers = sorted(os.listdir(dossier))
    readme = os.path.join(dossier, "README.md")
    sources = [f for f in fichiers if f != "README.md"]
    contenus = {f: open(os.path.join(dossier, f), encoding="utf-8").read() for f in fichiers}
    for f in fichiers:  # on repart d'un dossier vide pour le reconstruire
        os.remove(os.path.join(dossier, f))

    unites_src = [(f, b) for f in sources for b in blocs(contenus[f])]
    unites_doc = sections_readme(contenus["README.md"]) if "README.md" in contenus else []
    total = NB_COMMITS - 1  # le dernier commit est « Jour NNN : titre »
    part_doc = min(len(unites_doc) - 1, total // 2) if len(unites_doc) > 1 else 0
    part_src = max(1, total - part_doc)
    # Pas assez de blocs : on coupe en deux le plus long bloc (sur une fin de ligne).
    while len(unites_src) < part_src:
        idx = max(range(len(unites_src)), key=lambda j: unites_src[j][1].count("\n"))
        f, b = unites_src[idx]
        lignes_b = b.split("\n")
        if len(lignes_b) < 4:
            break
        milieu = len(lignes_b) // 2
        unites_src[idx:idx + 1] = [(f, "\n".join(lignes_b[:milieu]) + "\n"), (f, "\n".join(lignes_b[milieu:]))]
    etapes = [("src", g) for g in regrouper(unites_src, part_src)]
    etapes += [("doc", g) for g in regrouper(unites_doc[:-1], part_doc)]
    nb = len(etapes) + 1

    deja = {f: "" for f in fichiers}
    for k, (genre, groupe) in enumerate(etapes, 1):
        if genre == "src":
            for f, b in groupe:
                deja[f] += b
            touches = sorted({f for f, _ in groupe})
            for f in touches:
                with open(os.path.join(dossier, f), "w", encoding="utf-8") as h:
                    h.write(deja[f])
            desc = decrire("".join(b for _, b in groupe), groupe[0][0])
            commit(f"Jour {jour} · étape {k}/{nb} : {desc}", *[os.path.join(dossier, f) for f in touches])
        else:
            for _, contenu in groupe:
                deja["README.md"] += contenu
            with open(readme, "w", encoding="utf-8") as h:
                h.write(deja["README.md"])
            desc = ", ".join(t for t, _ in groupe)
            commit(f"Jour {jour} · étape {k}/{nb} : README — {desc}", readme)

    for f in fichiers:  # état final exact, identique à la réserve
        with open(os.path.join(dossier, f), "w", encoding="utf-8") as h:
            h.write(contenus[f])
    with open("PLAN.md", "w", encoding="utf-8") as h:
        h.write(plan_final)
    commit(f"Jour {jour} : {titre}", dossier, "PLAN.md")


def main() -> int:
    forcer = os.environ.get("FORCER", "false") == "true"
    if deja_publie_aujourdhui() and not forcer:
        print("Un jour a déjà été publié aujourd'hui : rien à faire.")
        return 0

    with open("PLAN.md", encoding="utf-8") as f:
        lignes = f.read().split("\n")

    for i, ligne in enumerate(lignes):
        m = LIGNE.match(ligne)
        if m and m.group(2) == " ":
            jour, titre = m.group(1), m.group(4)
            break
    else:
        print("Les 200 jours sont publiés. Bravo !")
        return 0

    dossier = f"jour-{jour}"
    try:
        git("checkout", "origin/reserve", "--", dossier)
    except subprocess.CalledProcessError:
        print(f"Le dossier {dossier} est absent de la branche reserve.", file=sys.stderr)
        return 1

    lignes[i] = ligne.replace("| [ ] |", "| [x] |", 1)
    git("reset", "-q")  # le checkout ci-dessus a indexé le dossier : on désindexe
    publier_progressivement(jour, titre, dossier, "\n".join(lignes))

    sortie("jour", jour)
    sortie("titre", titre)
    return 0


if __name__ == "__main__":
    sys.exit(main())
