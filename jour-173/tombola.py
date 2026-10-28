"""Jour 173 — Tirage au sort équitable (tombola) avec graine

Problème : lors d'une tombola, d'un jeu concours sur les réseaux sociaux ou
d'une attribution de places, les participants soupçonnent souvent que le
tirage est truqué. Comment faire un tirage au sort que tout le monde peut
vérifier après coup ?
Solution : un tirage « vérifiable ». Avant le tirage, on publie la liste
exacte des tickets et son empreinte SHA-256 (impossible de la modifier
ensuite sans que cela se voie). La graine du hasard est un élément que
personne ne contrôle et connu de tous après coup (par exemple les numéros
d'un tirage public annoncé à l'avance). Graine + liste → tirage
déterministe : n'importe qui peut relancer le programme et retrouver
exactement les mêmes gagnants. Un participant avec plus de tickets a plus de
chances, proportionnellement.
On apprend : hashlib, random.Random(graine) et le déterminisme, le tirage
sans remise, la pondération et la notion de vérifiabilité.
"""
import hashlib
import random
import sys
from collections import Counter


def empreinte(tickets: list[str]) -> str:
    return hashlib.sha256("\n".join(tickets).encode()).hexdigest()


def tirer(tickets: list[str], graine_publique: str, nb_lots: int) -> list[str]:
    graine = int(hashlib.sha256((graine_publique + empreinte(tickets)).encode()).hexdigest(), 16)
    rng = random.Random(graine)
    gagnants, deja = [], set()
    ordre = tickets[:]
    rng.shuffle(ordre)
    for t in ordre:                                     # un lot maximum par personne
        personne = t.split("#")[0]
        if personne not in deja:
            gagnants.append(t)
            deja.add(personne)
        if len(gagnants) == nb_lots:
            break
    return gagnants


def main() -> int:
    participants = {"Awa": 3, "Kofi": 1, "Inès": 5, "Serge": 2, "Carine": 1, "Yves": 4, "Mireille": 2, "Paul": 1}
    tickets = [f"{nom}#{i + 1}" for nom, n in participants.items() for i in range(n)]
    graine = sys.argv[1] if len(sys.argv) == 2 else "Loto du 28/10/2026 : 07-19-33-41-58"
    if len(sys.argv) > 2 or not graine.strip():
        print("Usage : python3 tombola.py [\"graine publique\"]", file=sys.stderr)
        return 1
    print(f"{len(tickets)} tickets vendus, empreinte publiée AVANT le tirage :\n   {empreinte(tickets)}")
    print(f"Graine publique : « {graine} »\n")
    lots = ["1er lot : téléphone", "2e lot : bon d'achat 20 000 FCFA", "3e lot : casque audio"]
    gagnants = tirer(tickets, graine, len(lots))
    for lot, g in zip(lots, gagnants):
        print(f"   {lot:34} → ticket {g}")
    assert tirer(tickets, graine, len(lots)) == gagnants
    print("\nVérification : relancer le tirage avec la même graine donne les mêmes gagnants ✔")
    triche = tickets + ["Kofi#99"]
    print(f"Si l'organisateur ajoute un ticket après coup, l'empreinte change : {empreinte(triche)[:16]}… ✘")
    victoires = Counter()
    for k in range(3000):                               # équité : 3 000 tirages simulés (1 lot)
        victoires[tirer(tickets, f"test-{k}", 1)[0].split("#")[0]] += 1
    print("\nÉquité sur 3 000 tirages d'un seul lot (chance attendue = part des tickets) :")
    for nom, n in participants.items():
        print(f"   {nom:9} {n} ticket(s) : attendu {n / len(tickets):5.1%}, observé {victoires[nom] / 3000:5.1%}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
