"""Jour 170 — Simulateur de crédit et comparaison d'offres

Problème : pour financer une moto, du matériel ou un stock, plusieurs offres
de crédit sont proposées (banque, microfinance, vendeur à crédit). Le taux
affiché ne dit pas tout : frais de dossier, assurance obligatoire, durée et
type de taux (dégressif ou « flat ») changent complètement le coût réel.
Solution : pour chaque offre on calcule l'échéancier, le coût total et le
taux effectif global (TEG), c'est-à-dire le taux qui égalise le montant
réellement reçu (après frais) et la somme actualisée des mensualités. On le
trouve par dichotomie, faute de formule directe. On montre le piège du taux
« flat » (intérêts calculés sur le capital initial pendant toute la durée).
Offres fictives ; ce programme ne remplace pas le conseil d'un professionnel.
On apprend : l'annuité constante, le taux flat, la valeur actuelle, la
résolution d'équation par dichotomie et la comparaison d'offres.
"""
import sys
from dataclasses import dataclass


@dataclass
class Offre:
    nom: str
    taux_annuel: float
    mois: int
    frais_dossier: float
    assurance_mensuelle_pct: float     # en % du capital initial, par mois
    flat: bool = False

    def mensualite(self, capital: float) -> float:
        t = self.taux_annuel / 12
        if self.flat:
            base = capital / self.mois + capital * t                 # intérêts toujours sur le capital initial
        else:
            base = capital * t / (1 - (1 + t) ** -self.mois) if t else capital / self.mois
        return base + capital * self.assurance_mensuelle_pct / 100


def valeur_actuelle(mensualite: float, mois: int, taux_mensuel: float) -> float:
    return mensualite * (1 - (1 + taux_mensuel) ** -mois) / taux_mensuel if taux_mensuel else mensualite * mois


def teg(offre: Offre, capital: float) -> float:
    recu, m = capital - offre.frais_dossier, offre.mensualite(capital)
    bas, haut = 1e-9, 1.0                                             # taux mensuel entre 0 et 100 %
    for _ in range(100):                                              # dichotomie
        milieu = (bas + haut) / 2
        if valeur_actuelle(m, offre.mois, milieu) > recu:
            bas = milieu
        else:
            haut = milieu
    return ((1 + bas) ** 12 - 1)


def main() -> int:
    capital = 1_000_000
    if len(sys.argv) == 2:
        try:
            capital = float(sys.argv[1])
            if not 10_000 <= capital <= 100_000_000:
                raise ValueError
        except ValueError:
            print("Usage : python3 comparaison_credits.py [montant entre 10 000 et 100 000 000 FCFA]", file=sys.stderr)
            return 1
    elif len(sys.argv) > 2:
        print("Usage : python3 comparaison_credits.py [montant]", file=sys.stderr)
        return 1
    offres = [Offre("Banque (taux dégressif)", 0.11, 24, 25_000, 0.05),
              Offre("Microfinance (taux flat)", 0.09, 24, 10_000, 0.0, flat=True),
              Offre("Crédit vendeur 12 mois", 0.15, 12, 0, 0.0),
              Offre("Banque 36 mois", 0.10, 36, 25_000, 0.05)]
    print(f"Montant emprunté : {capital:,.0f} FCFA\n".replace(",", " "))
    print(f"{'Offre':26} {'Taux affiché':>12} {'Mensualité':>11} {'Coût total':>11} {'TEG':>7}")
    resultats = []
    for o in offres:
        m = o.mensualite(capital)
        cout = m * o.mois + o.frais_dossier - capital
        t = teg(o, capital)
        resultats.append((t, o.nom))
        print(f"{o.nom:26} {o.taux_annuel:11.1%} {m:11,.0f} {cout:11,.0f} {t:7.1%}".replace(",", " "))
    meilleur = min(resultats)
    print(f"\nOffre la moins chère en coût réel (TEG) : {meilleur[1]}")
    print("Remarque : le taux « flat » de 9 % coûte en réalité bien plus qu'un taux dégressif de 11 %.")
    print("Informations indicatives : faites-vous conseiller avant de signer un contrat de crédit.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
