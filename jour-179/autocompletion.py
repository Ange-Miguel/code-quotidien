"""Jour 179 — Autocomplétion avec un arbre préfixe (trie)

Problème : quand on tape « cha » dans la recherche d'une boutique, on veut
voir instantanément « chargeur rapide », « chargeur solaire », « chapeau »…
en commençant par les plus recherchés. Parcourir toute la liste de produits à
chaque lettre tapée devient lent quand le catalogue grandit.
Solution : un arbre préfixe (trie) : chaque nœud représente une lettre, et
le chemin depuis la racine forme un préfixe. Tous les produits qui commencent
par « cha » sont sous le même nœud. Chaque nœud mémorise aussi les k
meilleures complétions (par popularité), ce qui rend la suggestion
instantanée. Les accents sont ignorés à la saisie (« eco » trouve « écouteurs »).
On apprend : la structure de trie, les dictionnaires imbriqués, le
précalcul des meilleurs résultats et la normalisation de la saisie.
"""
import sys
import time
import unicodedata


def normaliser(texte: str) -> str:
    return unicodedata.normalize("NFD", texte.lower()).encode("ascii", "ignore").decode()


class Noeud:
    __slots__ = ("enfants", "meilleurs")

    def __init__(self):
        self.enfants: dict[str, "Noeud"] = {}
        self.meilleurs: list[tuple[int, str]] = []    # (popularité, libellé), triés


class Trie:
    def __init__(self, k: int = 4):
        self.racine, self.k, self.nb = Noeud(), k, 0

    def ajouter(self, libelle: str, popularite: int) -> None:
        noeud = self.racine
        for c in normaliser(libelle):
            noeud = noeud.enfants.setdefault(c, Noeud())
            noeud.meilleurs.append((popularite, libelle))
            noeud.meilleurs.sort(key=lambda x: (-x[0], x[1]))
            del noeud.meilleurs[self.k:]
            self.nb += 1

    def suggerer(self, prefixe: str) -> list[str]:
        noeud = self.racine
        for c in normaliser(prefixe):
            if c not in noeud.enfants:
                return []
            noeud = noeud.enfants[c]
        return [lib for _, lib in noeud.meilleurs]


CATALOGUE = {"chargeur rapide": 950, "chargeur solaire": 310, "chapeau de paille": 45, "chaussures de sport": 520,
             "chaise pliante": 130, "câble usb-c": 870, "casque audio": 400, "écouteurs bluetooth": 1200,
             "écran 24 pouces": 150, "enceinte portable": 610, "lampe solaire": 480, "lampe torche": 220,
             "montre connectée": 690, "moto électrique": 35, "ventilateur rechargeable": 760}


def main() -> int:
    trie = Trie()
    for produit, pop in CATALOGUE.items():
        trie.ajouter(produit, pop)
    saisies = sys.argv[1:] or ["c", "cha", "char", "eco", "lampe s", "mo", "xyz"]
    print(f"{len(CATALOGUE)} produits indexés ({trie.nb} insertions de nœuds)\n")
    for s in saisies:
        r = trie.suggerer(s)
        print(f"« {s} » → {', '.join(r) if r else 'aucune suggestion'}")
    grand = Trie()
    for i in range(20_000):
        grand.ajouter(f"produit {i:05d} réf {i * 7919 % 10_000}", i % 997)
    t0 = time.perf_counter()
    for _ in range(1000):
        grand.suggerer("produit 12")
    print(f"\nCatalogue de 20 000 produits : 1 000 suggestions en {1000 * (time.perf_counter() - t0):.1f} ms")
    return 0


if __name__ == "__main__":
    sys.exit(main())
