"""Jour 095 — Conversion de fuseaux horaires (Cotonou, Paris, Pékin)

Problème : un importateur parle avec un fournisseur à Guangzhou, un client à
Paris et un associé à Montréal. Quelle heure est-il chez chacun, et quel
créneau de réunion convient à tout le monde pendant les heures de bureau ?
Solution : on travaille en temps universel (UTC) et on applique le décalage
de chaque ville. Cotonou (UTC+1) et Pékin (UTC+8) n'ont pas d'heure d'été ;
Paris (UTC+1/+2) et Montréal (UTC−5/−4) en ont une. On calcule nous-mêmes les
dates de changement d'heure (règles de l'Union européenne et de l'Amérique du
Nord), sans base de données de fuseaux, puis on cherche les heures où tout
le monde est entre 8 h et 18 h.
On apprend : l'UTC, les décalages horaires, le calcul du « dernier dimanche
du mois », timezone et timedelta du module datetime.
"""
import sys
from datetime import datetime, timedelta, timezone


def dernier_dimanche(annee: int, mois: int) -> datetime:
    d = datetime(annee + (mois == 12), mois % 12 + 1, 1) - timedelta(days=1)
    return d - timedelta(days=(d.weekday() + 1) % 7)


def nieme_dimanche(annee: int, mois: int, n: int) -> datetime:
    d = datetime(annee, mois, 1)
    d += timedelta(days=(6 - d.weekday()) % 7)
    return d + timedelta(weeks=n - 1)


def decalage_paris(utc: datetime) -> int:
    debut = dernier_dimanche(utc.year, 3).replace(hour=1)
    fin = dernier_dimanche(utc.year, 10).replace(hour=1)
    return 2 if debut <= utc.replace(tzinfo=None) < fin else 1


def decalage_montreal(utc: datetime) -> int:
    debut = nieme_dimanche(utc.year, 3, 2).replace(hour=7)    # 2 h locale = 7 h UTC
    fin = nieme_dimanche(utc.year, 11, 1).replace(hour=6)     # 2 h locale = 6 h UTC
    return -4 if debut <= utc.replace(tzinfo=None) < fin else -5


JOURS = ["lun", "mar", "mer", "jeu", "ven", "sam", "dim"]

VILLES = {"Cotonou": lambda u: 1, "Paris": decalage_paris, "Pékin": lambda u: 8, "Montréal": decalage_montreal}


def heures_locales(utc: datetime) -> dict[str, datetime]:
    return {v: utc + timedelta(hours=f(utc)) for v, f in VILLES.items()}


def afficher(utc: datetime) -> None:
    print(f"À {utc:%Y-%m-%d %H:%M} UTC :")
    for v, h in heures_locales(utc).items():
        print(f"   {v:9} {JOURS[h.weekday()]} {h:%d %H:%M} (UTC{VILLES[v](utc):+d})")


def creneaux_communs(jour: datetime, villes: list[str]) -> list[int]:
    return [h for h in range(24)
            if all(8 <= heures_locales(jour.replace(hour=h))[v].hour < 18 for v in villes)]


def main() -> int:
    if len(sys.argv) == 2:
        try:
            utc = datetime.fromisoformat(sys.argv[1]).replace(tzinfo=timezone.utc)
        except ValueError:
            print("Usage : python3 fuseaux.py [AAAA-MM-JJTHH:MM (heure UTC)]", file=sys.stderr)
            return 1
        afficher(utc)
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 fuseaux.py [AAAA-MM-JJTHH:MM]", file=sys.stderr)
        return 1
    print(f"Changement d'heure 2026 à Paris : {dernier_dimanche(2026, 3):%d/%m} et {dernier_dimanche(2026, 10):%d/%m}\n")
    afficher(datetime(2026, 10, 20, 9, 0, tzinfo=timezone.utc))
    afficher(datetime(2026, 11, 20, 9, 0, tzinfo=timezone.utc))
    jour = datetime(2026, 11, 20, tzinfo=timezone.utc)
    for groupe in (["Cotonou", "Pékin"], ["Cotonou", "Paris", "Pékin"], ["Cotonou", "Paris", "Montréal"]):
        c = creneaux_communs(jour, groupe)
        texte = ", ".join(f"{h:02d}h UTC" for h in c) or "aucun"
        print(f"\nRéunion {' + '.join(groupe)} (8 h–18 h chez tous) : {texte}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
