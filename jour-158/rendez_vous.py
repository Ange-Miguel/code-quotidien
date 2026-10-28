"""Jour 158 — Prise de rendez-vous médical sans chevauchement

Problème : un cabinet médical gère ses rendez-vous dans un cahier ; deux
patients se retrouvent sur le même créneau, ou un médecin est réservé
pendant sa pause. Une application doit proposer uniquement des créneaux
réellement libres, selon la durée de chaque type de consultation.
Solution : pour chaque médecin, les rendez-vous sont gardés triés par heure
de début (module bisect). Deux intervalles [a, b[ et [c, d[ se chevauchent
si a < d et c < b. Pour proposer des créneaux, on parcourt les plages
d'ouverture et on « saute » les rendez-vous existants, avec une marge de
5 minutes entre deux patients. On gère aussi l'annulation, qui libère le
créneau.
On apprend : la représentation d'intervalles de temps, le test de
chevauchement, bisect.insort pour garder une liste triée et la recherche de
créneaux libres.
"""
import bisect
import sys
from datetime import datetime, timedelta

DUREES = {"consultation": 20, "suivi": 15, "échographie": 30, "vaccination": 10}
MARGE = timedelta(minutes=5)


class Agenda:
    def __init__(self, medecin: str, plages: list[tuple[str, str]], jour: datetime):
        self.medecin = medecin
        self.plages = [(jour.replace(hour=int(a[:2]), minute=int(a[3:])), jour.replace(hour=int(b[:2]), minute=int(b[3:])))
                       for a, b in plages]
        self.rdv: list[tuple[datetime, datetime, str]] = []

    def libre(self, debut: datetime, fin: datetime) -> bool:
        if not any(a <= debut and fin <= b for a, b in self.plages):
            return False
        return all(not (debut < f + MARGE and d < fin + MARGE) for d, f, _ in self.rdv)

    def reserver(self, patient: str, debut: datetime, type_: str) -> bool:
        fin = debut + timedelta(minutes=DUREES[type_])
        if not self.libre(debut, fin):
            return False
        bisect.insort(self.rdv, (debut, fin, f"{patient} ({type_})"))
        return True

    def annuler(self, patient: str) -> bool:
        for i, (_, _, nom) in enumerate(self.rdv):
            if nom.startswith(patient):
                del self.rdv[i]
                return True
        return False

    def creneaux(self, type_: str, nombre: int = 4) -> list[datetime]:
        duree, trouves = timedelta(minutes=DUREES[type_]), []
        for a, b in self.plages:
            t = a
            while t + duree <= b and len(trouves) < nombre:
                if self.libre(t, t + duree):
                    trouves.append(t)
                    t += duree + MARGE
                else:
                    t += timedelta(minutes=5)
        return trouves


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    jour = datetime(2026, 11, 3)
    a = Agenda("Dr Houngbo", [("08:00", "12:00"), ("15:00", "17:00")], jour)
    demandes = [("Awa", "08:00", "consultation"), ("Kofi", "08:10", "suivi"), ("Kofi", "08:25", "suivi"),
                ("Carine", "11:45", "échographie"), ("Carine", "09:00", "échographie"), ("Yves", "12:30", "vaccination"),
                ("Inès", "09:20", "consultation"), ("Paul", "09:35", "vaccination")]
    for patient, heure, type_ in demandes:
        t = jour.replace(hour=int(heure[:2]), minute=int(heure[3:]))
        ok = a.reserver(patient, t, type_)
        print(f"{patient:7} {heure} {type_:13} → {'réservé' if ok else 'refusé (chevauchement ou hors horaires)'}")
    print(f"\nCarine annule son échographie : {'ok' if a.annuler('Carine') else 'introuvable'}")
    t = jour.replace(hour=9, minute=0)
    print(f"Inès demande le créneau libéré de 09:00 → {'réservé' if a.reserver('Inès', t, 'consultation') else 'refusé'}")
    print(f"\nAgenda de {a.medecin} le {jour:%d/%m/%Y} :")
    for d, f, nom in a.rdv:
        print(f"   {d:%H:%M}-{f:%H:%M}  {nom}")
    for type_ in ("échographie", "vaccination"):
        print(f"Prochains créneaux libres pour une {type_} : " + ", ".join(f"{t:%H:%M}" for t in a.creneaux(type_)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
