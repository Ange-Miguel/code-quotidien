# Jour 167 — Calcul des charges et du bénéfice mensuel d'une petite entreprise

**Langue :** Python

## Le problème
Beaucoup de petits commerçants regardent seulement l'argent en caisse. Ils ne distinguent pas chiffre d'affaires, marge, charges fixes et bénéfice, et ne savent pas à partir de quel volume de ventes ils gagnent vraiment de l'argent (seuil de rentabilité).

## La solution
À partir des ventes du mois (avec le coût d'achat des produits vendus), on calcule la marge brute, on retire les charges fixes (loyer, salaire, internet, électricité) et variables (commissions Mobile Money, livraisons), puis on obtient le résultat. Le seuil de rentabilité = charges fixes / taux de marge sur coûts variables. On montre l'évolution sur 3 mois et l'effet d'une hausse de 10 % des ventes. Chiffres d'exemple.

## Ce qu'on apprend
Les notions de base de gestion (CA, marge, charges, résultat, seuil de rentabilité), les dataclasses et la présentation d'un compte de résultat simplifié.

## Lancer
```bash
python3 benefice_mensuel.py
```

## Exemple de sortie
```
Charges fixes mensuelles : loyer boutique 60 000, salaire vendeur 50 000, internet + téléphone 15 000, électricité 12 000, publicité réseaux sociaux 20 000

Août 2026
   Chiffre d'affaires              520 000
   − coût des produits vendus      330 000
   = marge brute                   190 000  (37% du CA)
   − charges variables              45 200
   − charges fixes                 157 000
   = RÉSULTAT                      -12 200  PERTE
   Seuil de rentabilité : 563 812 FCFA de ventes (non atteint)

Septembre 2026
   Chiffre d'affaires              610 000
   − coût des produits vendus      380 000
   = marge brute                   230 000  (38% du CA)
```
