# Jour 198 — Simulation d'une file de supermarché (caisses optimales)

**Langue :** Java

## Le problème
Un supermarché veut savoir combien de caisses ouvrir selon l'heure : trop peu, les clients attendent et partent ; trop, on paie des caissiers inoccupés. Faut-il aussi une file unique (comme à la banque) ou une file par caisse ?

## La solution
Simulation à événements discrets. Les arrivées suivent un processus de Poisson dont le débit varie au cours de la journée (pics à midi et en fin de journée) ; le temps de passage en caisse dépend du nombre d'articles. On compare, pour 2 à 6 caisses : temps d'attente moyen et 95e centile, taux d'occupation des caissiers et coût total (salaires + coût estimé de l'attente des clients), puis file unique contre files séparées.

## Ce qu'on apprend
La simulation à événements discrets avec une file de priorité d'événements, les lois exponentielles, les centiles et l'optimisation d'un coût.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Journée de 12 h, ~900 clients, pics à 12 h 30 et 18 h (file unique)

Caisses  Attente moy.  95 % < (min)  Occupation       Coût total
      2       72,8 min         195,7         92%     3 035 421 FCFA
      3       15,5 min          59,5         74%       692 377 FCFA
      4        4,6 min          23,5         58%       262 037 FCFA
      5        1,3 min           7,3         46%       143 876 FCFA
      6        0,2 min           1,6         39%       115 400 FCFA
      7        0,0 min           0,3         33%       127 833 FCFA
      8        0,0 min           0,0         29%       144 582 FCFA

Nombre de caisses le plus économique (salaires + attente) : 6

Avec 6 caisses : file unique 0,2 min d'attente moyenne (95 % < 1,6 min)
                 files séparées 2,4 min d'attente moyenne (95 % < 12,0 min)
```
