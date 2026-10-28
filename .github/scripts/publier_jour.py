#!/usr/bin/env python3
"""Publie le prochain jour du plan : copie jour-NNN depuis la branche `reserve`,
coche la ligne dans PLAN.md et prépare le commit.

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
    with open("PLAN.md", "w", encoding="utf-8") as f:
        f.write("\n".join(lignes))

    sortie("jour", jour)
    sortie("titre", titre)
    return 0


if __name__ == "__main__":
    sys.exit(main())
