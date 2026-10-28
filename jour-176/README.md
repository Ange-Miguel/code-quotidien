# Jour 176 — Détection de spam par mots-clés

**Langue :** Python

## Le problème
La boîte de réception d'une boutique ou le formulaire de contact d'un site reçoit des dizaines de messages indésirables (« Vous avez gagné », « Cliquez ici », faux transferts d'argent…), qui noient les vrais clients.

## La solution
Un filtre simple et explicable. Chaque message reçoit un score : mots et expressions suspects pondérés, nombre de liens, proportion de MAJUSCULES, points d'exclamation répétés, montants d'argent mirobolants, demande de code ou de mot de passe. Au-delà d'un seuil, le message est classé spam, et le programme affiche les raisons, ce qui permet d'ajuster les poids. On mesure enfin la précision sur un petit jeu de messages étiquetés.

## Ce qu'on apprend
Les expressions régulières, la normalisation des accents, les scores à règles pondérées, et les mesures précision / rappel.

## Lancer
```bash
python3 detection_spam.py
```

## Exemple de sortie
```
normal  0.0  Bonjour, avez-vous encore les écouteurs à 4500 ? Je passe dema…
SPAM   10.0  FÉLICITATIONS !!! VOUS AVEZ GAGNÉ 5 000 000 FCFA. Cliquez ici …
              « vous avez gagné » (+3.0); « félicitations » (+1.0); « cliquez ici » (+2.0); trop de MAJUSCULES (+1.5); !!! (+1); grosse somme d'argent (+1.5)
SPAM    4.0  Votre colis est bloqué. Envoyez le code reçu par SMS pour le d…
              « urgent » (+1.0); demande de code secret (+3.0)
normal  0.0  Bonsoir, je voudrais un devis pour un logo et une affiche pour…
SPAM    5.5  Héritage de 2 500 000 $ : transfert de fonds possible, réponde…
              héritage / transfert (+2.5); 2 liens (+1.5); grosse somme d'argent (+1.5)
normal  1.0  Rappel : réunion de l'association samedi à 10 h. Gratuit pour …
              « gratuit » (+1.0)
SPAM    4.0  Investissez en bitcoin, 100 % garanti, doublez votre argent !!…
              promesse « 100 % » (+1.5); cryptomonnaie (+1.5); !!! (+1)
normal  1.0  Urgent : la livraison de demain est décalée à 15 h, merci de c…
              « urgent » (+1.0)

```
