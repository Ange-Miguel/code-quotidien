"""Jour 146 — Chiffrement XOR et clé partagée pour une trame IoT

Problème : une trame radio (433 MHz, LoRa) circule en clair : n'importe qui
avec un récepteur à 20 000 FCFA peut lire la température, ou pire, la
commande « ouvrir le portail ». Comprendre le chiffrement le plus simple,
le XOR, aide à voir ce qui protège vraiment… et ce qui ne protège pas.
Solution : chiffrer = faire un XOR de chaque octet avec un flux de clé ;
déchiffrer = refaire exactement le même XOR (car (m ^ k) ^ k = m). On montre
trois versions : (1) clé courte répétée : cassable dès qu'on connaît un
morceau du message ; (2) flux de clé dérivé d'une clé secrète et d'un
compteur (nonce) différent pour chaque trame, via SHA-256 ; (3) le danger
de réutiliser le même flux : le XOR de deux messages chiffrés révèle le XOR
des messages clairs. Pour un vrai produit, il faut un algorithme standard
(AES-CCM, ChaCha20-Poly1305), jamais un chiffrement maison.
On apprend : l'opération XOR, les flux de clé, le rôle du nonce, une attaque
par texte clair connu, et pourquoi la cryptographie maison est dangereuse.
"""
import hashlib
import sys


def xor(donnees: bytes, flux: bytes) -> bytes:
    return bytes(d ^ f for d, f in zip(donnees, flux))


def cle_repetee(cle: bytes, n: int) -> bytes:
    return (cle * (n // len(cle) + 1))[:n]


def flux_cle(secret: bytes, nonce: int, n: int) -> bytes:
    """Flux pseudo-aléatoire : SHA-256(secret || nonce || bloc), bloc après bloc (principe d'un mode compteur)."""
    flux, bloc = b"", 0
    while len(flux) < n:
        flux += hashlib.sha256(secret + nonce.to_bytes(8, "big") + bloc.to_bytes(4, "big")).digest()
        bloc += 1
    return flux[:n]


def main() -> int:
    if len(sys.argv) == 3:
        message, cle = sys.argv[1].encode(), sys.argv[2].encode()
        if not cle:
            print("Clé vide interdite", file=sys.stderr)
            return 1
        print(xor(message, cle_repetee(cle, len(message))).hex())
        return 0
    if len(sys.argv) != 1:
        print("Usage : python3 chiffrement_xor.py [message clé]", file=sys.stderr)
        return 1
    trame = b"PORTAIL:OUVRIR;code=4412"
    print(f"Trame en clair      : {trame.decode()}\n")

    cle = b"K3y"
    chiffre = xor(trame, cle_repetee(cle, len(trame)))
    print(f"1) Clé courte répétée « {cle.decode()} » : {chiffre.hex()}")
    connu = b"PORTAIL:"                                        # l'attaquant devine le début
    cle_retrouvee = xor(chiffre[:len(connu)], connu)
    print(f"   L'attaquant devine « PORTAIL: » et retrouve la clé : {cle_retrouvee[:3].decode()}…")
    print(f"   Message déchiffré par l'attaquant : {xor(chiffre, cle_repetee(cle_retrouvee[:3], len(chiffre))).decode()}\n")

    secret = b"secret-partage-entre-portail-et-telecommande"
    for nonce in (1, 2):
        c = xor(trame, flux_cle(secret, nonce, len(trame)))
        print(f"2) Flux SHA-256, nonce {nonce} : {c.hex()}")
    retour = xor(xor(trame, flux_cle(secret, 1, len(trame))), flux_cle(secret, 1, len(trame)))
    print(f"   Le récepteur (même secret + nonce) retrouve : {retour.decode()}")
    print("   Même message, nonce différent → chiffré totalement différent.\n")

    autre = b"PORTAIL:FERMER;code=4412"
    c1, c2 = xor(trame, flux_cle(secret, 7, len(trame))), xor(autre, flux_cle(secret, 7, len(autre)))
    print("3) Erreur grave : réutiliser le nonce 7 pour deux trames")
    print(f"   c1 XOR c2 = clair1 XOR clair2 : {xor(c1, c2).hex()}")
    print(f"   Les octets nuls révèlent les parties identiques des deux messages : "
          f"{''.join('=' if b == 0 else '?' for b in xor(c1, c2))}")
    print("\nPour un produit réel : AES-CCM ou ChaCha20-Poly1305, avec authentification (voir jour 147).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
