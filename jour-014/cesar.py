"""Jour 014 — Chiffrement de César : chiffrer et déchiffrer un message

Problème : on veut cacher le sens d'un court message (jeu, devinette) et,
surtout, comprendre pourquoi les chiffrements trop simples sont cassables.
Solution : chaque lettre est décalée de k positions dans l'alphabet. Pour
déchiffrer sans la clé, on teste les 26 décalages et on garde celui dont la
fréquence des lettres ressemble le plus au français (attaque par analyse de
fréquences, score du khi-deux).
On apprend : les codes de caractères (ord/chr), l'arithmétique modulaire et
une première attaque cryptographique.
"""
import string
import sys

# Fréquences approximatives des lettres en français (en %)
FREQ_FR = dict(zip(string.ascii_lowercase, [
    7.6, 0.9, 3.3, 3.7, 14.7, 1.1, 0.9, 0.7, 7.5, 0.6, 0.1, 5.5, 3.0,
    7.1, 5.8, 2.5, 1.4, 6.7, 7.9, 7.2, 6.3, 1.8, 0.1, 0.4, 0.3, 0.1]))


def cesar(texte: str, cle: int) -> str:
    resultat = []
    for c in texte:
        if c in string.ascii_lowercase:
            resultat.append(chr((ord(c) - 97 + cle) % 26 + 97))
        elif c in string.ascii_uppercase:
            resultat.append(chr((ord(c) - 65 + cle) % 26 + 65))
        else:
            resultat.append(c)
    return "".join(resultat)


def score_francais(texte: str) -> float:
    """Khi-deux entre les fréquences observées et celles du français (plus bas = mieux)."""
    lettres = [c for c in texte.lower() if c in FREQ_FR]
    if not lettres:
        return float("inf")
    total = len(lettres)
    return sum((lettres.count(l) - total * p / 100) ** 2 / (total * p / 100) for l, p in FREQ_FR.items())


def casser(chiffre: str) -> tuple[int, str]:
    meilleure = min(range(26), key=lambda k: score_francais(cesar(chiffre, -k)))
    return meilleure, cesar(chiffre, -meilleure)


def main() -> int:
    if len(sys.argv) == 3:
        try:
            cle = int(sys.argv[1])
        except ValueError:
            print("Usage : python3 cesar.py <clé> \"message\"", file=sys.stderr)
            return 1
        print(cesar(sys.argv[2], cle))
        return 0
    if len(sys.argv) != 1:
        print("Usage : python3 cesar.py <clé> \"message\"", file=sys.stderr)
        return 1
    message = "Rendez-vous demain matin devant la bibliotheque de l'universite"
    chiffre = cesar(message, 7)
    print("Message  :", message)
    print("Chiffré  :", chiffre)
    print("Déchiffré:", cesar(chiffre, -7))
    cle, texte = casser(chiffre)
    print(f"\nAttaque sans la clé → clé trouvée : {cle}")
    print("Résultat :", texte)
    return 0


if __name__ == "__main__":
    sys.exit(main())
