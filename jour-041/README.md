# Jour 041 — Pierre-feuille-ciseaux contre l'ordinateur

**Langue :** Python

## Le problème
Un jeu simple, mais un bon terrain pour une première « IA » : un humain ne joue jamais vraiment au hasard, il a des habitudes qu'on peut exploiter.

## La solution
L'ordinateur mémorise ce que le joueur a joué après chacun de ses coups précédents (chaîne de Markov d'ordre 1). Il prédit le coup le plus probable et joue ce qui le bat. En démo, il affronte un joueur simulé qui a une habitude (il répète souvent son coup) puis un joueur vraiment aléatoire.

## Ce qu'on apprend
Les dictionnaires imbriqués, Counter, une prédiction statistique simple et l'importance du vrai hasard face à un adversaire qui apprend.

## Lancer
```bash
python3 pierre_feuille_ciseaux.py
```

## Exemple de sortie
```
200 manches contre trois types de joueurs :
Joueur qui répète son coup   → ordinateur 154 | joueur 23 | égalités 23
Joueur qui tourne en cycle   → ordinateur 197 | joueur 0 | égalités 3
Joueur vraiment aléatoire    → ordinateur 70 | joueur 69 | égalités 61
```
