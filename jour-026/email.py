"""Jour 026 — Validateur d'adresse e-mail

Problème : une faute de frappe dans un e-mail (« gmai.com », espace, double @)
fait perdre un client ou une confirmation de commande.
Solution : on vérifie la structure locale@domaine avec des règles simples et
explicables (une expression régulière par partie, longueurs maximales), puis on
détecte les fautes courantes sur les domaines connus avec la distance de
Levenshtein pour proposer une correction (« gmai.com » → « gmail.com »).
On apprend : les expressions régulières (module re), la décomposition d'un
problème en règles, et une première distance d'édition entre deux mots.
"""
import re
import sys

LOCAL = re.compile(r"^[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+(\.[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+)*$")
ETIQUETTE = re.compile(r"^[A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?$")
CONNUS = ["gmail.com", "yahoo.fr", "yahoo.com", "hotmail.com", "outlook.com", "icloud.com", "live.fr"]


def distance(a: str, b: str) -> int:
    prec = list(range(len(b) + 1))
    for i, ca in enumerate(a, 1):
        cour = [i]
        for j, cb in enumerate(b, 1):
            cour.append(min(prec[j] + 1, cour[j - 1] + 1, prec[j - 1] + (ca != cb)))
        prec = cour
    return prec[-1]


def valider(adresse: str) -> tuple[bool, str]:
    if adresse != adresse.strip() or " " in adresse:
        return False, "contient des espaces"
    if adresse.count("@") != 1:
        return False, "doit contenir exactement un @"
    local, domaine = adresse.split("@")
    if not local or len(local) > 64 or not LOCAL.match(local):
        return False, "partie avant le @ invalide"
    etiquettes = domaine.lower().split(".")
    if len(etiquettes) < 2 or len(domaine) > 253:
        return False, "domaine incomplet (ex. gmail.com)"
    if not all(ETIQUETTE.match(e) for e in etiquettes):
        return False, "domaine invalide"
    if not etiquettes[-1].isalpha() or len(etiquettes[-1]) < 2:
        return False, "extension invalide (.com, .bj, .fr…)"
    d = domaine.lower()
    proche = min(CONNUS, key=lambda c: distance(d, c))
    if 0 < distance(d, proche) <= 2:
        return True, f"valide, mais vouliez-vous dire {local}@{proche} ?"
    return True, "valide"


def main() -> int:
    adresses = sys.argv[1:] or ["angemiguel@gmail.com", "client@gmai.com", "contact@orvix.bj",
                                "mauvais@@mail.com", "sans-domaine@", "jean dupont@yahoo.fr",
                                "a.b-c@sous.domaine.org", "zzz"]
    toutes_valides = True
    for a in adresses:
        ok, message = valider(a)
        toutes_valides &= ok
        print(f"{a:28} → {'✔' if ok else '✘'} {message}")
    return 0 if toutes_valides or not sys.argv[1:] else 1


if __name__ == "__main__":
    sys.exit(main())
