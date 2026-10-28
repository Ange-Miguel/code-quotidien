# Jour 063 — Table de hachage : dictionnaire français → anglais

**Langue :** Java

## Le problème
Comment un dictionnaire, un cache ou un annuaire retrouve-t-il un mot parmi des millions en temps quasi constant ? Grâce aux tables de hachage, qu'on utilise tous les jours avec HashMap sans les connaître.

## La solution
On écrit notre propre table de hachage : une fonction de hachage (polynomiale, comme String.hashCode) transforme le mot en indice de case ; les collisions sont gérées par chaînage (liste dans chaque case) ; quand le taux de remplissage dépasse 0,75, on double la taille et on redistribue.

## Ce qu'on apprend
Le hachage, les collisions, le facteur de charge, le redimensionnement et les classes génériques en Java.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Insertion de 20 mots :
   (redimensionnement : 4 → 8 cases)
   (redimensionnement : 8 → 16 cases)
   (redimensionnement : 16 → 32 cases)
   20 mots, 32 cases (17 vides), chaîne la plus longue : 2
capteur      → sensor
livraison    → delivery
réseau       → network
zemidjan     → (absent du dictionnaire)
```
