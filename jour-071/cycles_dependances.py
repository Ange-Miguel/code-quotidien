"""Jour 071 — Détection de cycle dans des dépendances de tâches

Problème : dans un projet (construction, logiciel, organisation d'événement),
certaines tâches dépendent d'autres. Si A attend B, B attend C et C attend A,
le projet est bloqué pour toujours : c'est un cycle, souvent difficile à voir.
Solution : on représente les dépendances par un graphe orienté et on fait un
parcours en profondeur (DFS) avec trois couleurs : blanc (non visité), gris
(en cours d'exploration), noir (terminé). Rencontrer un sommet gris signifie
qu'on est revenu sur ses pas : il y a un cycle, qu'on reconstruit et affiche.
On apprend : les graphes orientés, le DFS itératif et récursif, la méthode des
trois couleurs et la reconstruction d'un cycle à partir de la pile.
"""
import sys

BLANC, GRIS, NOIR = 0, 1, 2


def trouver_cycle(deps: dict[str, list[str]]) -> list[str] | None:
    couleur = {t: BLANC for t in deps}
    for d in list(deps.values()):
        for t in d:
            couleur.setdefault(t, BLANC)
    pile = []

    def visiter(t: str) -> list[str] | None:
        couleur[t] = GRIS
        pile.append(t)
        for suivant in deps.get(t, []):
            if couleur[suivant] == GRIS:
                return pile[pile.index(suivant):] + [suivant]
            if couleur[suivant] == BLANC:
                cycle = visiter(suivant)
                if cycle:
                    return cycle
        pile.pop()
        couleur[t] = NOIR
        return None

    for t in couleur:
        if couleur[t] == BLANC:
            cycle = visiter(t)
            if cycle:
                return cycle
    return None


def analyser(nom: str, deps: dict[str, list[str]]) -> bool:
    print(f"Projet « {nom} » :")
    for tache, avant in deps.items():
        print(f"   {tache:24} attend : {', '.join(avant) or '—'}")
    cycle = trouver_cycle(deps)
    if cycle:
        print("   ✘ BLOCAGE : " + " → attend → ".join(cycle) + "\n")
        return False
    print("   ✔ Aucun cycle : le projet peut avancer\n")
    return True


def main() -> int:
    if len(sys.argv) > 1:
        # Format : "A>B" signifie « A attend B »
        deps: dict[str, list[str]] = {}
        for arg in sys.argv[1:]:
            if arg.count(">") != 1 or not all(arg.split(">")):
                print("Usage : python3 cycles_dependances.py \"A>B\" \"B>C\" ...", file=sys.stderr)
                return 1
            a, b = arg.split(">")
            deps.setdefault(a, []).append(b)
        return 0 if analyser("personnalisé", deps) else 2
    analyser("Boutique en ligne", {
        "Mise en ligne": ["Paiement mobile", "Catalogue"],
        "Paiement mobile": ["Compte marchand"],
        "Catalogue": ["Photos produits"],
        "Photos produits": [],
        "Compte marchand": ["Statuts société"],
        "Statuts société": [],
    })
    analyser("Dossier administratif", {
        "Ouvrir compte bancaire": ["Obtenir RCCM"],
        "Obtenir RCCM": ["Justificatif de domicile"],
        "Justificatif de domicile": ["Facture à son nom"],
        "Facture à son nom": ["Ouvrir compte bancaire"],
    })
    return 0


if __name__ == "__main__":
    sys.exit(main())
