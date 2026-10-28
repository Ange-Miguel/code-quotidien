"""Jour 101 — Manipulation de bits : activer, désactiver, basculer un bit de registre

Problème : sur un microcontrôleur (AVR, STM32, ESP32), on allume une LED, on
active une interruption ou on règle une broche en écrivant des bits précis
dans un registre de 8, 16 ou 32 bits, sans toucher aux autres bits.
Solution : les quatre opérations de base, avec un masque (1 << n) :
activer = reg | masque, désactiver = reg & ~masque, basculer = reg ^ masque,
lire = (reg >> n) & 1. On simule le registre PORTB d'un AVR (8 bits) qui
pilote des LED, et on affiche le registre en binaire après chaque opération.
Python n'a pas d'entiers de taille fixe : on applique & 0xFF pour rester sur
8 bits, comme le ferait le matériel.
On apprend : les opérateurs bit à bit (|, &, ^, ~, <<, >>), les masques, la
lecture-modification-écriture d'un registre et le piège des entiers Python.
"""
import sys

LARGEUR = 8
MASQUE_REGISTRE = (1 << LARGEUR) - 1


class Registre:
    def __init__(self, nom: str, valeur: int = 0):
        self.nom, self.valeur = nom, valeur & MASQUE_REGISTRE

    def _verifier(self, bit: int) -> int:
        if not 0 <= bit < LARGEUR:
            raise ValueError(f"bit {bit} hors de 0..{LARGEUR - 1}")
        return 1 << bit

    def activer(self, bit: int) -> None:
        self.valeur |= self._verifier(bit)

    def desactiver(self, bit: int) -> None:
        self.valeur &= ~self._verifier(bit) & MASQUE_REGISTRE

    def basculer(self, bit: int) -> None:
        self.valeur ^= self._verifier(bit)

    def lire(self, bit: int) -> int:
        return (self.valeur >> bit) & 1 if self._verifier(bit) else 0

    def ecrire_champ(self, decalage: int, largeur: int, valeur: int) -> None:
        """Écrit un champ de plusieurs bits (ex. une vitesse sur 3 bits)."""
        masque = ((1 << largeur) - 1) << decalage
        self.valeur = (self.valeur & ~masque | (valeur << decalage) & masque) & MASQUE_REGISTRE

    def __str__(self) -> str:
        bits = f"{self.valeur:08b}"
        leds = "".join("●" if b == "1" else "○" for b in bits)
        return f"{self.nom} = 0b{bits} = 0x{self.valeur:02X} = {self.valeur:3d}   LED7→0 : {leds}"


def main() -> int:
    portb = Registre("PORTB")
    operations = [("activer", 0), ("activer", 3), ("activer", 7), ("basculer", 3), ("basculer", 1),
                  ("desactiver", 7), ("champ", (4, 3, 0b101))]
    if len(sys.argv) == 3:
        try:
            operations = [(sys.argv[1], int(sys.argv[2]))]
            if sys.argv[1] not in ("activer", "desactiver", "basculer"):
                raise ValueError("opération inconnue")
            Registre("test")._verifier(operations[0][1])
        except ValueError as e:
            print(f"Erreur : {e}. Usage : python3 bits_registre.py activer|desactiver|basculer <bit 0-7>", file=sys.stderr)
            return 1
    elif len(sys.argv) != 1:
        print("Usage : python3 bits_registre.py [activer|desactiver|basculer <bit>]", file=sys.stderr)
        return 1
    print(f"{'départ':22} {portb}")
    for op, arg in operations:
        if op == "champ":
            portb.ecrire_champ(*arg)
            libelle = f"champ bits 4-6 ← {arg[2]:03b}"
        else:
            getattr(portb, op)(arg)
            libelle = f"{op} bit {arg}"
        print(f"{libelle:22} {portb}")
    print(f"\nLecture : bit 0 = {portb.lire(0)}, bit 3 = {portb.lire(3)}, bit 5 = {portb.lire(5)}")
    print(f"Piège Python : ~0b00000001 vaut {~0b00000001} ; avec & 0xFF on obtient 0b{~0b00000001 & 0xFF:08b}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
