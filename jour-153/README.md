# Jour 153 — Calcul de tarif zem selon la distance et l'heure

**Langue :** Java

## Le problème
Avec un zémidjan, le prix se négocie à chaque course, et le client comme le conducteur ne savent pas toujours si le prix est juste. Une application de réservation doit afficher un tarif clair et justifiable.

## La solution
Un tarif = prise en charge + prix au kilomètre (dégressif au-delà de 5 km) + majorations (nuit, pluie, forte demande), avec un minimum et un arrondi aux 50 FCFA, car on paie en espèces. On calcule aussi la part du conducteur après commission de la plateforme et son gain par litre d'essence. Tous les montants sont des exemples à ajuster au terrain.

## Ce qu'on apprend
La conception d'une grille tarifaire, les enums pour les conditions, l'arrondi monétaire et la décomposition d'un prix.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Zogbadjè → Campus UAC (07:30)      →    350 FCFA
   prise en charge 150 + 1,2 km → 270 | forte demande +15%
   conducteur : 315 FCFA après commission, 20 FCFA d'essence → gain 295 FCFA
Godomey → Calavi Kpota (08:10)     →    800 FCFA
   prise en charge 150 + 6,8 km → 785
   conducteur : 720 FCFA après commission, 116 FCFA d'essence → gain 604 FCFA
Calavi → Cotonou Ganhi (18:20)     →  2 150 FCFA
   prise en charge 150 + 17,5 km → 1588 | pluie +20% | forte demande +15%
   conducteur : 1 935 FCFA après commission, 298 FCFA d'essence → gain 1 638 FCFA
Akpakpa → Cadjèhoun (23:15)        →  1 100 FCFA
   prise en charge 150 + 7,4 km → 830 | nuit (22 h-5 h) +30%
   conducteur : 990 FCFA après commission, 126 FCFA d'essence → gain 864 FCFA
Calavi → Cotonou (nuit + pluie) (23:40)   →  2 400 FCFA
   prise en charge 150 + 17,5 km → 1588 | nuit (22 h-5 h) +30% | pluie +20%
   conducteur : 2 160 FCFA après commission, 298 FCFA d'essence → gain 1 863 FCFA
```
