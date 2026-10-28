# Jour 163 — Calcul des frais de livraison par zone à Cotonou

**Langue :** C

## Le problème
Une boutique en ligne livre à Cotonou et dans les environs. Les frais doivent dépendre de la zone (centre, périphérie, villes voisines), du poids du colis, de l'urgence (livraison express) et être gratuits au-delà d'un certain montant d'achat, pour encourager les grosses commandes.

## La solution
Une grille tarifaire en tableau : chaque zone a un prix de base et un supplément par kilogramme au-delà de 2 kg ; les quartiers sont associés à leur zone par une table de correspondance (recherche insensible à la casse). On ajoute l'express (+50 %, impossible hors zones 1-2), la gratuité conditionnelle et un délai estimé. Grille et quartiers donnés à titre d'exemple.

## Ce qu'on apprend
Les tables de correspondance, la séparation données / calcul, la gestion des cas particuliers et l'arrondi à 100 FCFA.

## Lancer
```bash
gcc -Wall -Wextra -o livraison_zones livraison_zones.c -lm
./livraison_zones
```

## Exemple de sortie
```
Ganhi        Zone 1 : Cotonou centre      1.2 kg standard →  1000 FCFA, délai ~4 h
Cadjèhoun   Zone 1 : Cotonou centre      1.2 kg express  →  1500 FCFA, délai ~2 h
Akpakpa      Zone 2 : Cotonou périphérie  4.5 kg standard →  2300 FCFA, délai ~6 h
Calavi Kpota Zone 3 : Calavi / Godomey    7.0 kg standard →  3500 FCFA, délai ~24 h
Zogbadjè    Zone 3 : Calavi / Godomey    2.0 kg standard →     0 FCFA (offerte dès 50 000 FCFA), délai ~24 h
Godomey      : express indisponible en Zone 3 : Calavi / Godomey
Porto-Novo   Zone 4 : Porto-Novo / Ouidah  3.2 kg standard →  3800 FCFA, délai ~48 h
Parakou      : quartier non desservi, contactez-nous
```
