# Jour 131 — Encodage JSON léger d'une mesure de capteur

**Langue :** Python

## Le problème
Un objet connecté envoie ses mesures à un serveur ou à une application. JSON est le format universel, mais sur une liaison LoRa ou GSM facturée à l'octet, chaque caractère compte : '{"temperature": 28.4567}' gaspille beaucoup par rapport à '{"t":28.5}'.

## La solution
On construit la même mesure de trois façons : JSON lisible (indenté), JSON compact (séparateurs sans espaces, clés courtes, valeurs arrondies à la précision réelle du capteur) et un format binaire avec le module struct. On vérifie que chaque format se relit sans perte de l'info utile, et on compare les tailles et le coût d'envoi d'un an de mesures.

## Ce qu'on apprend
Le module json (dumps, separators, sort_keys), la précision significative, struct.pack et le compromis lisibilité / taille.

## Lancer
```bash
python3 json_leger.py
```

## Exemple de sortie
```
Même mesure, trois formats :

{
  "capteur": "ferme-calavi-03",
  "temperature": 28.456789,
  "humidite": 71.234,
  "batterie": 3.71234,
  "horodatage": 1793174400
}
{"id":"ferme-calavi-03","t":28.5,"h":71,"b":3.71,"ts":1793174400}
03 00 1e 0b d3 1b 80 0e 80 ab e1 6a

Format             Octets   Gain    1 an, 1 mesure/10 min
JSON lisible          135     0%                 6929 Ko
JSON compact           65    52%                 3336 Ko
```
