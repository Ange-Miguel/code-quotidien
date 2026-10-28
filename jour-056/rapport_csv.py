"""Jour 056 — Export d'un rapport de ventes en CSV

Problème : le comptable, l'associé ou l'investisseur veut un fichier qu'il
peut ouvrir dans Excel ou Google Sheets : ventes par produit et par semaine,
avec totaux et évolution.
Solution : à partir d'une liste de ventes, on construit un tableau croisé
(produits en lignes, semaines en colonnes), on ajoute les totaux et la
variation entre la première et la dernière semaine, puis on écrit le CSV avec
le séparateur « ; » et l'encodage UTF-8 avec BOM, pour qu'Excel en français
l'ouvre correctement avec les accents.
On apprend : le module csv (DictWriter), isocalendar pour les semaines, le
tableau croisé et les détails pratiques d'encodage des fichiers.
"""
import csv
import os
import sys
import tempfile
from collections import defaultdict
from datetime import date, timedelta
import random

def generer_ventes(seed: int = 56) -> list[tuple[date, str, int]]:
    rng = random.Random(seed)
    produits = {"Écouteurs": 4500, "Chargeur": 2500, "Montre": 12000, "Lampe solaire": 6000}
    debut = date(2026, 9, 7)
    ventes = []
    for jour in range(28):
        for produit, prix in produits.items():
            q = rng.randint(0, 4)
            if q:
                ventes.append((debut + timedelta(days=jour), produit, q * prix))
    return ventes


def tableau_croise(ventes):
    table = defaultdict(lambda: defaultdict(int))
    semaines = set()
    for d, produit, montant in ventes:
        s = f"S{d.isocalendar().week:02d}"
        semaines.add(s)
        table[produit][s] += montant
    return table, sorted(semaines)


def exporter(ventes, chemin: str) -> None:
    table, semaines = tableau_croise(ventes)
    colonnes = ["Produit", *semaines, "Total", "Évolution"]
    with open(chemin, "w", newline="", encoding="utf-8-sig") as f:
        w = csv.DictWriter(f, fieldnames=colonnes, delimiter=";")
        w.writeheader()
        totaux = defaultdict(int)
        for produit in sorted(table):
            ligne = {"Produit": produit}
            for s in semaines:
                ligne[s] = table[produit][s]
                totaux[s] += table[produit][s]
            ligne["Total"] = sum(table[produit].values())
            premier, dernier = table[produit][semaines[0]], table[produit][semaines[-1]]
            ligne["Évolution"] = f"{(dernier - premier) / premier:+.0%}" if premier else "n/a"
            w.writerow(ligne)
        w.writerow({"Produit": "TOTAL", **totaux, "Total": sum(totaux.values()), "Évolution": ""})


def main() -> int:
    if len(sys.argv) > 2:
        print("Usage : python3 rapport_csv.py [fichier_sortie.csv]", file=sys.stderr)
        return 1
    ventes = generer_ventes()
    if len(sys.argv) == 2:
        if not sys.argv[1].endswith(".csv"):
            print("Le fichier de sortie doit finir par .csv", file=sys.stderr)
            return 1
        exporter(ventes, sys.argv[1])
        print(f"Rapport écrit dans {sys.argv[1]}")
        return 0
    with tempfile.TemporaryDirectory() as d:
        chemin = os.path.join(d, "rapport_ventes.csv")
        exporter(ventes, chemin)
        print(f"{len(ventes)} ventes → rapport CSV (aperçu) :\n")
        with open(chemin, encoding="utf-8-sig") as f:
            print(f.read())
    return 0


if __name__ == "__main__":
    sys.exit(main())
