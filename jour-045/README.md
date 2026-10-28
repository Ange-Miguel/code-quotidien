# Jour 045 — Calcul de la quantité de ciment / sable pour une dalle

**Langue :** Java

## Le problème
Pour couler une dalle (terrasse, sol de boutique, fondation légère), il faut commander le bon nombre de sacs de ciment, de brouettes de sable et de gravier. Une erreur coûte cher ou bloque le chantier.

## La solution
Volume = longueur × largeur × épaisseur. On applique un dosage classique de béton (350 kg de ciment par m³, 400 L de sable, 800 L de gravier, 175 L d'eau), majoré de 5 % pour les pertes, et on convertit en sacs de 50 kg et en brouettes de 60 L. Valeurs indicatives à faire valider par un professionnel.

## Ce qu'on apprend
Les enums avec attributs, l'arrondi au supérieur (Math.ceil) et la conversion d'un besoin en unités d'achat.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Terrasse (5,0 × 4,0 m, 12 cm, dosage DALLE)
  Volume avec pertes : 2,52 m³
  Ciment  : 18 sacs de 50 kg (~93 600 FCFA)
  Sable   : 17 brouettes (1008 L)
  Gravier : 34 brouettes (2016 L)
  Eau     : ~441 L

Sol de boutique (6,0 × 3,5 m, 5 cm, dosage CHAPE)
  Volume avec pertes : 1,10 m³
  Ciment  : 9 sacs de 50 kg (~46 800 FCFA)
  Sable   : 19 brouettes (1103 L)
  Eau     : ~193 L

Valeurs indicatives : faites valider le dosage par un maçon ou un ingénieur.
```
