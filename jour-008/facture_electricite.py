"""Jour 008 — Calculateur de facture d'électricité selon la tranche

Problème : les factures d'électricité domestiques sont souvent calculées par
tranches de consommation : plus on consomme, plus le kWh coûte cher. Difficile
de prévoir sa facture à la main.
Solution : on découpe la consommation mensuelle en tranches et on applique à
chaque morceau son propre prix, puis on ajoute la redevance fixe et la TVA.
Les tarifs utilisés sont des valeurs d'exemple modifiables en haut du fichier.
On apprend : les listes de tuples, le calcul par tranches (comme l'impôt
progressif), l'arrondi et la mise en forme de montants.
"""
import sys

# (limite haute de la tranche en kWh, prix du kWh en FCFA) — valeurs d'exemple
TRANCHES = [(20, 79), (250, 113), (float("inf"), 129)]
REDEVANCE = 500   # FCFA par mois, exemple
TVA = 0.18


def facture(kwh: float) -> dict:
    if kwh < 0:
        raise ValueError("consommation négative")
    lignes, debut = [], 0.0
    for limite, prix in TRANCHES:
        if kwh <= debut:
            break
        quantite = min(kwh, limite) - debut
        lignes.append((debut, min(kwh, limite), quantite, prix, quantite * prix))
        debut = limite
    hors_taxe = sum(l[4] for l in lignes) + REDEVANCE
    return {"lignes": lignes, "ht": hors_taxe, "tva": hors_taxe * TVA, "ttc": hors_taxe * (1 + TVA)}


def afficher(kwh: float) -> None:
    f = facture(kwh)
    print(f"Consommation : {kwh:.0f} kWh")
    for debut, fin, q, prix, montant in f["lignes"]:
        print(f"  {debut:>5.0f} → {fin:>5.0f} kWh : {q:6.0f} × {prix} = {montant:>9,.0f} FCFA".replace(",", " "))
    print(f"  Redevance fixe : {REDEVANCE} FCFA")
    print(f"  Hors taxe : {f['ht']:,.0f} | TVA 18 % : {f['tva']:,.0f}".replace(",", " "))
    print(f"  TOTAL : {f['ttc']:,.0f} FCFA\n".replace(",", " "))


def main() -> int:
    if len(sys.argv) == 1:
        for kwh in (15, 120, 400):
            afficher(kwh)
        return 0
    try:
        afficher(float(sys.argv[1]))
    except ValueError as e:
        print(f"Erreur : {e}. Usage : python3 facture_electricite.py <kWh>", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
