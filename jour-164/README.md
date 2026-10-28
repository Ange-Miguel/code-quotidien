# Jour 164 — Suivi de commande import Chine : étapes et délais

**Langue :** Python

## Le problème
Un importateur passe commande chez un fournisseur chinois ; la marchandise passe par la production, l'entrepôt du transitaire, le transport (aérien ou maritime), le dédouanement à Cotonou puis la livraison. Les clients demandent sans cesse « où en est ma commande ? » et les retards ne sont découverts que trop tard.

## La solution
Chaque mode de transport a ses étapes avec une durée typique. Pour une commande, on enregistre les dates réelles des étapes franchies ; le programme calcule l'étape en cours, le retard accumulé par rapport au plan, une date d'arrivée réestimée (durées restantes ajoutées à la date réelle de la dernière étape) et une alerte si le retard dépasse un seuil. Les durées sont indicatives.

## Ce qu'on apprend
La modélisation d'un processus par étapes, datetime et timedelta, la réestimation dynamique d'une échéance et les alertes.

## Lancer
```bash
python3 suivi_import.py
```

## Exemple de sortie
```
CMD-0091 écouteurs ×100 — aérien, commandé le 10/10/2026
   ✔ Production fournisseur             16/10  (-1 j)
   ✔ Envoi à l'entrepôt du transitaire  19/10  (-1 j)
   ✔ Vol Guangzhou → Cotonou            25/10  (-1 j)
   ▶ Dédouanement                       en cours (prévu le 30/10)
     Livraison au client                prévu le 31/10
   → arrivée estimée le 30/10/2026, retard 0 jour(s)

CMD-0087 ventilateurs ×60 — maritime, commandé le 20/08/2026
   ✔ Production fournisseur             03/09  (+4 j)
   ✔ Envoi à l'entrepôt du transitaire  08/09  (+5 j)
   ✔ Groupage conteneur                 20/09  (+10 j)
   ▶ Traversée maritime                 en cours (prévu le 20/10)
     Port de Cotonou et dédouanement    prévu le 28/10
     Livraison au client                prévu le 30/10
```
