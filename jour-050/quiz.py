"""Jour 050 — Quiz de culture générale à choix multiples avec score

Problème : réviser en s'amusant, ou animer une soirée, avec un quiz qui
mélange questions et réponses, compte les points et affiche un bilan.
Solution : les questions sont des données (liste de dictionnaires) séparées
du moteur du quiz. Les choix sont mélangés à chaque partie, la bonne réponse
est suivie par son texte et non sa position. Un mode démo fait jouer un
joueur simulé ; le mode --jouer interroge vraiment l'utilisateur.
On apprend : la séparation données / logique, random.shuffle avec une
graine, le calcul de score par catégorie et la validation d'une saisie.
"""
import random
import sys
from collections import defaultdict

QUESTIONS = [
    {"cat": "Bénin", "q": "Quelle est la capitale officielle du Bénin ?",
     "bonne": "Porto-Novo", "fausses": ["Cotonou", "Parakou", "Abomey"]},
    {"cat": "Bénin", "q": "En quelle année le Dahomey est-il devenu indépendant ?",
     "bonne": "1960", "fausses": ["1958", "1975", "1990"]},
    {"cat": "Sciences", "q": "Quelle est l'unité de la résistance électrique ?",
     "bonne": "L'ohm", "fausses": ["Le volt", "L'ampère", "Le watt"]},
    {"cat": "Sciences", "q": "Combien de bits dans un octet ?",
     "bonne": "8", "fausses": ["4", "16", "10"]},
    {"cat": "Informatique", "q": "Qui a créé le langage C ?",
     "bonne": "Dennis Ritchie", "fausses": ["Bill Gates", "Guido van Rossum", "James Gosling"]},
    {"cat": "Informatique", "q": "Que signifie IoT ?",
     "bonne": "Internet des objets", "fausses": ["Interface of Things", "Input Output Test", "Internet Online Tool"]},
    {"cat": "Afrique", "q": "Quel est le plus long fleuve d'Afrique ?",
     "bonne": "Le Nil", "fausses": ["Le Congo", "Le Niger", "Le Zambèze"]},
]


def preparer(rng: random.Random) -> list[dict]:
    questions = [dict(q) for q in QUESTIONS]
    rng.shuffle(questions)
    for q in questions:
        q["choix"] = [q["bonne"], *q["fausses"]]
        rng.shuffle(q["choix"])
    return questions


def jouer(questions: list[dict], repondre) -> None:
    scores = defaultdict(lambda: [0, 0])
    for i, q in enumerate(questions, 1):
        print(f"{i}. {q['q']}")
        for lettre, choix in zip("ABCD", q["choix"]):
            print(f"   {lettre}) {choix}")
        reponse = repondre(q)
        juste = q["choix"]["ABCD".index(reponse)] == q["bonne"]
        scores[q["cat"]][0] += juste
        scores[q["cat"]][1] += 1
        print(f"   → {reponse} : {'bonne réponse !' if juste else 'raté, c était ' + q['bonne']}\n")
    total = sum(s[0] for s in scores.values())
    print(f"Score final : {total}/{len(questions)} ({100 * total / len(questions):.0f} %)")
    for cat, (bon, nb) in scores.items():
        print(f"   {cat:13} {bon}/{nb}")


def saisie(q: dict) -> str:
    while True:
        r = input("   Votre réponse (A-D) : ").strip().upper()
        if r in ("A", "B", "C", "D"):
            return r
        print("   Répondez par A, B, C ou D.")


def main() -> int:
    if sys.argv[1:] == ["--jouer"]:
        jouer(preparer(random.Random()), saisie)
        return 0
    if len(sys.argv) > 1:
        print("Usage : python3 quiz.py [--jouer]", file=sys.stderr)
        return 1
    rng = random.Random(50)
    # Joueur simulé : connaît la bonne réponse 70 % du temps
    simule = lambda q: "ABCD"[q["choix"].index(q["bonne"])] if rng.random() < 0.7 else rng.choice("ABCD")
    jouer(preparer(rng), simule)
    return 0


if __name__ == "__main__":
    sys.exit(main())
