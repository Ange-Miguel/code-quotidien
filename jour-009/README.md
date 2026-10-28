# Jour 009 — Partage d'une addition entre amis avec pourboire

**Langue :** Java

## Le problème
Au maquis ou au restaurant, il faut partager l'addition, ajouter un pourboire et arrondir pour que chacun paie un montant pratique.

## La solution
On calcule le total avec pourboire, la part de chacun arrondie au multiple de 50 FCFA supérieur, puis le surplus éventuel collecté (qui peut servir de pourboire en plus).

## Ce qu'on apprend
Les calculs avec des entiers pour éviter les erreurs d'arrondi de la monnaie, l'arrondi au multiple supérieur et le formatage avec printf.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Addition : 23 500 FCFA | Pourboire 10 % : 2 350 FCFA
Total : 25 850 FCFA pour 4 personne(s)
Part exacte : 6 462 FCFA → chacun paie 6 500 FCFA
Collecté : 26 000 FCFA (surplus 150 FCFA)

Addition : 18 000 FCFA | Pourboire 0 % : 0 FCFA
Total : 18 000 FCFA pour 3 personne(s)
Part exacte : 6 000 FCFA → chacun paie 6 000 FCFA
Collecté : 18 000 FCFA (surplus 0 FCFA)

Addition : 47 250 FCFA | Pourboire 5 % : 2 362 FCFA
Total : 49 612 FCFA pour 6 personne(s)
Part exacte : 8 268 FCFA → chacun paie 8 300 FCFA
Collecté : 49 800 FCFA (surplus 188 FCFA)
```
