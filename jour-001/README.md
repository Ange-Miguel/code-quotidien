# Jour 001 — Convertisseur FCFA ↔ euro ↔ dollar

**Langue :** C

## Le problème
Quand on achète en ligne ou qu'on compare des prix, il faut convertir rapidement entre le FCFA, l'euro et le dollar.

## La solution
- Le FCFA (XOF) a une parité **fixe** avec l'euro : 1 EUR = 655,957 XOF.
- Le dollar varie : le taux EUR→USD est passé en argument (1,08 par défaut).
- L'euro sert de monnaie pivot : tout montant est d'abord converti en euros, puis vers les autres devises.

## Ce qu'on apprend
- Lire des arguments en ligne de commande (`argc`, `argv`)
- Convertir du texte en nombre avec `strtod` et vérifier les erreurs
- Organiser le code en petites fonctions

## Lancer
```bash
gcc -Wall -Wextra -o convertisseur convertisseur.c
./convertisseur 10000 XOF
./convertisseur 100 USD 1.10
```
