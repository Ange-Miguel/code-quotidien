# Jour 097 — Simulation d'un distributeur automatique (machine à états)

**Langue :** C

## Le problème
Un distributeur de boissons ou de crédit doit réagir correctement à toutes les séquences d'actions : pièces insérées, choix d'un produit épuisé, annulation, monnaie à rendre… Un code plein de « if » imbriqués devient vite impossible à maintenir et plein de bugs.

## La solution
Une machine à états finis. Les états (ATTENTE, PAIEMENT, DISTRIBUTION, RENDU) et les événements (PIECE, CHOIX, ANNULER, FIN) sont des énumérations ; une seule fonction de transition décide du nouvel état et de l'action. On rejoue un scénario d'événements et on trace chaque transition. C'est exactement ainsi qu'on programme les automates sur microcontrôleur.

## Ce qu'on apprend
Les machines à états finis, les enum et switch imbriqués, la séparation entre état, événements et actions, et le traçage.

## Lancer
```bash
gcc -Wall -Wextra -o distributeur distributeur.c -lm
./distributeur
```

## Exemple de sortie
```
 1. CHOIX     Eau 50 cl : 200 FCFA, il manque 200   [ATTENTE → ATTENTE]
 2. PIECE     pièce 100 acceptée, crédit 100   [ATTENTE → PAIEMENT]
 3. PIECE     pièce 25 refusée (rendue)   [PAIEMENT → PAIEMENT]
 4. CHOIX     Jus d'ananas : 500 FCFA, il manque 400   [PAIEMENT → PAIEMENT]
 5. PIECE     pièce 500 acceptée, crédit 600   [PAIEMENT → PAIEMENT]
 6. CHOIX     Soda : épuisé   [PAIEMENT → PAIEMENT]
 7. CHOIX     Jus d'ananas choisi   [PAIEMENT → DISTRIBUTION]
 8. PIECE     occupé, événement ignoré   [DISTRIBUTION → DISTRIBUTION]
 9. FIN       Jus d'ananas distribué   [DISTRIBUTION → RENDU]
10. FIN       monnaie rendue : 100 FCFA   [RENDU → ATTENTE]
11. PIECE     pièce 200 acceptée, crédit 200   [ATTENTE → PAIEMENT]
12. ANNULER   annulation   [PAIEMENT → RENDU]
13. FIN       monnaie rendue : 200 FCFA   [RENDU → ATTENTE]
14. PIECE     pièce 200 acceptée, crédit 200   [ATTENTE → PAIEMENT]
15. CHOIX     Eau 50 cl choisi   [PAIEMENT → DISTRIBUTION]
```
