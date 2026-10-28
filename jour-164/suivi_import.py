"""Jour 164 — Suivi de commande import Chine : étapes et délais

Problème : un importateur passe commande chez un fournisseur chinois ; la
marchandise passe par la production, l'entrepôt du transitaire, le transport
(aérien ou maritime), le dédouanement à Cotonou puis la livraison. Les
clients demandent sans cesse « où en est ma commande ? » et les retards ne
sont découverts que trop tard.
Solution : chaque mode de transport a ses étapes avec une durée typique.
Pour une commande, on enregistre les dates réelles des étapes franchies ;
le programme calcule l'étape en cours, le retard accumulé par rapport au
plan, une date d'arrivée réestimée (durées restantes ajoutées à la date
réelle de la dernière étape) et une alerte si le retard dépasse un seuil.
Les durées sont indicatives.
On apprend : la modélisation d'un processus par étapes, datetime et
timedelta, la réestimation dynamique d'une échéance et les alertes.
"""
import sys
from datetime import date, timedelta

ETAPES = {
    "aérien": [("Production fournisseur", 7), ("Envoi à l'entrepôt du transitaire", 3), ("Vol Guangzhou → Cotonou", 6),
               ("Dédouanement", 4), ("Livraison au client", 1)],
    "maritime": [("Production fournisseur", 10), ("Envoi à l'entrepôt du transitaire", 4), ("Groupage conteneur", 7),
                 ("Traversée maritime", 40), ("Port de Cotonou et dédouanement", 8), ("Livraison au client", 2)],
}


def suivre(nom: str, mode: str, commande: date, faites: list[date], aujourdhui: date, seuil: int = 5) -> None:
    etapes = ETAPES[mode]
    if len(faites) > len(etapes):
        raise ValueError("plus de dates que d'étapes")
    prevu, cumul = [], commande
    for _, jours in etapes:
        cumul += timedelta(days=jours)
        prevu.append(cumul)
    print(f"{nom} — {mode}, commandé le {commande:%d/%m/%Y}")
    for i, (etape, _) in enumerate(etapes):
        if i < len(faites):
            ecart = (faites[i] - prevu[i]).days
            print(f"   ✔ {etape:34} {faites[i]:%d/%m}  ({'+' if ecart > 0 else ''}{ecart} j)" if ecart else f"   ✔ {etape:34} {faites[i]:%d/%m}  (à l'heure)")
        elif i == len(faites):
            print(f"   ▶ {etape:34} en cours (prévu le {prevu[i]:%d/%m})")
        else:
            print(f"     {etape:34} prévu le {prevu[i]:%d/%m}")
    base = faites[-1] if faites else commande
    restant = sum(j for _, j in etapes[len(faites):])
    estimation = max(base + timedelta(days=restant), aujourdhui)
    retard = (estimation - prevu[-1]).days
    etat = "LIVRÉE" if len(faites) == len(etapes) else f"arrivée estimée le {estimation:%d/%m/%Y}"
    print(f"   → {etat}, retard {max(retard, 0)} jour(s)" + ("  ⚠ prévenir le client" if retard > seuil else "") + "\n")


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    aujourdhui = date(2026, 10, 28)
    suivre("CMD-0091 écouteurs ×100", "aérien", date(2026, 10, 10),
           [date(2026, 10, 16), date(2026, 10, 19), date(2026, 10, 25)], aujourdhui)
    suivre("CMD-0087 ventilateurs ×60", "maritime", date(2026, 8, 20),
           [date(2026, 9, 3), date(2026, 9, 8), date(2026, 9, 20)], aujourdhui)
    suivre("CMD-0079 montres ×40", "aérien", date(2026, 9, 1),
           [date(2026, 9, 8), date(2026, 9, 11), date(2026, 9, 17), date(2026, 9, 21), date(2026, 9, 22)], aujourdhui)
    return 0


if __name__ == "__main__":
    sys.exit(main())
