"""Jour 140 — Simulation d'une maison connectée : lampes, portes, capteurs

Problème : avant d'acheter du matériel domotique, on veut tester les
scénarios : « quand la porte d'entrée s'ouvre la nuit, allumer l'entrée et
prévenir », « à 22 h, tout éteindre sauf la chambre », « s'il fait plus de
30 °C dans le salon, lancer le ventilateur ». Tout cela peut se simuler.
Solution : des classes pour chaque type d'appareil (Lampe, Porte, Capteur,
Ventilateur) qui héritent d'une classe Appareil ; un bus d'événements simple
(observateur) : chaque changement d'état publie un événement ; des règles
(condition + action) écoutent ces événements. On rejoue une journée simulée
et on affiche le journal des automatismes déclenchés.
On apprend : l'héritage et le polymorphisme, le patron de conception
Observateur, les règles « si … alors … » et la simulation par événements.
"""
import sys
from dataclasses import dataclass, field
from typing import Callable


class Bus:
    def __init__(self):
        self.ecouteurs: list[Callable] = []
        self.journal: list[str] = []
        self.heure = "00:00"

    def publier(self, appareil: "Appareil", attribut: str, valeur) -> None:
        for e in list(self.ecouteurs):
            e(appareil, attribut, valeur)


@dataclass
class Appareil:
    nom: str
    piece: str
    bus: Bus = field(repr=False)

    def changer(self, attribut: str, valeur) -> None:
        if getattr(self, attribut) != valeur:
            setattr(self, attribut, valeur)
            self.bus.journal.append(f"{self.bus.heure}  {self.nom:18} {attribut} = {valeur}")
            self.bus.publier(self, attribut, valeur)


@dataclass
class Lampe(Appareil):
    allumee: bool = False


@dataclass
class Porte(Appareil):
    ouverte: bool = False


@dataclass
class Capteur(Appareil):
    temperature: float = 27.0


@dataclass
class Ventilateur(Appareil):
    marche: bool = False


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    bus = Bus()
    entree, salon, chambre = Lampe("lampe entrée", "entrée", bus), Lampe("lampe salon", "salon", bus), Lampe("lampe chambre", "chambre", bus)
    porte, sonde, ventilo = Porte("porte d'entrée", "entrée", bus), Capteur("sonde salon", "salon", bus), Ventilateur("ventilateur salon", "salon", bus)
    nuit = lambda: not "07:00" <= bus.heure < "19:00"

    def regle_porte(app, attr, val):
        if app is porte and val and nuit():
            entree.changer("allumee", True)
            bus.journal.append(f"{bus.heure}  ⚑ notification : porte ouverte de nuit")

    def regle_chaleur(app, attr, val):
        if app is sonde:
            if val > 30:
                ventilo.changer("marche", True)
            elif val < 28:
                ventilo.changer("marche", False)

    bus.ecouteurs += [regle_porte, regle_chaleur]
    scenario = [("06:30", porte, "ouverte", True), ("06:31", porte, "ouverte", False), ("12:00", sonde, "temperature", 29.5),
                ("13:30", sonde, "temperature", 31.2), ("16:00", sonde, "temperature", 30.4), ("18:00", sonde, "temperature", 27.6),
                ("19:30", salon, "allumee", True), ("20:00", chambre, "allumee", True), ("21:15", porte, "ouverte", True),
                ("21:16", porte, "ouverte", False)]
    for heure, app, attr, val in scenario:
        bus.heure = heure
        app.changer(attr, val)
    bus.heure = "22:00"
    bus.journal.append("22:00  ⚑ scène « nuit » : tout éteindre sauf la chambre")
    for lampe in (entree, salon):
        lampe.changer("allumee", False)
    print("Journal de la journée :")
    print("\n".join("   " + l for l in bus.journal))
    allumees = [a.nom for a in (entree, salon, chambre) if a.allumee]
    print(f"\nÉtat final : lampes allumées {allumees}, ventilateur {'en marche' if ventilo.marche else 'arrêté'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
