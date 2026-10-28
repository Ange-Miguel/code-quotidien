"""Jour 035 — Nombre en lettres en français (pour chèques et factures)

Problème : sur un chèque, une facture ou un contrat, le montant doit être
écrit en toutes lettres (« deux cent cinquante mille francs CFA »). Les règles
du français sont piégeuses : soixante-dix, quatre-vingts, cent/cents, et-un…
Solution : on découpe le nombre en tranches de trois chiffres (unités,
milliers, millions, milliards). Chaque tranche est écrite par une fonction
dédiée qui applique les règles d'accord : « quatre-vingts » et « deux cents »
prennent un s seulement en fin de nombre, « mille » est invariable, « million »
et « milliard » prennent un s au pluriel. Orthographe traditionnelle.
On apprend : la décomposition récursive d'un problème, les cas particuliers
d'une langue et les tests par exemples connus.
"""
import sys

UNITES = ["zéro", "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix",
          "onze", "douze", "treize", "quatorze", "quinze", "seize", "dix-sept", "dix-huit", "dix-neuf"]
DIZAINES = ["", "", "vingt", "trente", "quarante", "cinquante", "soixante", "soixante", "quatre-vingt",
            "quatre-vingt"]


def moins_de_cent(n: int, final: bool) -> str:
    if n < 20:
        return UNITES[n]
    d, u = divmod(n, 10)
    if d in (7, 9):                       # soixante-dix…, quatre-vingt-dix…
        u += 10
    base = DIZAINES[d]
    if u == 0:
        return base + ("s" if d == 8 and final else "")
    if u in (1, 11) and d != 8 and d != 9:
        return f"{base} et {UNITES[u]}"
    return f"{base}-{UNITES[u]}"


def moins_de_mille(n: int, final: bool) -> str:
    c, reste = divmod(n, 100)
    if c == 0:
        return moins_de_cent(reste, final)
    centaine = "cent" if c == 1 else f"{UNITES[c]} cent"
    if reste == 0:
        return centaine + ("s" if c > 1 and final else "")
    return f"{centaine} {moins_de_cent(reste, final)}"


def en_lettres(n: int) -> str:
    if n == 0:
        return "zéro"
    if not 0 < n < 10 ** 12:
        raise ValueError("nombre entre 0 et 999 999 999 999")
    morceaux = []
    for valeur, nom in ((10 ** 9, "milliard"), (10 ** 6, "million"), (1000, "mille")):
        q, n = divmod(n, valeur)
        if q == 0:
            continue
        if nom == "mille":
            morceaux.append("mille" if q == 1 else f"{moins_de_mille(q, False)} mille")
        else:
            morceaux.append(f"{moins_de_mille(q, True)} {nom}{'s' if q > 1 else ''}")
    if n:
        morceaux.append(moins_de_mille(n, True))
    return " ".join(morceaux)


TESTS = {21: "vingt et un", 71: "soixante et onze", 80: "quatre-vingts", 81: "quatre-vingt-un",
         91: "quatre-vingt-onze", 200: "deux cents", 280: "deux cent quatre-vingts",
         1000: "mille", 80000: "quatre-vingt mille", 2_000_000: "deux millions"}


def main() -> int:
    for n, attendu in TESTS.items():
        assert en_lettres(n) == attendu, (n, en_lettres(n))
    nombres = sys.argv[1:] or ["71", "180", "1960", "250000", "3750500", "1200000000"]
    for texte in nombres:
        try:
            n = int(texte.replace(" ", ""))
            print(f"{n:>15,} → {en_lettres(n)} francs CFA".replace(",", " "))
        except ValueError as e:
            print(f"Erreur pour {texte!r} : {e}", file=sys.stderr)
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
