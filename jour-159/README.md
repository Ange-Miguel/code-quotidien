# Jour 159 — Mise en relation mentor / mentoré par compétences

**Langue :** Java

## Le problème
Une association ou une plateforme de mentorat a des mentors (professionnels expérimentés) et des mentorés (étudiants, jeunes entrepreneurs). Former les bons binômes à la main, en tenant compte des compétences recherchées, des disponibilités et de la capacité de chaque mentor, devient vite impossible.

## La solution
Chaque personne a un ensemble de compétences et de créneaux. Le score d'un binôme combine la similarité de Jaccard entre les compétences recherchées et offertes (|A ∩ B| / |A ∪ B|), le nombre de créneaux communs et un bonus si la ville est la même. On affecte ensuite les mentorés par ordre de « difficulté » (ceux qui ont le moins d'options d'abord), en respectant la capacité de chaque mentor. Données fictives.

## Ce qu'on apprend
La similarité de Jaccard, les opérations d'ensembles (retainAll, addAll), un score composite et l'affectation sous contrainte de capacité.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Serge  → aucun mentor disponible (créneaux ou compétences incompatibles)
Awa    → Mme A. (développeuse)    score 0,55  (compétences communes : [freelance, web])
Kofi   → M. B. (ingénieur IoT)    score 0,65  (compétences communes : [c, électronique])
Inès   → Mme C. (entrepreneure)   score 0,45  (compétences communes : [marketing, vente])
Ange   → M. B. (ingénieur IoT)    score 0,48  (compétences communes : [iot, startup])

Places restantes : [Mme A. (développeuse) 1, Mme C. (entrepreneure) 0, M. B. (ingénieur IoT) 0]
```
