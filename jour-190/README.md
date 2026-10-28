# Jour 190 — Conversion Markdown → HTML simple

**Langue :** C

## Le problème
Écrire directement du HTML pour un blog, une documentation ou une fiche produit est fastidieux. Le Markdown (celui des README de GitHub) est beaucoup plus simple à taper : # pour un titre, **gras**, *italique*, - pour une liste, [texte](lien). Il faut ensuite le convertir en HTML.

## La solution
Un convertisseur ligne par ligne avec un petit automate : on sait si on est dans un paragraphe, une liste ou un bloc de code, et on ferme les balises au bon moment. Dans chaque ligne, un analyseur traite les éléments en ligne (gras, italique, code, liens) et échappe les caractères spéciaux (<, >, &) pour éviter qu'un texte injecte du HTML ou du script.

## Ce qu'on apprend
L'analyse de texte par états, l'écriture dans un tampon, l'échappement HTML (sécurité) et la structure d'un document HTML.

## Lancer
```bash
gcc -Wall -Wextra -o markdown_html markdown_html.c -lm
./markdown_html
```

## Exemple de sortie
```
<h1>Ma boutique</h1>
<p>Bienvenue ! Nos produits sont <strong>importés directement</strong> et <em>garantis 6 mois</em>. Livraison à Cotonou et Calavi.</p>
<h2>Nos best-sellers</h2>
<ul>
  <li><strong>Écouteurs</strong> Bluetooth : 4 500 FCFA</li>
  <li>Lampe solaire, idéale pendant les coupures</li>
  <li>Chargeur rapide <code>20 W</code></li>
</ul>
<p>Contact : <a href="https://wa.me/22901000000">WhatsApp</a> ou <a href="#">ce lien piégé</a></p>
<pre><code>if (prix &lt; 5000) { promo = true; }
</code></pre>
<p>Attention : 2 &lt; 3 &amp; &lt;script&gt; n'est pas exécuté.</p>
```
