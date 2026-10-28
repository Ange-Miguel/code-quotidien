"""Jour 125 — Calcul de consommation électrique des appareils d'une maison

Problème : la facture d'électricité monte et on ne sait pas quel appareil en
est responsable. Le congélateur ? Le climatiseur ? Le fer à repasser ? Et que
rapporterait le passage aux ampoules LED ou un climatiseur inverter ?
Solution : chaque appareil est décrit par sa puissance, son usage quotidien
et éventuellement sa veille. On calcule les kWh par mois, la part de chaque
appareil (diagramme en barres texte) et le coût selon le prix du kWh. Puis
on simule des scénarios d'économie et on calcule le temps de retour sur
investissement de chaque changement. Prix et puissances sont des exemples.
On apprend : la différence entre watts et kWh, l'impact de la veille, le
calcul d'un retour sur investissement et la comparaison de scénarios.
"""
import sys
from dataclasses import dataclass, replace

PRIX_KWH = 125        # FCFA, exemple


@dataclass
class Appareil:
    nom: str
    watts: float
    heures: float
    veille_w: float = 0.0
    quantite: int = 1

    def kwh_mois(self) -> float:
        actif = self.watts * self.heures
        veille = self.veille_w * (24 - self.heures)
        return (actif + veille) * self.quantite * 30 / 1000


MAISON = [
    Appareil("Climatiseur", 1100, 6), Appareil("Réfrigérateur", 150, 10), Appareil("Ampoules", 40, 6, quantite=8),
    Appareil("Ventilateur", 60, 8, quantite=2), Appareil("Téléviseur", 90, 4, veille_w=3),
    Appareil("Fer à repasser", 1200, 0.3), Appareil("Décodeur TV", 15, 4, veille_w=8),
    Appareil("Pompe à eau", 750, 0.7), Appareil("Ordinateur", 65, 5), Appareil("Chargeurs", 5, 3, veille_w=0.5, quantite=4),
]


def bilan(appareils: list[Appareil], titre: str) -> float:
    total = sum(a.kwh_mois() for a in appareils)
    print(f"{titre} : {total:.0f} kWh/mois = {total * PRIX_KWH:,.0f} FCFA/mois".replace(",", " "))
    return total


def detail(appareils: list[Appareil]) -> None:
    total = sum(a.kwh_mois() for a in appareils)
    for a in sorted(appareils, key=lambda a: -a.kwh_mois()):
        part = a.kwh_mois() / total
        print(f"   {a.nom:15} {a.kwh_mois():6.1f} kWh {part:5.1%} {'█' * round(part * 50)}")


def scenario(nom: str, avant: list[Appareil], apres: list[Appareil], investissement: int) -> None:
    economie = (sum(a.kwh_mois() for a in avant) - sum(a.kwh_mois() for a in apres)) * PRIX_KWH
    retour = investissement / economie if economie > 0 else float("inf")
    print(f"   {nom:38} économie {economie:7,.0f} FCFA/mois, investissement {investissement:8,} → "
          f"rentabilisé en {retour:4.1f} mois".replace(",", " "))


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    bilan(MAISON, "Consommation actuelle")
    detail(MAISON)
    print("\nScénarios d'économie (prix indicatifs) :")
    remplace = lambda nom, **chg: [replace(a, **chg) if a.nom == nom else a for a in MAISON]
    scenario("Ampoules LED (40 W → 9 W)", MAISON, remplace("Ampoules", watts=9), 8 * 1500)
    scenario("Climatiseur inverter (-40 %)", MAISON, remplace("Climatiseur", watts=660), 250_000)
    scenario("Clim 2 h de moins par jour", MAISON, remplace("Climatiseur", heures=4), 0 + 1)
    scenario("Couper la veille TV + décodeur", MAISON,
             [replace(a, veille_w=0) if a.nom in ("Téléviseur", "Décodeur TV") else a for a in MAISON], 3000)
    return 0


if __name__ == "__main__":
    sys.exit(main())
