# Jour 160 — Marketplace d'occasion : recherche et filtres d'annonces

**Langue :** C

## Le problème
Sur une plateforme d'annonces d'occasion (téléphones, motos, meubles), l'acheteur veut combiner plusieurs filtres : catégorie, ville, fourchette de prix, état, mots-clés dans le titre, puis trier par prix ou par date. Il faut que ce soit rapide et que les filtres absents soient simplement ignorés.

## La solution
Chaque annonce est une structure ; un critère de recherche est une structure dont les champs vides signifient « peu importe ». Une seule fonction teste si une annonce correspond (recherche de mots-clés insensible à la casse avec strcasestr), puis on trie les résultats avec qsort et une fonction de comparaison choisie à l'exécution. Annonces fictives.

## Ce qu'on apprend
La recherche multicritère, les filtres optionnels, les pointeurs de fonction pour choisir un tri et la recherche de sous-chaînes.

## Lancer
```bash
gcc -Wall -Wextra -o annonces annonces.c -lm
./annonces
```

## Exemple de sortie
```
Téléphones à Calavi, moins de 80 000 FCFA, triés par prix → 3 résultat(s)
   #2  Samsung A14 écran fissuré    Calavi        38000 FCFA  à réparer il y a 1 j
   #8  Itel A70                       Calavi        42000 FCFA  bon       il y a 8 j
   #4  Tecno Spark 10 sous garantie   Calavi        72000 FCFA  neuf      il y a 3 j

« samsung » en bon état ou mieux, toutes villes, plus récents d'abord → 2 résultat(s)
   #6  Samsung A34 128 Go             Porto-Novo   115000 FCFA  très bon il y a 5 j
   #10 Samsung A14                    Cotonou       60000 FCFA  très bon il y a 9 j

Motos entre 300 000 et 600 000 FCFA → 2 résultat(s)
   #3  Moto Haojue 125                Calavi       420000 FCFA  bon       il y a 6 j
   #7  Bajaj Boxer 2022               Cotonou      510000 FCFA  très bon il y a 1 j

Meubles à Porto-Novo → 0 résultat(s)
```
