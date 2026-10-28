# Jour 179 — Autocomplétion avec un arbre préfixe (trie)

**Langue :** Python

## Le problème
Quand on tape « cha » dans la recherche d'une boutique, on veut voir instantanément « chargeur rapide », « chargeur solaire », « chapeau »… en commençant par les plus recherchés. Parcourir toute la liste de produits à chaque lettre tapée devient lent quand le catalogue grandit.

## La solution
Un arbre préfixe (trie) : chaque nœud représente une lettre, et le chemin depuis la racine forme un préfixe. Tous les produits qui commencent par « cha » sont sous le même nœud. Chaque nœud mémorise aussi les k meilleures complétions (par popularité), ce qui rend la suggestion instantanée. Les accents sont ignorés à la saisie (« eco » trouve « écouteurs »).

## Ce qu'on apprend
La structure de trie, les dictionnaires imbriqués, le précalcul des meilleurs résultats et la normalisation de la saisie.

## Lancer
```bash
python3 autocompletion.py
```

## Exemple de sortie
```
15 produits indexés (235 insertions de nœuds)

« c » → chargeur rapide, câble usb-c, chaussures de sport, casque audio
« cha » → chargeur rapide, chaussures de sport, chargeur solaire, chaise pliante
« char » → chargeur rapide, chargeur solaire
« eco » → écouteurs bluetooth
« lampe s » → lampe solaire
« mo » → montre connectée, moto électrique
« xyz » → aucune suggestion

Catalogue de 20 000 produits : 1 000 suggestions en 0.7 ms
```
