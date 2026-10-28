"""Jour 065 — Gestion de bibliothèque : emprunts et retours de livres

Problème : une bibliothèque de quartier, d'association ou de département
prête des livres sur papier ; on perd la trace de qui a quoi et des retards.
Solution : trois classes (Livre, Membre, Bibliotheque). Les règles métier sont
dans les méthodes : 3 emprunts maximum par membre, 14 jours de prêt, pas
d'emprunt si le membre a un retard, file de réservation quand le livre est
déjà sorti, pénalité de 50 FCFA par jour de retard. Des exceptions dédiées
expliquent chaque refus.
On apprend : la programmation orientée objet, les exceptions personnalisées,
les files de réservation (deque) et l'encapsulation des règles de gestion.
"""
import sys
from collections import deque
from dataclasses import dataclass, field
from datetime import date, timedelta

DUREE_PRET = timedelta(days=14)
MAX_EMPRUNTS = 3
PENALITE_JOUR = 50


class RefusEmprunt(Exception):
    pass


@dataclass
class Livre:
    code: str
    titre: str
    emprunteur: "Membre | None" = None
    retour_prevu: date | None = None
    reservations: deque = field(default_factory=deque)


@dataclass
class Membre:
    nom: str
    livres: list = field(default_factory=list)
    penalites: int = 0


class Bibliotheque:
    def __init__(self):
        self.livres: dict[str, Livre] = {}

    def ajouter(self, code: str, titre: str) -> None:
        self.livres[code] = Livre(code, titre)

    def emprunter(self, membre: Membre, code: str, jour: date) -> None:
        livre = self.livres.get(code)
        if livre is None:
            raise RefusEmprunt("livre inconnu")
        if any(l.retour_prevu < jour for l in membre.livres):
            raise RefusEmprunt("retard en cours, rendez d'abord vos livres")
        if len(membre.livres) >= MAX_EMPRUNTS:
            raise RefusEmprunt(f"maximum {MAX_EMPRUNTS} livres")
        if livre.emprunteur:
            if membre not in livre.reservations:
                livre.reservations.append(membre)
            raise RefusEmprunt(f"déjà emprunté → réservé ({len(livre.reservations)} en attente)")
        if livre.reservations and livre.reservations[0] is not membre:
            raise RefusEmprunt(f"réservé pour {livre.reservations[0].nom}")
        if livre.reservations:
            livre.reservations.popleft()
        livre.emprunteur, livre.retour_prevu = membre, jour + DUREE_PRET
        membre.livres.append(livre)

    def rendre(self, code: str, jour: date) -> int:
        livre = self.livres[code]
        membre = livre.emprunteur
        retard = max(0, (jour - livre.retour_prevu).days)
        membre.penalites += retard * PENALITE_JOUR
        membre.livres.remove(livre)
        livre.emprunteur = livre.retour_prevu = None
        return retard


def action(texte: str, f, *args) -> None:
    try:
        r = f(*args)
        print(f"✔ {texte}" + (f" (retard {r} j)" if r else ""))
    except RefusEmprunt as e:
        print(f"✘ {texte} : {e}")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    b = Bibliotheque()
    for code, titre in [("L1", "Le Langage C"), ("L2", "Électronique pour les nuls"),
                        ("L3", "Une si longue lettre"), ("L4", "L'Enfant noir")]:
        b.ajouter(code, titre)
    awa, kofi = Membre("Awa"), Membre("Kofi")
    j = date(2026, 10, 1)
    action("Awa emprunte L1", b.emprunter, awa, "L1", j)
    action("Kofi emprunte L1", b.emprunter, kofi, "L1", j)
    action("Awa emprunte L2", b.emprunter, awa, "L2", j)
    action("Awa emprunte L3", b.emprunter, awa, "L3", j)
    action("Awa emprunte L4", b.emprunter, awa, "L4", j)
    action("Awa rend L1 le 20/10", b.rendre, "L1", date(2026, 10, 20))
    action("Awa emprunte L4 le 20/10", b.emprunter, awa, "L4", date(2026, 10, 20))
    action("Kofi emprunte L1 (sa réservation)", b.emprunter, kofi, "L1", date(2026, 10, 20))
    print(f"\nPénalités d'Awa : {awa.penalites} FCFA")
    for l in b.livres.values():
        etat = f"chez {l.emprunteur.nom} jusqu'au {l.retour_prevu}" if l.emprunteur else "disponible"
        print(f"   {l.code} {l.titre:28} {etat}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
