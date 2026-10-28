# Jour 148 — Mise à jour OTA simulée avec vérification d'intégrité

**Langue :** C

## Le problème
Mettre à jour à distance (Over The Air) le programme de cent capteurs dispersés dans une ferme ou une ville évite de les démonter un par un. Mais un téléchargement interrompu ou corrompu peut transformer chaque capteur en brique inutilisable.

## La solution
Le schéma à deux partitions (A/B). L'appareil fonctionne sur la partition active et télécharge le nouveau micrologiciel dans l'autre, par blocs, chacun vérifié par CRC-32. À la fin, on vérifie la taille, la version (pas de retour arrière) et l'empreinte globale annoncée par le serveur. Seulement alors, on bascule. Au redémarrage, si le nouveau programme ne confirme pas son bon fonctionnement, on revient à l'ancien. On simule une mise à jour réussie, un bloc corrompu (re-téléchargé), une coupure en plein téléchargement et un micrologiciel qui plante au démarrage.

## Ce qu'on apprend
Le principe A/B et du retour arrière, le téléchargement par blocs vérifiés, les métadonnées de version et la robustesse aux pannes.

## Lancer
```bash
gcc -Wall -Wextra -o ota_integrite ota_integrite.c -lm
./ota_integrite
```

## Exemple de sortie
```
Mise à jour v2 avec un bloc abîmé en route (actuellement v1) :
   bloc 3 corrompu, nouvelle demande (tentative 2)
   v2 écrite et vérifiée (CRC 322F06BF) : redémarrage sur la nouvelle partition
   le nouveau programme confirme son bon fonctionnement : v2 validée
   → en service : v2

Mise à jour v3 avec coupure de courant (actuellement v2) :
   coupure de courant au bloc 5 : partition B incomplète, on reste sur A
   → en service : v2

Mise à jour v3 qui plante au démarrage (actuellement v2) :
   v3 écrite et vérifiée (CRC 91670FA8) : redémarrage sur la nouvelle partition
   le nouveau programme plante (watchdog) : retour automatique à v2
   → en service : v2

```
