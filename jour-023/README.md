# Jour 023 — Calculateur d'intérêts simples et composés (tontine/épargne)

**Langue :** Python

## Le problème
Quand on épargne (compte, tontine améliorée, placement), on veut savoir combien on aura dans quelques années et comprendre la différence entre intérêts simples et intérêts composés.

## La solution
Intérêts simples : capital × (1 + taux × années). Intérêts composés : capital × (1 + taux/n)^(n × années), avec n capitalisations par an. On ajoute un versement mensuel régulier simulé mois par mois, et la « règle de 72 » pour estimer le temps de doublement.

## Ce qu'on apprend
Les puissances, les simulations mois par mois, la comparaison de formules et la mise en forme de tableaux.

## Lancer
```bash
python3 epargne.py
```

## Exemple de sortie
```
Capital 500 000 FCFA, taux 6.0 %/an, versement 25 000 FCFA/mois
Année            Simples           Composés         + versements
    1       530 000 FCFA       530 839 FCFA         839 228 FCFA
    2       560 000 FCFA       563 580 FCFA       1 199 379 FCFA
    3       590 000 FCFA       598 340 FCFA       1 581 743 FCFA
    4       620 000 FCFA       635 245 FCFA       1 987 690 FCFA
    5       650 000 FCFA       674 425 FCFA       2 418 676 FCFA
Règle de 72 : le capital double en ~12.0 ans à intérêts composés
Total versé : 2 000 000 FCFA → intérêts gagnés : 418 676 FCFA
```
