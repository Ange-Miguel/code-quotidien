# Jour 011 — Vérificateur de force d'un mot de passe

**Langue :** Python

## Le problème
Beaucoup de comptes sont piratés parce que le mot de passe est trop court, trop simple ou trop courant (« 123456 », « azerty », « password »).

## La solution
On estime l'entropie (en bits) à partir de la taille de l'alphabet utilisé et de la longueur, puis on applique des pénalités : mot de passe courant, répétitions, suites de clavier ou de chiffres. On en déduit une note et des conseils concrets.

## Ce qu'on apprend
Les ensembles (set), les méthodes de chaînes (isdigit, isupper…), le logarithme et la notion d'entropie en sécurité.

## Lancer
```bash
python3 force_mot_de_passe.py
```

## Exemple de sortie
```
'123456'                 →   3.5 bits : Très faible
      - ajouter des minuscules
      - ajouter des majuscules
      - ajouter des symboles (!, ?, #…)
      - viser au moins 12 caractères
      - ce mot de passe fait partie des plus utilisés au monde
      - éviter les suites (abcd, 1234, azer…)
'Benin2026'              →  53.6 bits : Moyen
      - ajouter des symboles (!, ?, #…)
      - viser au moins 12 caractères
'azerty1234'             →  36.2 bits : Moyen
      - ajouter des majuscules
      - ajouter des symboles (!, ?, #…)
      - viser au moins 12 caractères
      - éviter les suites (abcd, 1234, azer…)
```
