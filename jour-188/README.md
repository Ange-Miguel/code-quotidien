# Jour 188 — Hachage de mots de passe avec sel

**Langue :** Python

## Le problème
Si la base de données d'un site est volée et que les mots de passe y sont en clair, ou simplement hachés avec SHA-256, les pirates les retrouvent en quelques secondes : deux utilisateurs avec le même mot de passe ont la même empreinte, et des tables précalculées donnent les mots de passe courants.

## La solution
On ne stocke jamais le mot de passe. On stocke un sel aléatoire (16 octets, différent pour chaque utilisateur) et le résultat d'une fonction de dérivation volontairement LENTE : PBKDF2-HMAC-SHA256 avec 200 000 itérations (module hashlib), ou scrypt. La vérification recalcule et compare en temps constant (hmac.compare_digest). On montre l'attaque par dictionnaire contre un SHA-256 simple, puis son coût contre PBKDF2.

## Ce qu'on apprend
La différence entre hacher et chiffrer, le sel, le coût volontaire (itérations), le format de stockage et la comparaison sûre.

## Lancer
```bash
python3 hachage_mdp.py
```

## Exemple de sortie
```
1) Mauvaise pratique : SHA-256 sans sel
   awa   6f9a4524b32990e5b71c8fae…
   kofi  6f9a4524b32990e5b71c8fae…
   ines  16020a0ec898a34dce0c7c36…
   → Awa et Kofi ont la même empreinte : on voit qu'ils partagent le même mot de passe.
   Attaque par dictionnaire : ['awa', 'kofi'] trouvés en 0.1 ms

2) Bonne pratique : PBKDF2-SHA256, sel aléatoire, 200 000 itérations
   awa   pbkdf2_sha256$200000$4b79014b9…d0c209181a03
   kofi  pbkdf2_sha256$200000$e71baa9a9…974b4196eb69
   ines  pbkdf2_sha256$200000$1423312e1…e1ce72d7b7d2
   → même mot de passe, empreintes différentes grâce au sel.
   Vérification Awa « benin229 » : True | « Benin229 » : False
   Un essai coûte 26 ms : tester 10 millions de mots de passe prendrait ~3 jours par compte.

```
