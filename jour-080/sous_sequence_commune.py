"""Jour 080 — Plus longue sous-séquence commune (comparaison de textes)

Problème : comment un outil comme « git diff » ou un correcteur de copies
trouve-t-il ce qui a changé entre deux versions d'un texte ? Et comment
mesurer la ressemblance entre deux séquences d'ADN ou deux devoirs ?
Solution : la plus longue sous-séquence commune (LCS) se calcule par
programmation dynamique : T[i][j] = longueur de la LCS des préfixes. En
remontant le tableau, on obtient les éléments communs, puis un diff ligne par
ligne : « - » ligne supprimée, « + » ligne ajoutée, « ' ' » inchangée. On
mesure aussi le taux de similarité entre deux textes.
On apprend : la programmation dynamique sur deux dimensions, la remontée d'un
tableau pour reconstruire une solution et le principe des outils de diff.
"""
import sys


def table_lcs(a: list, b: list) -> list[list[int]]:
    t = [[0] * (len(b) + 1) for _ in range(len(a) + 1)]
    for i in range(1, len(a) + 1):
        for j in range(1, len(b) + 1):
            t[i][j] = t[i - 1][j - 1] + 1 if a[i - 1] == b[j - 1] else max(t[i - 1][j], t[i][j - 1])
    return t


def lcs(a, b) -> list:
    t, i, j, res = table_lcs(a, b), len(a), len(b), []
    while i and j:
        if a[i - 1] == b[j - 1]:
            res.append(a[i - 1]); i -= 1; j -= 1
        elif t[i - 1][j] >= t[i][j - 1]:
            i -= 1
        else:
            j -= 1
    return res[::-1]


def diff(a: list[str], b: list[str]) -> list[str]:
    t, i, j, sortie = table_lcs(a, b), len(a), len(b), []
    while i or j:
        if i and j and a[i - 1] == b[j - 1]:
            sortie.append("  " + a[i - 1]); i -= 1; j -= 1
        elif j and (not i or t[i][j - 1] >= t[i - 1][j]):
            sortie.append("+ " + b[j - 1]); j -= 1
        else:
            sortie.append("- " + a[i - 1]); i -= 1
    return sortie[::-1]


def similarite(a, b) -> float:
    return 2 * len(lcs(a, b)) / (len(a) + len(b)) if a or b else 1.0


def main() -> int:
    if len(sys.argv) == 3:
        x, y = sys.argv[1], sys.argv[2]
        print(f"LCS de {x!r} et {y!r} : {''.join(lcs(x, y))!r} ({similarite(x, y):.0%} de similarité)")
        return 0
    if len(sys.argv) != 1:
        print("Usage : python3 sous_sequence_commune.py [texte1 texte2]", file=sys.stderr)
        return 1
    for x, y in [("ABCBDAB", "BDCABA"), ("électronique", "électrique"), ("GATTACA", "TACGATA")]:
        print(f"LCS({x}, {y}) = {''.join(lcs(x, y))} | similarité {similarite(x, y):.0%}")
    v1 = ["Bonjour,", "Votre commande de 3 écouteurs est prête.", "Livraison à Calavi demain.",
          "Paiement à la livraison.", "Merci."]
    v2 = ["Bonjour,", "Votre commande de 5 écouteurs est prête.", "Livraison à Calavi demain.",
          "Merci pour votre confiance."]
    print("\nDiff entre deux versions d'un message :")
    for ligne in diff(v1, v2):
        print("   " + ligne)
    print(f"Similarité des deux versions : {similarite(v1, v2):.0%}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
