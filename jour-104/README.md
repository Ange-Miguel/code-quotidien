# Jour 104 — Filtre moyenne glissante sur des mesures bruitées

**Langue :** Python

## Le problème
Un capteur analogique (température, niveau d'eau, courant) donne des valeurs qui sautillent à cause du bruit électrique. Afficher ou décider sur une valeur brute fait clignoter un affichage ou déclencher un relais à tort.

## La solution
Deux filtres classiques de l'embarqué. (1) La moyenne glissante sur N échantillons, calculée en O(1) par mise à jour d'une somme (on ajoute la nouvelle valeur, on retire la plus ancienne). (2) La moyenne exponentielle (EMA) : y = y + α(x − y), qui ne demande qu'une seule variable de mémoire. On mesure la réduction du bruit (écart-type) et le retard introduit par chaque filtre sur un signal simulé.

## Ce qu'on apprend
Les filtres numériques simples, deque à taille fixe, le compromis lissage / réactivité et la mesure de l'efficacité d'un filtre.

## Lancer
```bash
python3 moyenne_glissante.py
```

## Exemple de sortie
```
Signal : 25 °C puis saut à 30 °C à t = 60, bruit ±0,8 °C ; fenêtre N = 8

Filtre                    bruit résiduel  retard au saut
Brut                              0.75 °C            0 éch.
Moyenne glissante                 0.26 °C            5 éch.
Moyenne exponentielle             0.26 °C            4 éch.

  t   brut  glissante  EMA
 54  25.01    24.99   25.00
 56  25.72    24.92   25.02
 58  24.44    24.87   24.92
 60  28.68    25.21   25.52
 62  29.45    26.31   27.05
 64  30.23    27.46   28.07
 66  30.58    28.65   28.70
```
