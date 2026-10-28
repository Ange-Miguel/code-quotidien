# Jour 083 — Codage de Huffman d'un message

**Langue :** Python

## Le problème
En ASCII, chaque caractère occupe 8 bits, même le « e » qui revient sans cesse. Peut-on utiliser des codes plus courts pour les caractères fréquents, et plus longs pour les rares, sans ambiguïté au décodage ?

## La solution
L'algorithme de Huffman construit un arbre binaire : on fusionne toujours les deux nœuds les moins fréquents (file de priorité heapq) jusqu'à n'en garder qu'un. Le chemin vers chaque feuille (0 = gauche, 1 = droite) donne son code. Aucun code n'est le préfixe d'un autre, donc le décodage est sans ambiguïté. On encode, décode et mesure le gain.

## Ce qu'on apprend
Les tas (heapq), la construction d'arbres, les codes préfixes et la compression sans perte utilisée dans ZIP, JPEG ou MP3.

## Lancer
```bash
python3 huffman.py
```

## Exemple de sortie
```
Texte : 'le zem est passé devant le marché de calavi ce matin, et le '…
   ' '   × 15 → 00
   'e'   × 12 → 110
   'l'   ×  4 → 0100
   'm'   ×  4 → 0101
   's'   ×  4 → 0110
   't'   ×  6 → 1011
   'a'   ×  7 → 1111
   'z'   ×  2 → 01110
   'p'   ×  2 → 01111
   'é'   ×  2 → 10000
   … et 8 autres caractères
   616 bits en UTF-8 → 283 bits avec Huffman (46% de la taille, 3.77 bits/caractère)

Texte : 'aaaaaaaaaaaaaaaaaaaabbbbbbbbccccd'
```
