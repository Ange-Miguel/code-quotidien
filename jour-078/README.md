# Jour 078 — Sac à dos : choisir les produits les plus rentables à importer

**Langue :** Java

## Le problème
Un importateur dispose d'un budget limité (ou d'un colis limité en kilos) et d'une liste de produits, chacun avec un coût et un bénéfice attendu. Quels produits commander pour maximiser le bénéfice total sans dépasser le budget ?

## La solution
C'est le problème du sac à dos 0/1. La programmation dynamique remplit un tableau meilleur[i][b] = meilleur bénéfice avec les i premiers produits et un budget b. On remonte ensuite le tableau pour savoir quels produits ont été choisis. On compare avec l'approche gloutonne (meilleur ratio d'abord), qui peut se tromper.

## Ce qu'on apprend
La programmation dynamique, la reconstruction d'une solution, la discrétisation (budget en milliers de FCFA) et les limites du glouton.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Budget : 750 000 FCFA

Solution optimale (programmation dynamique) : bénéfice 390 000 FCFA pour 750 000 FCFA investis
   - Écouteurs Bluetooth x100   coût  330 k  bénéfice  180 k
   - Lot de montres connectées  coût  420 k  bénéfice  210 k

Approche gloutonne (meilleur ratio d'abord) : bénéfice 350 000 FCFA pour 680 000 FCFA investis
   - Coques de téléphone x200   coût   90 k  bénéfice   50 k
   - Écouteurs Bluetooth x100   coût  330 k  bénéfice  180 k
   - Lampes solaires x60        coût  260 k  bénéfice  120 k
```
