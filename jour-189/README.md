# Jour 189 — Validation de formulaire d'inscription

**Langue :** Java

## Le problème
Un formulaire d'inscription (application de covoiturage, association, boutique) reçoit des saisies incomplètes, mal formatées ou malveillantes. Valider seulement dans le navigateur ne suffit pas : le serveur doit tout revérifier, et les messages d'erreur doivent aider l'utilisateur à corriger, champ par champ.

## La solution
Un petit moteur de validation : chaque champ a une liste de règles (obligatoire, longueur, format par expression régulière, plage de valeurs, règle croisée comme « confirmation identique ») qui renvoient un message clair. On normalise d'abord les saisies (espaces, majuscules du nom, téléphone). Toutes les erreurs sont collectées en une fois, plutôt que la première seulement. Les données ne sont jamais réaffichées sans échappement.

## Ce qu'on apprend
Les interfaces fonctionnelles et les lambdas, les expressions régulières, la normalisation des entrées et la collecte d'erreurs.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
Inscription de «   akpanon   ange-miguel  » :
   normalisé : Akpanon Ange-miguel | ange.miguel@exemple.bj | +2290166546144
   ✔ valide, compte créé

Inscription de « &lt;script&gt;alert(1)&lt;/script&gt; » :
   ✘ nom ne doit contenir que des lettres
   ✘ email n'est pas une adresse e-mail valide
   ✘ telephone doit être un numéro béninois (01 + 8 chiffres)
   ✘ naissance il faut avoir au moins 16 ans
   ✘ motDePasse doit faire entre 10 et 128 caractères
   ✘ confirmation ne correspond pas au mot de passe

Inscription de « Awa » :
   ✘ email est obligatoire
   ✘ naissance doit être au format AAAA-MM-JJ
```
