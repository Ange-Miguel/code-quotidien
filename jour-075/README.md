# Jour 075 — Jeu du pendu avec mots en français

**Langue :** Java

## Le problème
Le pendu est un bon exercice pour manipuler des chaînes : il faut accepter « e » pour « é », « è » ou « ê », afficher les lettres trouvées et gérer les erreurs de saisie.

## La solution
Chaque mot est normalisé (accents supprimés avec java.text.Normalizer) pour la comparaison, mais affiché avec ses accents. Un Set mémorise les lettres proposées. En démo, un joueur automatique propose les lettres dans l'ordre de fréquence du français (e, a, s, i…) ; avec --jouer, c'est vous qui jouez.

## Ce qu'on apprend
Normalizer et les caractères Unicode, les Set, la boucle de jeu, Scanner pour lire la saisie et l'affichage progressif d'un dessin ASCII.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Démo : joueur automatique (lettres par fréquence) sur « _ _ _ _ _ _ _ _ _ »
  e → oui   _ é _ é _ _ _ _ e  (erreurs 0/6)
  a → non   _ é _ é _ _ _ _ e  (erreurs 1/6)
  s → non   _ é _ é _ _ _ _ e  (erreurs 2/6)
  i → non   _ é _ é _ _ _ _ e  (erreurs 3/6)
  t → oui   t é _ é _ _ _ _ e  (erreurs 3/6)
  n → oui   t é _ é _ _ _ n e  (erreurs 3/6)
  r → non   t é _ é _ _ _ n e  (erreurs 4/6)
  u → non   t é _ é _ _ _ n e  (erreurs 5/6)
  l → oui   t é l é _ _ _ n e  (erreurs 5/6)
  o → oui   t é l é _ _ o n e  (erreurs 5/6)
  d → non   t é l é _ _ o n e  (erreurs 6/6)
 O
/|\
/ \
```
