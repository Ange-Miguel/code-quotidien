"""Jour 083 — Codage de Huffman d'un message

Problème : en ASCII, chaque caractère occupe 8 bits, même le « e » qui revient
sans cesse. Peut-on utiliser des codes plus courts pour les caractères
fréquents, et plus longs pour les rares, sans ambiguïté au décodage ?
Solution : l'algorithme de Huffman construit un arbre binaire : on fusionne
toujours les deux nœuds les moins fréquents (file de priorité heapq) jusqu'à
n'en garder qu'un. Le chemin vers chaque feuille (0 = gauche, 1 = droite)
donne son code. Aucun code n'est le préfixe d'un autre, donc le décodage est
sans ambiguïté. On encode, décode et mesure le gain.
On apprend : les tas (heapq), la construction d'arbres, les codes préfixes et
la compression sans perte utilisée dans ZIP, JPEG ou MP3.
"""
import heapq
import itertools
import sys
from collections import Counter


def construire_codes(texte: str) -> dict[str, str]:
    compteur = itertools.count()          # départage les égalités dans le tas
    tas = [(f, next(compteur), c) for c, f in Counter(texte).items()]
    heapq.heapify(tas)
    if len(tas) == 1:
        return {tas[0][2]: "0"}
    while len(tas) > 1:
        f1, _, g = heapq.heappop(tas)
        f2, _, d = heapq.heappop(tas)
        heapq.heappush(tas, (f1 + f2, next(compteur), (g, d)))
    codes = {}

    def parcourir(noeud, prefixe: str) -> None:
        if isinstance(noeud, str):
            codes[noeud] = prefixe
        else:
            parcourir(noeud[0], prefixe + "0")
            parcourir(noeud[1], prefixe + "1")

    parcourir(tas[0][2], "")
    return codes


def encoder(texte: str, codes: dict[str, str]) -> str:
    return "".join(codes[c] for c in texte)


def decoder(bits: str, codes: dict[str, str]) -> str:
    inverse, courant, sortie = {v: k for k, v in codes.items()}, "", []
    for b in bits:
        courant += b
        if courant in inverse:
            sortie.append(inverse[courant])
            courant = ""
    if courant:
        raise ValueError("bits restants non décodables")
    return "".join(sortie)


def analyser(texte: str) -> None:
    if not texte:
        raise ValueError("texte vide")
    codes = construire_codes(texte)
    bits = encoder(texte, codes)
    assert decoder(bits, codes) == texte
    freq = Counter(texte)
    print(f"Texte : {texte[:60]!r}{'…' if len(texte) > 60 else ''}")
    for c, code in sorted(codes.items(), key=lambda x: (len(x[1]), x[1]))[:10]:
        print(f"   {c!r:5} ×{freq[c]:3d} → {code}")
    if len(codes) > 10:
        print(f"   … et {len(codes) - 10} autres caractères")
    taille = len(texte.encode("utf-8")) * 8
    print(f"   {taille} bits en UTF-8 → {len(bits)} bits avec Huffman "
          f"({len(bits) / taille:.0%} de la taille, {len(bits) / len(texte):.2f} bits/caractère)\n")


def main() -> int:
    textes = [" ".join(sys.argv[1:])] if len(sys.argv) > 1 else [
        "le zem est passé devant le marché de calavi ce matin, et le zem est reparti",
        "aaaaaaaaaaaaaaaaaaaabbbbbbbbccccd",
    ]
    for t in textes:
        try:
            analyser(t)
        except ValueError as e:
            print(f"Erreur : {e}", file=sys.stderr)
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
