# Jour 155 — Gestion d'ordonnances : vérification des doublons de médicaments

**Langue :** Python

## Le problème
Un patient consulte plusieurs médecins et reçoit plusieurs ordonnances. Deux médicaments de noms commerciaux différents peuvent contenir la même molécule : il risque d'en prendre deux fois sans le savoir. Un logiciel de pharmacie ou d'hôpital doit le signaler au pharmacien.

## La solution
Un référentiel associe chaque nom commercial à sa ou ses molécules (DCI, dénomination commune internationale) et à sa classe. Pour toutes les ordonnances actives d'un patient, on regroupe les lignes par molécule et on signale : même molécule prescrite deux fois, même classe thérapeutique en double, et médicament inconnu du référentiel. Le programme ne fait qu'ALERTER : la décision revient toujours au pharmacien ou au médecin. Les noms commerciaux sont inventés pour l'exemple.

## Ce qu'on apprend
La normalisation de données de santé, les regroupements par clé, la traçabilité (quelle ordonnance, quel médecin) et la limite d'un outil d'aide à la décision.

## Lancer
```bash
python3 ordonnances.py
```

## Exemple de sortie
```
Patient : 7 lignes sur 3 ordonnances actives

? Siropax (ORD-0423) : inconnu du référentiel, à vérifier manuellement
✘ DOUBLON de molécule « paracétamol » : Dolorax 500 [ORD-0412, Dr A.], Algiduo [ORD-0419, Dr K.]
⚠ Deux anti-inflammatoires différents : DICLOGEL, IBUFLAM 400 (association à confirmer)

Outil d'aide uniquement : chaque alerte doit être validée par le pharmacien ou le médecin.
```
