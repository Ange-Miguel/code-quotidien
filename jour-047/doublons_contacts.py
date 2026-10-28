"""Jour 047 — Détecteur de doublons dans une liste de contacts

Problème : après plusieurs changements de téléphone, le répertoire contient
« Maman », « maman », « Maman Calavi » avec le même numéro écrit de trois
façons. Il faut repérer et fusionner les doublons.
Solution : on normalise chaque numéro (chiffres seulement, indicatif +229 et
préfixe 01 harmonisés) et chaque nom (minuscules, sans accents ni espaces
superflus). Deux contacts sont des doublons s'ils ont le même numéro
normalisé, ou des noms très proches (ratio difflib ≥ 0,85). On propose une
fiche fusionnée qui garde le nom le plus complet et tous les numéros.
On apprend : la normalisation de données, unicodedata, difflib et le
regroupement avec une structure union-find.
"""
import difflib
import re
import sys
import unicodedata


def normaliser_numero(numero: str) -> str:
    chiffres = re.sub(r"\D", "", numero)
    if chiffres.startswith("00229"):
        chiffres = chiffres[5:]
    elif chiffres.startswith("229") and len(chiffres) > 10:
        chiffres = chiffres[3:]
    if len(chiffres) == 8:           # ancien format à 8 chiffres
        chiffres = "01" + chiffres
    return chiffres


def normaliser_nom(nom: str) -> str:
    sans_accents = unicodedata.normalize("NFD", nom).encode("ascii", "ignore").decode()
    return " ".join(sans_accents.lower().split())


class UnionFind:
    def __init__(self, n: int):
        self.parent = list(range(n))

    def trouver(self, x: int) -> int:
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def unir(self, a: int, b: int) -> None:
        self.parent[self.trouver(a)] = self.trouver(b)


def regrouper(contacts: list[tuple[str, str]]) -> list[list[int]]:
    uf = UnionFind(len(contacts))
    noms = [normaliser_nom(n) for n, _ in contacts]
    nums = [normaliser_numero(t) for _, t in contacts]
    for i in range(len(contacts)):
        for j in range(i + 1, len(contacts)):
            if nums[i] == nums[j] or difflib.SequenceMatcher(None, noms[i], noms[j]).ratio() >= 0.85:
                uf.unir(i, j)
    groupes = {}
    for i in range(len(contacts)):
        groupes.setdefault(uf.trouver(i), []).append(i)
    return list(groupes.values())


def main() -> int:
    contacts = [("Maman", "+229 01 97 12 34 56"), ("maman ", "97123456"), ("Koffi Mensah", "01 61 22 33 44"),
                ("Kofi Mensah", "0161223355"), ("Père Gérard", "00229 01 95 00 11 22"),
                ("Pere Gerard", "+229 0195001122"), ("Aïcha", "01 66 00 00 01"), ("Boutique Gaz", "01 52 44 44 44")]
    if len(sys.argv) > 1:
        try:
            contacts = [tuple(a.split("=", 1)) for a in sys.argv[1:]]
            if any(len(c) != 2 for c in contacts):
                raise ValueError
        except ValueError:
            print("Usage : python3 doublons_contacts.py \"Nom=numéro\" ...", file=sys.stderr)
            return 1
    groupes = regrouper(contacts)
    doublons = [g for g in groupes if len(g) > 1]
    print(f"{len(contacts)} contacts → {len(groupes)} contacts uniques, {len(doublons)} groupe(s) de doublons\n")
    for g in doublons:
        print("Doublons :")
        for i in g:
            print(f"   - {contacts[i][0]!r:18} {contacts[i][1]}")
        nom = max((contacts[i][0].strip() for i in g), key=len)
        numeros = sorted({normaliser_numero(contacts[i][1]) for i in g})
        print(f"   → fusion : {nom} : {', '.join('+229 ' + n for n in numeros)}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
