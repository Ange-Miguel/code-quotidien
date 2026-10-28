# Jour 149 — Tableau de bord console des capteurs en temps réel

**Langue :** Python

## Le problème
Pendant la mise au point d'une installation (serre, chambre froide, atelier), on veut voir d'un coup d'œil l'état de tous les capteurs, leurs tendances et les alertes, directement dans le terminal du Raspberry Pi, sans interface graphique.

## La solution
Un tableau de bord texte rafraîchi périodiquement : pour chaque capteur, la dernière valeur, un mini-graphique (sparkline) de l'historique récent avec les caractères ▁▂▃▄▅▆▇█, une flèche de tendance, l'état (normal / alerte / hors ligne si aucune donnée récente) et l'heure de la dernière mesure. Les codes ANSI permettent la couleur et le retour en haut de l'écran. En démo, 3 images du tableau sont produites sans attendre ; avec --live, il se rafraîchit chaque seconde.

## Ce qu'on apprend
Les séquences d'échappement ANSI, les sparklines, deque à taille fixe, la détection de capteur muet et la mise en page console.

## Lancer
```bash
python3 tableau_de_bord.py
```

## Exemple de sortie
```
=== Tableau de bord — serre de Calavi — t = 12 s ===
Capteur                Valeur  Historique               T  État
Température air       30.4 °C  ▂▁▃▄▂▆▅▇▆▆▆█▆            ↗  normal
Humidité air           79.8 %  ▁▁▃▂▇▄▅▆▆▇▇█▇            ↗  normal
Humidité sol           56.9 %  ▁▁▃▄▄▄▄▆▆█▅▄▄            ↘  normal
Lumière              60.5 klx  ▁▁▃▄▅▄▆▆▅█▅▇▃            ↘  normal
Niveau cuve            40.7 %  ▂▂▁▄▄▄▇█▇▅▆▅▆            ↘  normal
Alertes : aucune

=== Tableau de bord — serre de Calavi — t = 25 s ===
Capteur                Valeur  Historique               T  État
Température air       23.3 °C  ▅▆▅▇▆▇▇▇▇█▇▆▆▆▆▄▅▄▄▂▂▁▁▁ ↘  normal
Humidité air           62.8 %  ▅▄▇▆▆▆▇▇▇█▇▆▅▆▅▅▄▄▄▂▄▂▁▁ ↘  normal
Humidité sol           47.5 %  ▄▅▅▅▆▇▆█▆▆▆▅▆▆▅▄▃▂▁▃▁▁▁▁ ↘  normal
Lumière              18.5 klx  ▅▅▆▆▇▇▆█▆▇▅▆▆▆▅▄▃▃▃▂▂▁▁▁ ↘  normal
```
