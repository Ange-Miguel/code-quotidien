# Jour 124 — Dimensionnement d'un kit solaire (panneaux, batterie, onduleur)

**Langue :** C

## Le problème
Avec les coupures fréquentes ou loin du réseau, beaucoup de foyers et de boutiques s'équipent en solaire. Mais un kit mal dimensionné (panneaux trop petits, batterie qui s'épuise la nuit) déçoit vite.

## La solution
On fait le bilan des appareils (puissance × heures d'usage), on en déduit l'énergie quotidienne, puis : puissance crête des panneaux = énergie / (heures d'ensoleillement équivalent × rendement global) ; capacité batterie = énergie de nuit × jours d'autonomie / (tension × profondeur de décharge) ; puissance de l'onduleur = somme des appareils pouvant fonctionner ensemble × marge de démarrage. Valeurs indicatives : faire valider par un installateur.

## Ce qu'on apprend
Le bilan énergétique (Wh), la différence entre puissance (W) et énergie (Wh), l'arrondi au composant du commerce supérieur et les hypothèses d'un dimensionnement.

## Lancer
```bash
gcc -Wall -Wextra -o kit_solaire kit_solaire.c -lm
./kit_solaire
```

## Exemple de sortie
```
Appareil                        W Qté    Wh/jour
Ampoules LED                    9    6        324
Ventilateurs                   45    2        900
Téléviseur                   80    1        400
Réfrigérateur (moyenne)      60    1       1440
Chargeurs téléphone          10    4        120
Ordinateur portable            65    1        455
Box Internet                   12    1        288
Total : 3927 Wh/jour (dont 2084 Wh la nuit)

Panneaux : 1047 Wc nécessaires → 3 panneau(x) de 400 W (1200 Wc)
Batterie : 109 Ah en 24 V pour 1.0 nuit(s) → 2 batterie(s) de 100 Ah en parallèle
Onduleur : appareils simultanés 401 W × 1,5 → au moins 602 W (ex. 1000 W)
Régulateur MPPT : courant ≈ 62 A sous 24 V

```
