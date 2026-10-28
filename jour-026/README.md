# Jour 026 — Validateur d'adresse e-mail simple

**Langue :** Python

## Le problème
Une faute de frappe dans un e-mail (« gmai.com », espace, double @) fait perdre un client ou une confirmation de commande.

## La solution
On vérifie la structure locale@domaine avec des règles simples et explicables (une expression régulière par partie, longueurs maximales), puis on détecte les fautes courantes sur les domaines connus avec la distance de Levenshtein pour proposer une correction (« gmai.com » → « gmail.com »).

## Ce qu'on apprend
Les expressions régulières (module re), la décomposition d'un problème en règles, et une première distance d'édition entre deux mots.

## Lancer
```bash
python3 email.py
```

## Exemple de sortie
```
angemiguel@gmail.com         → ✔ valide
client@gmai.com              → ✔ valide, mais vouliez-vous dire client@gmail.com ?
contact@orvix.bj             → ✔ valide
mauvais@@mail.com            → ✘ doit contenir exactement un @
sans-domaine@                → ✘ domaine incomplet (ex. gmail.com)
jean dupont@yahoo.fr         → ✘ contient des espaces
a.b-c@sous.domaine.org       → ✔ valide
zzz                          → ✘ doit contenir exactement un @
```
