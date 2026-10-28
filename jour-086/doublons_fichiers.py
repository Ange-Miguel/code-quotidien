"""Jour 086 — Recherche de fichiers en double par empreinte

Problème : le téléphone ou l'ordinateur est plein, souvent à cause des mêmes
photos et vidéos reçues plusieurs fois sur WhatsApp, sous des noms
différents. Comparer les fichiers deux à deux serait trop long.
Solution : en trois étapes de plus en plus coûteuses : (1) regrouper par
taille (deux fichiers de tailles différentes ne peuvent pas être identiques) ;
(2) dans chaque groupe, comparer l'empreinte des 4 premiers Ko ; (3) pour les
candidats restants, calculer l'empreinte SHA-256 complète. On affiche les
doublons et la place récupérable. Rien n'est supprimé.
On apprend : os.walk, hashlib, la lecture par blocs de gros fichiers, et le
filtrage progressif pour éviter des calculs inutiles.
"""
import hashlib
import os
import sys
import tempfile
from collections import defaultdict


def empreinte(chemin: str, limite: int | None = None) -> str:
    h = hashlib.sha256()
    with open(chemin, "rb") as f:
        lu = 0
        while bloc := f.read(65536 if limite is None else min(65536, limite - lu)):
            h.update(bloc)
            lu += len(bloc)
            if limite is not None and lu >= limite:
                break
    return h.hexdigest()


def trouver_doublons(dossier: str) -> list[list[str]]:
    par_taille = defaultdict(list)
    for racine, _, fichiers in os.walk(dossier):
        for nom in fichiers:
            chemin = os.path.join(racine, nom)
            if os.path.isfile(chemin) and not os.path.islink(chemin):
                par_taille[os.path.getsize(chemin)].append(chemin)
    groupes = []
    for taille, chemins in par_taille.items():
        if len(chemins) < 2:
            continue
        par_debut = defaultdict(list)
        for c in chemins:
            par_debut[empreinte(c, 4096)].append(c)
        for candidats in par_debut.values():
            if len(candidats) < 2:
                continue
            par_complet = defaultdict(list)
            for c in candidats:
                par_complet[empreinte(c)].append(c)
            groupes.extend(sorted(g) for g in par_complet.values() if len(g) > 1)
    return groupes


def rapport(dossier: str) -> None:
    groupes = trouver_doublons(dossier)
    gain = 0
    for g in groupes:
        taille = os.path.getsize(g[0])
        gain += taille * (len(g) - 1)
        print(f"{len(g)} copies identiques ({taille:,} octets) :".replace(",", " "))
        for c in g:
            print(f"   {os.path.relpath(c, dossier)}")
    print(f"\n{len(groupes)} groupe(s) de doublons, {gain:,} octets récupérables (rien n'a été supprimé)".replace(",", " "))


def main() -> int:
    if len(sys.argv) == 2:
        if not os.path.isdir(sys.argv[1]):
            print(f"Dossier introuvable : {sys.argv[1]}", file=sys.stderr)
            return 1
        rapport(sys.argv[1])
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 doublons_fichiers.py [dossier]", file=sys.stderr)
        return 1
    with tempfile.TemporaryDirectory() as d:
        photo = os.urandom(50_000)
        fichiers = {"WhatsApp/IMG-20261001.jpg": photo, "WhatsApp/IMG-20261001(1).jpg": photo,
                    "Photos/anniversaire.jpg": photo, "Docs/cours_C.pdf": os.urandom(30_000),
                    "Docs/cours_C_copie.pdf": None, "Docs/autre.pdf": None}
        cours = fichiers["Docs/cours_C.pdf"]
        fichiers["Docs/cours_C_copie.pdf"] = cours
        fichiers["Docs/autre.pdf"] = cours[:-1] + bytes([cours[-1] ^ 1])   # même taille, 1 bit différent
        for nom, contenu in fichiers.items():
            os.makedirs(os.path.join(d, os.path.dirname(nom)), exist_ok=True)
            with open(os.path.join(d, nom), "wb") as f:
                f.write(contenu)
        print(f"Dossier de démonstration avec {len(fichiers)} fichiers :\n")
        rapport(d)
    return 0


if __name__ == "__main__":
    sys.exit(main())
