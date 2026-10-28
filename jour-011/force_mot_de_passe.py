"""Jour 011 — Vérificateur de force d'un mot de passe

Problème : beaucoup de comptes sont piratés parce que le mot de passe est trop
court, trop simple ou trop courant (« 123456 », « azerty », « password »).
Solution : on estime l'entropie (en bits) à partir de la taille de l'alphabet
utilisé et de la longueur, puis on applique des pénalités : mot de passe
courant, répétitions, suites de clavier ou de chiffres. On en déduit une note
et des conseils concrets.
On apprend : les ensembles (set), les méthodes de chaînes (isdigit, isupper…),
le logarithme et la notion d'entropie en sécurité.
"""
import math
import sys

COURANTS = {"123456", "password", "azerty", "qwerty", "motdepasse", "000000",
            "iloveyou", "admin", "benin229", "12345678", "soleil"}
SUITES = ["azertyuiop", "qwertyuiop", "abcdefghijklmnopqrstuvwxyz", "0123456789"]


def analyser(mdp: str) -> tuple[float, list[str]]:
    conseils = []
    alphabet = 0
    if any(c.islower() for c in mdp): alphabet += 26
    else: conseils.append("ajouter des minuscules")
    if any(c.isupper() for c in mdp): alphabet += 26
    else: conseils.append("ajouter des majuscules")
    if any(c.isdigit() for c in mdp): alphabet += 10
    else: conseils.append("ajouter des chiffres")
    if any(not c.isalnum() for c in mdp): alphabet += 32
    else: conseils.append("ajouter des symboles (!, ?, #…)")
    entropie = len(mdp) * math.log2(alphabet) if alphabet else 0.0

    if len(mdp) < 12:
        conseils.append("viser au moins 12 caractères")
    if mdp.lower() in COURANTS:
        entropie = min(entropie, 5)
        conseils.append("ce mot de passe fait partie des plus utilisés au monde")
    if len(set(mdp)) <= len(mdp) // 2:
        entropie *= 0.6
        conseils.append("trop de caractères répétés")
    bas = mdp.lower()
    for suite in SUITES:
        if any(suite[i:i + 4] in bas for i in range(len(suite) - 3)):
            entropie *= 0.7
            conseils.append("éviter les suites (abcd, 1234, azer…)")
            break
    return entropie, conseils


def note(entropie: float) -> str:
    if entropie < 28: return "Très faible"
    if entropie < 36: return "Faible"
    if entropie < 60: return "Moyen"
    if entropie < 80: return "Fort"
    return "Très fort"


def rapport(mdp: str) -> None:
    e, conseils = analyser(mdp)
    print(f"{mdp!r:24} → {e:5.1f} bits : {note(e)}")
    for c in conseils:
        print(f"      - {c}")


def main() -> int:
    exemples = sys.argv[1:] or ["123456", "Benin2026", "azerty1234", "M4!s0n-Calavi#Lac", "aaaaaaaaaaaa"]
    for mdp in exemples:
        if not mdp:
            print("Erreur : mot de passe vide", file=sys.stderr)
            return 1
        rapport(mdp)
    return 0


if __name__ == "__main__":
    sys.exit(main())
