# Jour 170 — Simulateur de crédit et comparaison d'offres

**Langue :** Python

## Le problème
Pour financer une moto, du matériel ou un stock, plusieurs offres de crédit sont proposées (banque, microfinance, vendeur à crédit). Le taux affiché ne dit pas tout : frais de dossier, assurance obligatoire, durée et type de taux (dégressif ou « flat ») changent complètement le coût réel.

## La solution
Pour chaque offre on calcule l'échéancier, le coût total et le taux effectif global (TEG), c'est-à-dire le taux qui égalise le montant réellement reçu (après frais) et la somme actualisée des mensualités. On le trouve par dichotomie, faute de formule directe. On montre le piège du taux « flat » (intérêts calculés sur le capital initial pendant toute la durée). Offres fictives ; ce programme ne remplace pas le conseil d'un professionnel.

## Ce qu'on apprend
L'annuité constante, le taux flat, la valeur actuelle, la résolution d'équation par dichotomie et la comparaison d'offres.

## Lancer
```bash
python3 comparaison_credits.py
```

## Exemple de sortie
```
Montant emprunté : 1 000 000 FCFA

Offre                      Taux affiché  Mensualité  Coût total     TEG
Banque (taux dégressif)          11.0%      47 108     155 588   15.7%
Microfinance (taux flat)          9.0%      49 167     190 000   18.9%
Crédit vendeur 12 mois           15.0%      90 258      83 100   16.1%
Banque 36 mois                   10.0%      32 767     204 619   13.6%

Offre la moins chère en coût réel (TEG) : Banque 36 mois
Remarque : le taux « flat » de 9 % coûte en réalité bien plus qu'un taux dégressif de 11 %.
Informations indicatives : faites-vous conseiller avant de signer un contrat de crédit.
```
