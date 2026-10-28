"""Jour 053 — Journal des dépenses quotidiennes avec total mensuel

Problème : à la fin du mois, l'argent est parti sans qu'on sache où :
transport, nourriture, crédit téléphone, sorties… Noter ses dépenses ne sert à
rien si on ne les analyse pas.
Solution : les dépenses sont enregistrées dans un fichier CSV (date,
catégorie, montant, note). Le programme calcule le total par mois et par
catégorie, compare au budget fixé pour chaque catégorie et signale les
dépassements, avec un graphique en barres.
On apprend : le module csv, datetime, les regroupements avec defaultdict,
les fichiers temporaires et la présentation d'un rapport.
"""
import csv
import os
import sys
import tempfile
from collections import defaultdict
from datetime import date

BUDGET = {"transport": 15000, "nourriture": 40000, "téléphone": 5000, "sorties": 10000, "études": 10000}


def ajouter(fichier: str, jour: date, categorie: str, montant: int, note: str = "") -> None:
    if montant <= 0:
        raise ValueError("montant positif attendu")
    nouveau = not os.path.exists(fichier)
    with open(fichier, "a", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        if nouveau:
            w.writerow(["date", "categorie", "montant", "note"])
        w.writerow([jour.isoformat(), categorie.lower(), montant, note])


def rapport(fichier: str) -> None:
    par_mois = defaultdict(lambda: defaultdict(int))
    with open(fichier, newline="", encoding="utf-8") as f:
        for ligne in csv.DictReader(f):
            mois = ligne["date"][:7]
            par_mois[mois][ligne["categorie"]] += int(ligne["montant"])
    for mois, categories in sorted(par_mois.items()):
        total = sum(categories.values())
        print(f"Mois {mois} : total {total:,} FCFA".replace(",", " "))
        for cat, montant in sorted(categories.items(), key=lambda x: -x[1]):
            budget = BUDGET.get(cat)
            barre = "█" * (montant * 30 // max(categories.values()))
            alerte = f" ⚠ dépasse le budget de {montant - budget:,}".replace(",", " ") if budget and montant > budget else ""
            print(f"   {cat:11} {montant:>8,} {barre}{alerte}".replace(",", " "))
        print()


def main() -> int:
    if len(sys.argv) == 2:
        if not os.path.isfile(sys.argv[1]):
            print(f"Fichier introuvable : {sys.argv[1]}", file=sys.stderr)
            return 1
        rapport(sys.argv[1])
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 depenses.py [depenses.csv]", file=sys.stderr)
        return 1
    with tempfile.TemporaryDirectory() as dossier:
        fichier = os.path.join(dossier, "depenses.csv")
        exemples = [
            (date(2026, 9, 2), "transport", 800, "zem campus"), (date(2026, 9, 2), "nourriture", 1500, "midi"),
            (date(2026, 9, 5), "téléphone", 2000, "forfait"), (date(2026, 9, 12), "sorties", 7000, "anniversaire"),
            (date(2026, 9, 20), "études", 6500, "polycopiés"), (date(2026, 9, 28), "sorties", 5000, "maquis"),
            (date(2026, 10, 1), "nourriture", 25000, "marché"), (date(2026, 10, 3), "transport", 9000, "semaine"),
            (date(2026, 10, 15), "nourriture", 18000, "marché"), (date(2026, 10, 20), "téléphone", 2000, "forfait"),
        ]
        for e in exemples:
            ajouter(fichier, *e)
        rapport(fichier)
    return 0


if __name__ == "__main__":
    sys.exit(main())
