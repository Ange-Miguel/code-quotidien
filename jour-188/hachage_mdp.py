"""Jour 188 — Hachage de mots de passe avec sel

Problème : si la base de données d'un site est volée et que les mots de
passe y sont en clair, ou simplement hachés avec SHA-256, les pirates les
retrouvent en quelques secondes : deux utilisateurs avec le même mot de
passe ont la même empreinte, et des tables précalculées donnent les mots de
passe courants.
Solution : on ne stocke jamais le mot de passe. On stocke un sel aléatoire
(16 octets, différent pour chaque utilisateur) et le résultat d'une fonction
de dérivation volontairement LENTE : PBKDF2-HMAC-SHA256 avec 200 000
itérations (module hashlib), ou scrypt. La vérification recalcule et compare
en temps constant (hmac.compare_digest). On montre l'attaque par
dictionnaire contre un SHA-256 simple, puis son coût contre PBKDF2.
On apprend : la différence entre hacher et chiffrer, le sel, le coût
volontaire (itérations), le format de stockage et la comparaison sûre.
"""
import hashlib
import hmac
import os
import sys
import time

ITERATIONS = 200_000


def hacher(mdp: str, sel: bytes | None = None, iterations: int = ITERATIONS) -> str:
    sel = sel or os.urandom(16)
    dk = hashlib.pbkdf2_hmac("sha256", mdp.encode(), sel, iterations)
    return f"pbkdf2_sha256${iterations}${sel.hex()}${dk.hex()}"


def verifier(mdp: str, stocke: str) -> bool:
    try:
        algo, iterations, sel_hex, dk_hex = stocke.split("$")
    except ValueError:
        return False
    if algo != "pbkdf2_sha256":
        return False
    calcule = hashlib.pbkdf2_hmac("sha256", mdp.encode(), bytes.fromhex(sel_hex), int(iterations))
    return hmac.compare_digest(calcule.hex(), dk_hex)


DICTIONNAIRE = ["123456", "password", "azerty", "benin229", "motdepasse", "soleil", "iloveyou", "cotonou2026",
                "qwerty", "000000"] * 10


def main() -> int:
    if len(sys.argv) == 2:
        if not sys.argv[1]:
            print("Mot de passe vide refusé", file=sys.stderr)
            return 1
        print(hacher(sys.argv[1]))
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 hachage_mdp.py [mot de passe]", file=sys.stderr)
        return 1
    base = {"awa": "benin229", "kofi": "benin229", "ines": "M4!s0n-Calavi#Lac"}
    print("1) Mauvaise pratique : SHA-256 sans sel")
    naive = {u: hashlib.sha256(m.encode()).hexdigest() for u, m in base.items()}
    for u, h in naive.items():
        print(f"   {u:5} {h[:24]}…")
    print("   → Awa et Kofi ont la même empreinte : on voit qu'ils partagent le même mot de passe.")
    t0 = time.perf_counter()
    trouves = {u for u, h in naive.items() for mot in DICTIONNAIRE if hashlib.sha256(mot.encode()).hexdigest() == h}
    print(f"   Attaque par dictionnaire : {sorted(trouves)} trouvés en {1000 * (time.perf_counter() - t0):.1f} ms\n")

    print(f"2) Bonne pratique : PBKDF2-SHA256, sel aléatoire, {ITERATIONS:_} itérations".replace("_", " "))
    stocke = {u: hacher(m) for u, m in base.items()}
    for u, s in stocke.items():
        print(f"   {u:5} {s[:30]}…{s[-12:]}")
    print("   → même mot de passe, empreintes différentes grâce au sel.")
    print(f"   Vérification Awa « benin229 » : {verifier('benin229', stocke['awa'])} | « Benin229 » : {verifier('Benin229', stocke['awa'])}")
    t0 = time.perf_counter()
    verifier("essai", stocke["awa"])
    un = time.perf_counter() - t0
    print(f"   Un essai coûte {un * 1000:.0f} ms : tester 10 millions de mots de passe prendrait ~{un * 1e7 / 86400:.0f} jours par compte.")
    print("\nRègle d'or : ne jamais stocker un mot de passe, seulement sel + dérivation lente (PBKDF2, scrypt, bcrypt, Argon2).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
