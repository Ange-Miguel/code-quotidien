# Jour 172 — Vote électronique simple et dépouillement

**Langue :** C

## Le problème
Une association ou une promotion vote pour élire son bureau. Il faut garantir qu'une personne ne vote qu'une fois, que les bulletins nuls sont comptés à part, et produire un dépouillement clair. Avec plusieurs candidats, le vote à un tour peut élire quelqu'un avec 35 % des voix.

## La solution
Chaque électeur inscrit a un code unique ; le bureau de vote vérifie le code et le marque comme utilisé (sans lier le code au bulletin, pour garder le secret du vote). Les bulletins sont des classements de préférence. On dépouille au scrutin majoritaire simple, puis au vote à second tour instantané (élimination successive du dernier et report des voix selon les préférences), et on compare. Données fictives ; un vrai scrutin exige bien plus de garanties (audit, chiffrement, anonymat).

## Ce qu'on apprend
La vérification d'unicité, la séparation identité / bulletin, deux méthodes de dépouillement et l'algorithme du vote alternatif.

## Lancer
```bash
gcc -Wall -Wextra -o vote vote.c -lm
./vote
```

## Exemple de sortie
```
Bureau de vote :
  code K7Q2 : a déjà voté, vote refusé
  code ZZZZ : inconnu, vote refusé
  14 inscrits, 14 votants (participation 100 %), 13 exprimés, 1 nul(s)

Scrutin majoritaire à un tour (premier choix seulement) :
   Awa     5 voix (38.5 %)
   Kofi    3 voix (23.1 %)
   Inès   3 voix (23.1 %)
   Serge   2 voix (15.4 %)
   → élu(e) : Awa avec 38 % des voix exprimées

Vote alternatif (report des voix des éliminés) :
   tour 1 : Awa 5 Kofi 3 Inès 3 Serge 2  → Serge éliminé(e)
   tour 2 : Awa 5 Kofi 4 Inès 4  → Kofi éliminé(e)
```
