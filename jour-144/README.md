# Jour 144 — Prédiction simple de consommation (régression linéaire)

**Langue :** Java

## Le problème
Un gestionnaire d'immeuble ou une boutique veut prévoir sa consommation d'électricité de la semaine prochaine, qui dépend fortement de la chaleur (climatiseurs, ventilateurs). Peut-on la prévoir à partir de la température prévue ?

## La solution
La régression linéaire des moindres carrés trouve la droite y = a·x + b qui passe au plus près des points (température, kWh) observés : a = cov(x, y) / var(x), b = moyenne(y) − a·moyenne(x). Le coefficient R² indique la qualité de l'ajustement. On entraîne le modèle sur 30 jours, on le teste sur 7 jours qu'il n'a jamais vus, et on prédit avec les prévisions météo.

## Ce qu'on apprend
La régression linéaire, R², la séparation entraînement / test, l'erreur moyenne absolue et les limites d'un modèle simple.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Modèle appris sur 30 jours : kWh = 6,96 × température -170,4
Chaque degré en plus coûte environ 7,0 kWh par jour. R² = 0,92

Test sur 7 jours jamais vus :
   27,9 °C : réel  26,8 kWh, prédit  23,8 kWh (écart -3,0)
   28,9 °C : réel  30,8 kWh, prédit  30,8 kWh (écart -0,1)
   30,4 °C : réel  37,5 kWh, prédit  40,9 kWh (écart +3,4)
   28,8 °C : réel  31,8 kWh, prédit  30,1 kWh (écart -1,8)
   30,5 °C : réel  42,3 kWh, prédit  41,7 kWh (écart -0,6)
   28,3 °C : réel  25,0 kWh, prédit  26,1 kWh (écart +1,1)
   29,8 °C : réel  33,7 kWh, prédit  37,0 kWh (écart +3,3)
Erreur moyenne : 1,9 kWh/jour

Prévisions avec la météo annoncée :
   29,5 °C →  34,7 kWh
```
