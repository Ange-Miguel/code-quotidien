# Jour 187 — Gestionnaire de sessions avec expiration

**Langue :** C

## Le problème
Quand on se connecte à un site ou à une application, le serveur crée une « session » identifiée par un jeton aléatoire stocké dans un cookie. Le jeton doit être imprévisible, la session doit expirer après une période d'inactivité (et au bout d'une durée maximale), et on doit pouvoir se déconnecter. Sinon, un téléphone perdu ou un jeton volé donne un accès permanent.

## La solution
Les jetons sont 16 octets lus dans /dev/urandom, affichés en hexadécimal. Chaque session mémorise l'utilisateur, la date de création et celle de la dernière activité. La validation vérifie l'expiration glissante (30 min d'inactivité) et absolue (8 h). Un nettoyage périodique libère les sessions expirées. L'horloge est simulée pour la démonstration.

## Ce qu'on apprend
La génération de jetons sûrs, l'expiration glissante et absolue, la comparaison en temps constant et la gestion d'une table fixe.

## Lancer
```bash
gcc -Wall -Wextra -o sessions sessions.c -lm
./sessions
```

## Exemple de sortie
```
Jetons (16 octets aléatoires) : awa=4a9534c7b8b86cc7… kofi=18c1b005a640191b…
  08:00  requête de Awa    → autorisée
  08:20  requête de Awa    → autorisée
  08:45  requête de Awa    → autorisée
  08:45  requête de Kofi   → REFUSÉE : session invalide ou expirée, reconnectez-vous
  08:45  requête de Pirate → REFUSÉE : session invalide ou expirée, reconnectez-vous
  08:46  requête de Awa    → REFUSÉE : session invalide ou expirée, reconnectez-vous
  12:31  requête de Awa    → autorisée
```
