# Jour 120 — Irrigation intelligente selon l'humidité du sol simulée

**Langue :** Java

## Le problème
Arroser un potager ou une petite ferme à heure fixe gaspille l'eau quand il a plu et laisse les plantes souffrir en saison sèche. Un capteur d'humidité du sol (capacitif) permet d'arroser seulement quand il le faut, idéalement le matin tôt ou le soir pour limiter l'évaporation.

## La solution
On simule 14 jours : l'humidité baisse avec l'évaporation (plus forte en milieu de journée et par forte chaleur), monte avec la pluie et l'arrosage. La règle d'arrosage : humidité < seuil de la culture, créneau autorisé (5 h–8 h ou 17 h–19 h), et pas d'arrosage si la pluie prévue est importante. On compare la consommation d'eau avec un arrosage fixe quotidien, et le nombre d'heures où les plantes sont en stress hydrique.

## Ce qu'on apprend
La modélisation d'un système agricole simple, les règles de décision combinées, les records et la comparaison de stratégies.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
14 jours simulés (pluies aléatoires), parcelle de 10 m²

Culture   Stratégie        Eau (L)   Stress (h)  Arrosages
Tomates   fixe à midi          700            0         14
Tomates   capteur              279            1          4
Piment    fixe à midi          700            0         14
Piment    capteur              266            1          4
Laitue    fixe à midi          700           17         14
Laitue    capteur              306            2          5

Le capteur arrose au bon moment, évite les jours de pluie et réduit le stress des plantes.
```
