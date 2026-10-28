# Jour 174 — Générateur de QR code texte (matrice simplifiée)

**Langue :** Java

## Le problème
Comment un QR code range-t-il du texte dans une grille de carrés noirs et blancs, et comment le lecteur retrouve-t-il l'orientation et détecte-t-il les erreurs ? Le vrai format QR (avec correction Reed-Solomon) est long ; on construit ici une version pédagogique qui reprend ses idées. ATTENTION : ce n'est pas un vrai QR code, un téléphone ne pourra pas le lire.

## La solution
La grille 21×21 contient trois motifs de repérage dans les coins (comme le QR), des lignes de synchronisation, puis les données : longueur, octets du texte et un octet de parité XOR, écrits bit par bit en zigzag. Un masque (damier) évite les grandes zones uniformes. Le décodeur relit la grille, retire le masque, vérifie la parité et signale une grille abîmée.

## Ce qu'on apprend
Les tableaux 2D de booléens, l'écriture et la lecture de bits, les masques XOR, le parcours en zigzag et le principe des codes 2D.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Matrice 21×21 pour « Tél +229 01 66 54 61 44 » (format pédagogique, pas un vrai QR code) :
  ██████████████  ██  ██████  ██████████████
  ██          ██  ██  ██      ██          ██
  ██  ██████  ██    ██████    ██  ██████  ██
  ██  ██████  ██  ████  ██    ██  ██████  ██
  ██  ██████  ██  ██  ██  ██  ██  ██████  ██
  ██          ██    ██████    ██          ██
  ██████████████  ██  ██  ██  ██████████████
                    ██  ████                
  ██          ██████  ██  ██████            
            ██  ██  ██  ██████    ██████  ██
      ██  ████████████  ████      ██████  ██
    ██████████    ██      ████  ████    ██  
  ██████████  ██  ██      ██  ██  ██    ██  
                    ██  ██████    ██  ████  
```
