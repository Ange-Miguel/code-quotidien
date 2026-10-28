# Jour 177 — Analyse de sentiment simple d'avis clients

**Langue :** Java

## Le problème
Une boutique reçoit chaque semaine des dizaines d'avis et de commentaires. Les lire tous prend du temps ; on voudrait savoir rapidement ce qui plaît, ce qui fâche, et repérer les clients mécontents à recontacter.

## La solution
Une analyse à base de lexique : chaque mot porte une polarité (« excellent » +2, « déçu » −2…). On gère les négations (« pas bon » inverse le mot suivant), les intensificateurs (« très », « trop »), puis on normalise le score. On extrait aussi les thèmes cités (livraison, prix, qualité, accueil) pour savoir de quoi parlent les avis positifs et négatifs. C'est simple et explicable ; l'ironie lui échappe, ce qu'on montre aussi.

## Ce qu'on apprend
Le traitement de texte (découpage en mots, normalisation), les Map de lexiques, la gestion du contexte (négation) et l'agrégation.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
+1,00 positif 🙂 Livraison très rapide, produit excellent. Je recommande !
-1,00 NÉGATIF ☹  Écouteurs cassés au bout d'une semaine, vraiment déçu de la qualité.
-0,18 neutre 😐  Le vendeur était gentil mais le prix est trop cher.
+1,00 positif 🙂 Pas mauvais du tout, bon rapport qualité prix.
+0,71 positif 🙂 Aucun problème, livré à Calavi le jour même. Merci !
-1,00 NÉGATIF ☹  Commande jamais livrée, arnaque !
+0,35 positif 🙂 Super, encore un retard de livraison... bravo.

Sentiment moyen par thème :
   accueil    -0,18 (1 avis)
   livraison  +0,69 (3 avis)
   prix       +0,41 (2 avis)
   qualité    +0,00 (2 avis)

Limite : l'ironie du dernier avis (« Super… bravo ») trompe le lexique.
```
