# Jour 081 — Distance de Levenshtein : correcteur orthographique simple

**Langue :** Java

## Le problème
Un client tape « telefone », « ordinatuer » ou « calvi » dans la recherche d'une boutique en ligne et ne trouve rien. Il faut lui proposer le mot le plus proche (« Vouliez-vous dire… ? »).

## La solution
La distance de Levenshtein compte le nombre minimal d'insertions, suppressions et substitutions pour passer d'un mot à l'autre. On la calcule par programmation dynamique avec seulement deux lignes de tableau (mémoire O(n)). On compare ensuite le mot saisi à un dictionnaire et on propose les mots à distance ≤ 2, du plus proche au plus éloigné.

## Ce qu'on apprend
La distance d'édition, l'optimisation mémoire d'un tableau de programmation dynamique et le classement de suggestions.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
telefone     → vouliez-vous dire « téléphone » ? ?
ordinatuer   → vouliez-vous dire « ordinateur » ? ?
calvi        → vouliez-vous dire « calavi » ? ?
chargeurs    → vouliez-vous dire « chargeur » ? ?
ecran        → correct (écran)
porto novo   → vouliez-vous dire « porto-novo » ? ?
xylophone    → aucune suggestion
```
