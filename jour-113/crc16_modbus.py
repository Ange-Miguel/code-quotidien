"""Jour 113 — Calcul CRC-16 Modbus

Problème : Modbus RTU est le protocole le plus répandu dans l'industrie :
compteurs d'énergie, variateurs, onduleurs solaires, automates. Chaque trame
série se termine par un CRC-16 de 2 octets (octet de poids faible en premier).
Pour dialoguer avec un compteur d'énergie depuis un ESP32, il faut savoir
construire et vérifier ces trames.
Solution : CRC-16 Modbus = polynôme 0xA001 (forme réfléchie de 0x8005),
valeur initiale 0xFFFF. On l'implémente, on le vérifie (valeur de contrôle
0x4B37 pour « 123456789 »), puis on construit une vraie requête « lire des
registres de maintien » (fonction 0x03) et on décode une réponse simulée de
compteur d'énergie, en rejetant une réponse corrompue.
On apprend : le CRC-16, l'ordre des octets (petit-boutiste / gros-boutiste),
le module struct et la structure d'une trame Modbus RTU.
"""
import struct
import sys


def crc16_modbus(donnees: bytes) -> int:
    crc = 0xFFFF
    for octet in donnees:
        crc ^= octet
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc


def ajouter_crc(trame: bytes) -> bytes:
    return trame + struct.pack("<H", crc16_modbus(trame))     # octet faible en premier


def verifier(trame: bytes) -> bool:
    return len(trame) >= 4 and crc16_modbus(trame[:-2]) == struct.unpack("<H", trame[-2:])[0]


def requete_lecture(esclave: int, registre: int, nombre: int) -> bytes:
    return ajouter_crc(struct.pack(">BBHH", esclave, 0x03, registre, nombre))


def decoder_reponse(trame: bytes) -> list[int]:
    if not verifier(trame):
        raise ValueError("CRC invalide")
    esclave, fonction, nb_octets = trame[0], trame[1], trame[2]
    if fonction & 0x80:
        raise ValueError(f"exception Modbus code {trame[2]}")
    return list(struct.unpack(f">{nb_octets // 2}H", trame[3:3 + nb_octets]))


def hexa(b: bytes) -> str:
    return " ".join(f"{x:02X}" for x in b)


def main() -> int:
    if len(sys.argv) == 2:
        try:
            trame = bytes.fromhex(sys.argv[1])
        except ValueError:
            print("Usage : python3 crc16_modbus.py <trame hexadécimale, ex. 0103000A0002>", file=sys.stderr)
            return 1
        print(f"{hexa(trame)} → avec CRC : {hexa(ajouter_crc(trame))}")
        return 0
    assert crc16_modbus(b"123456789") == 0x4B37
    print(f"Contrôle : CRC(\"123456789\") = 0x{crc16_modbus(b'123456789'):04X} (attendu 0x4B37)\n")
    req = requete_lecture(esclave=1, registre=0x0000, nombre=3)
    print(f"Requête « lire 3 registres à partir de 0 » vers le compteur n°1 :\n   {hexa(req)}")
    # Réponse simulée : tension 230,4 V (×10), courant 5,25 A (×100), puissance 1180 W
    reponse = ajouter_crc(struct.pack(">BBB3H", 1, 0x03, 6, 2304, 525, 1180))
    print(f"Réponse reçue :\n   {hexa(reponse)}")
    tension, courant, puissance = decoder_reponse(reponse)
    print(f"   → tension {tension / 10} V, courant {courant / 100} A, puissance {puissance} W")
    corrompue = bytearray(reponse)
    corrompue[4] ^= 0x10
    try:
        decoder_reponse(bytes(corrompue))
    except ValueError as e:
        print(f"Réponse corrompue (1 bit) :\n   {hexa(corrompue)} → rejetée : {e}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
