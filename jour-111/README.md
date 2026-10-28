# Jour 111 — Contrôleur PID simulé pour régler une température

**Langue :** Java

## Le problème
Maintenir un four, un incubateur à œufs ou une chambre de fermentation à une température précise. Un simple tout-ou-rien (jour 107) oscille ; il faut doser la puissance de chauffe en continu.

## La solution
Le régulateur PID calcule la commande à partir de l'erreur e = consigne − mesure : P (proportionnel à l'erreur), I (somme des erreurs passées, élimine l'écart résiduel), D (vitesse de variation, amortit). On simule un incubateur (résistance chauffante puis air, avec retard et pertes) et on compare P seul, PI et PID : dépassement, temps de stabilisation, erreur finale. L'intégrale est bornée (anti-windup) car la puissance est limitée.

## Ce qu'on apprend
Le principe du PID, la discrétisation (pas de temps dt), la saturation et l'anti-windup, et les indicateurs de performance.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Incubateur : consigne 37,7 °C, départ 28 °C, 1 heure simulée

Réglage              Dépassement  Stable (±0,2°C)  Erreur finale
P seul (Kp=20)           1,74 °C           jamais       0,313 °C
PI                       1,91 °C            386 s       0,000 °C
PID                      0,38 °C            279 s       0,000 °C
PI trop nerveux          3,25 °C            389 s       0,000 °C

Le P seul garde un écart permanent ; le I l'annule ; le D réduit le dépassement.
```
