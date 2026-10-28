# Jour 070 — Carte des quartiers de Calavi en graphe et parcours BFS

**Langue :** C

## Le problème
Un livreur ou un conducteur de zem veut savoir combien de quartiers il doit traverser pour aller d'un point à un autre, et quels quartiers sont à 1, 2 ou 3 « sauts » de sa base.

## La solution
Les quartiers sont les sommets d'un graphe non pondéré représenté par une matrice d'adjacence (liaisons simplifiées pour l'exemple). Le parcours en largeur (BFS) avec une file visite les quartiers par distance croissante : il donne le plus court chemin en nombre de quartiers et les « niveaux » autour du départ.

## Ce qu'on apprend
La matrice d'adjacence, le parcours en largeur, la file d'attente avec un simple tableau, et la reconstruction de chemin.

## Lancer
```bash
gcc -Wall -Wextra -o quartiers_bfs quartiers_bfs.c -lm
./quartiers_bfs
```

## Exemple de sortie
```
Kpota → Cococodji : 5 saut(s)
   Kpota → Calavi Centre → Zogbadjè → Arconville → Godomey → Cococodji
Togba → Arconville : 5 saut(s)
   Togba → Akassato → Kpota → Calavi Centre → Zogbadjè → Arconville
Autour de Calavi Centre :
   à 1 saut(s) : Zogbadjè; Tankpè; Kpota;
   à 2 saut(s) : Akassato; Arconville;
   à 3 saut(s) : Togba; Godomey;
   à 4 saut(s) : Cococodji; Womey; Ouèdo;
   à 5 saut(s) : Hêvié;
```
