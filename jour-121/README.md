# Jour 121 — Surveillance de température d'une chambre froide avec alarmes

**Langue :** C

## Le problème
Une pharmacie (vaccins entre 2 et 8 °C), une poissonnerie ou un vendeur de glaces perd tout son stock si la chambre froide dérive pendant une coupure ou une panne de compresseur. Une simple alarme instantanée sonne trop souvent (porte ouverte 30 s) ; il faut une logique intelligente.

## La solution
Une alarme à plusieurs niveaux : AVERTISSEMENT si la température sort de la plage pendant plus de 5 minutes consécutives, CRITIQUE au-delà de 20 minutes ou si l'écart dépasse 4 °C. On calcule aussi le temps total hors plage et le « degré-minute » (intégrale de l'écart), utilisé pour juger si les produits sont encore bons. Une journée est simulée avec une ouverture de porte et une coupure de courant.

## Ce qu'on apprend
Les alarmes temporisées, les niveaux de gravité, le calcul d'intégrale discrète et la séparation entre mesure et décision.

## Lancer
```bash
gcc -Wall -Wextra -o chambre_froide chambre_froide.c -lm
./chambre_froide
```

## Exemple de sortie
```
Chambre froide pour vaccins (plage 2-8 °C), journée simulée minute par minute

10h08   10.4 °C  normal → AVERTISSEMENT  (notification)
10h12    7.5 °C  AVERTISSEMENT → normal
14h22    8.9 °C  normal → AVERTISSEMENT  (notification)
14h37   11.3 °C  AVERTISSEMENT → CRITIQUE  (SMS au pharmacien + sirène)
15h07    7.7 °C  CRITIQUE → normal

Bilan : 57 min hors plage, maximum 15.6 °C, exposition 188 degré-minutes
Verdict : produits a priori conformes
```
