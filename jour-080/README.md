# Jour 080 — Plus longue sous-séquence commune (comparaison de textes)

**Langue :** Python

## Le problème
Comment un outil comme « git diff » ou un correcteur de copies trouve-t-il ce qui a changé entre deux versions d'un texte ? Et comment mesurer la ressemblance entre deux séquences d'ADN ou deux devoirs ?

## La solution
La plus longue sous-séquence commune (LCS) se calcule par programmation dynamique : T[i][j] = longueur de la LCS des préfixes. En remontant le tableau, on obtient les éléments communs, puis un diff ligne par ligne : « - » ligne supprimée, « + » ligne ajoutée, « ' ' » inchangée. On mesure aussi le taux de similarité entre deux textes.

## Ce qu'on apprend
La programmation dynamique sur deux dimensions, la remontée d'un tableau pour reconstruire une solution et le principe des outils de diff.

## Lancer
```bash
python3 sous_sequence_commune.py
```

## Exemple de sortie
```
LCS(ABCBDAB, BDCABA) = BCBA | similarité 62%
LCS(électronique, électrique) = électrique | similarité 91%
LCS(GATTACA, TACGATA) = GATA | similarité 57%

Diff entre deux versions d'un message :
     Bonjour,
   - Votre commande de 3 écouteurs est prête.
   + Votre commande de 5 écouteurs est prête.
     Livraison à Calavi demain.
   - Paiement à la livraison.
   - Merci.
   + Merci pour votre confiance.
Similarité des deux versions : 44%
```
