# Jour 027 — Liste de courses en ligne de commande (ajouter, retirer, afficher)

**Langue :** Java

## Le problème
On oublie toujours quelque chose au marché de Dantokpa. Une liste persistante, qu'on complète au fil de la semaine, règle le problème.

## La solution
La liste est enregistrée dans un fichier texte (une ligne par article : quantité;nom). Des commandes simples permettent d'ajouter, retirer, cocher et afficher. Une LinkedHashMap garde l'ordre d'ajout et fusionne les doublons.

## Ce qu'on apprend
La persistance dans un fichier, les Map, la lecture/écriture avec java.nio et la conception d'une petite interface en ligne de commande.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Démo (sans fichier) :
  [ ]   3 × tomates
  [ ]   1 × riz 25 kg
  [ ]   3 × piment
  3 article(s) différent(s)
Retrait du piment : true
  [ ]   3 × tomates
  [ ]   1 × riz 25 kg
  2 article(s) différent(s)

Commandes : ajouter <nom> [qté] | retirer <nom> | voir | vider
```
