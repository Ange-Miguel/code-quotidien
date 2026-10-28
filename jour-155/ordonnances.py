"""Jour 155 — Gestion d'ordonnances : vérification des doublons de médicaments

Problème : un patient consulte plusieurs médecins et reçoit plusieurs
ordonnances. Deux médicaments de noms commerciaux différents peuvent
contenir la même molécule : il risque d'en prendre deux fois sans le savoir.
Un logiciel de pharmacie ou d'hôpital doit le signaler au pharmacien.
Solution : un référentiel associe chaque nom commercial à sa ou ses
molécules (DCI, dénomination commune internationale) et à sa classe. Pour
toutes les ordonnances actives d'un patient, on regroupe les lignes par
molécule et on signale : même molécule prescrite deux fois, même classe
thérapeutique en double, et médicament inconnu du référentiel. Le programme
ne fait qu'ALERTER : la décision revient toujours au pharmacien ou au
médecin. Les noms commerciaux sont inventés pour l'exemple.
On apprend : la normalisation de données de santé, les regroupements par
clé, la traçabilité (quelle ordonnance, quel médecin) et la limite d'un
outil d'aide à la décision.
"""
import sys
from collections import defaultdict
from dataclasses import dataclass

# Référentiel fictif : nom commercial → (molécules, classe)
REFERENTIEL = {
    "DOLORAX 500": ({"paracétamol"}, "antalgique"),
    "FEBRINOL": ({"paracétamol"}, "antalgique"),
    "ALGIDUO": ({"paracétamol", "codéine"}, "antalgique"),
    "IBUFLAM 400": ({"ibuprofène"}, "anti-inflammatoire"),
    "DICLOGEL": ({"diclofénac"}, "anti-inflammatoire"),
    "AMOXIL-B": ({"amoxicilline"}, "antibiotique"),
    "PALUSTOP": ({"artéméther", "luméfantrine"}, "antipaludique"),
    "GASTRIPROT": ({"oméprazole"}, "protecteur gastrique"),
}


@dataclass
class Ligne:
    medicament: str
    ordonnance: str
    medecin: str


def normaliser(nom: str) -> str:
    return " ".join(nom.upper().split())


def analyser(lignes: list[Ligne]) -> list[str]:
    alertes = []
    par_molecule, par_classe = defaultdict(list), defaultdict(list)
    for l in lignes:
        ref = REFERENTIEL.get(normaliser(l.medicament))
        if ref is None:
            alertes.append(f"? {l.medicament} ({l.ordonnance}) : inconnu du référentiel, à vérifier manuellement")
            continue
        molecules, classe = ref
        for m in molecules:
            par_molecule[m].append(l)
        par_classe[classe].append(l)
    for m, ls in par_molecule.items():
        if len(ls) > 1:
            source = ", ".join(f"{l.medicament} [{l.ordonnance}, {l.medecin}]" for l in ls)
            alertes.append(f"✘ DOUBLON de molécule « {m} » : {source}")
    for c, ls in par_classe.items():
        noms = {normaliser(l.medicament) for l in ls}
        molecules = set().union(*(REFERENTIEL[n][0] for n in noms))
        if len(noms) > 1 and len(molecules) == sum(len(REFERENTIEL[n][0]) for n in noms):
            alertes.append(f"⚠ Deux {c}s différents : {', '.join(sorted(noms))} (association à confirmer)")
    return alertes


def main() -> int:
    if len(sys.argv) > 1:
        print("Ce programme est une démonstration : lancez-le sans argument.", file=sys.stderr)
        return 1
    patient = [Ligne("Dolorax 500", "ORD-0412", "Dr A."), Ligne("Amoxil-B", "ORD-0412", "Dr A."),
               Ligne("Algiduo", "ORD-0419", "Dr K."), Ligne("Ibuflam 400", "ORD-0419", "Dr K."),
               Ligne("Diclogel", "ORD-0423", "Dr Z."), Ligne("Gastriprot", "ORD-0423", "Dr Z."),
               Ligne("Siropax", "ORD-0423", "Dr Z.")]
    print(f"Patient : {len(patient)} lignes sur {len({l.ordonnance for l in patient})} ordonnances actives\n")
    for a in analyser(patient):
        print(a)
    print("\nOutil d'aide uniquement : chaque alerte doit être validée par le pharmacien ou le médecin.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
