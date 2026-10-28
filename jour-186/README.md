# Jour 186 — Limiteur de débit (rate limiter) pour une API

**Langue :** Java

## Le problème
Une API publique (envoi de SMS, consultation de stock, connexion) peut être saturée par un client trop gourmand ou attaquée par un robot qui essaie des milliers de mots de passe. Il faut limiter le nombre de requêtes par client, sans gêner les utilisateurs normaux.

## La solution
L'algorithme du seau à jetons (token bucket). Chaque client a un seau de capacité C qui se remplit de R jetons par seconde ; chaque requête consomme un jeton ; seau vide = requête refusée (code HTTP 429). Cela autorise de courtes rafales tout en limitant le débit moyen. On compare avec la fenêtre fixe, qui laisse passer le double de requêtes à la frontière entre deux fenêtres. L'horloge est simulée pour la démonstration.

## Ce qu'on apprend
Le seau à jetons, la fenêtre fixe et son défaut, le calcul paresseux du remplissage et la gestion par client avec une Map.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Règle : 10 requêtes par minute en moyenne, rafale de 5 autorisée

Scénario                          Seau à jetons   Fenêtre fixe (1 min)
Utilisateur normal               20 ok /   0 429         20 ok /   0 429
Robot (attaque)                  14 ok / 286 429         10 ok / 290 429
Rafale à la frontière             5 ok /  15 429         20 ok /   0 429

La fenêtre fixe laisse passer 20 requêtes en 4 secondes autour de t = 60 s (10 + 10) ;
le seau à jetons n'en accepte que la rafale prévue, puis le débit moyen.

Clients indépendants : true / true
```
