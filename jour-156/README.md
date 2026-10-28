# Jour 156 — Stock de pharmacie avec dates de péremption

**Langue :** Java

## Le problème
Une pharmacie ou un centre de santé perd de l'argent avec les médicaments périmés et peut mettre des patients en danger s'ils sont délivrés par erreur. Un même médicament existe souvent en plusieurs lots avec des dates différentes.

## La solution
Le stock est organisé par lots (numéro, quantité, date de péremption). La délivrance suit la règle FEFO (First Expired, First Out) : on sert toujours le lot qui expire le plus tôt, grâce à une file de priorité par produit. Les lots périmés sont bloqués, ceux qui expirent dans moins de 90 jours sont signalés, et on calcule la valeur à risque. Produits et lots fictifs.

## Ce qu'on apprend
La règle FEFO, PriorityQueue par clé dans une Map, LocalDate et ChronoUnit, et le blocage automatique d'articles non conformes.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Délivrance de 20 × Paracétamol 500 mg (boîte) :
   → servi : 15 du lot P2207 (2026-12-15) 5 du lot P2311 (2027-06-30)
Délivrance de 3 × Amoxicilline 500 mg (boîte) :
   ⛔ lot A0901 de Amoxicilline 500 mg (boîte) périmé le 2026-10-20 : bloqué, retiré du stock (10 unités)
   → servi : 3 du lot A1102 (2027-03-31)
Délivrance de 200 × Sels de réhydratation (sachet) :
   → stock insuffisant (120 disponibles)
Délivrance de 1 × Vitamine C :
   → produit inconnu

État du stock au 2026-10-28 :
   Amoxicilline 500 mg (boîte) lot A1102     22 u.  2027-03-31  ok
   Paracétamol 500 mg (boîte) lot P2311     35 u.  2027-06-30  ok
   Sels de réhydratation (sachet) lot S0455    120 u.  2027-01-10  expire dans 74 j ⚠
Valeur à écouler en priorité (≤ 90 jours) : 18 000 FCFA
```
