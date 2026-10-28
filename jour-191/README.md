# Jour 191 — Générateur de site statique minimal

**Langue :** Python

## Le problème
Un freelance, une association ou une petite boutique veut un site vitrine rapide, gratuit à héberger (GitHub Pages, Netlify) et sans base de données ni WordPress à maintenir. Écrire chaque page HTML à la main, avec le même menu et le même pied de page, est répétitif et source d'erreurs.

## La solution
Un générateur de site statique : les pages sont écrites en Markdown avec un en-tête (titre, ordre dans le menu) ; un gabarit HTML commun contient le menu, généré automatiquement, et le pied de page. Le programme convertit chaque page (titres, paragraphes, listes, gras, liens), l'insère dans le gabarit, échappe le HTML, écrit les fichiers dans un dossier de sortie et produit aussi un plan du site (sitemap.xml). En démo, tout est généré dans un dossier temporaire.

## Ce qu'on apprend
La séparation contenu / présentation, les gabarits avec string.Template, la génération de fichiers avec pathlib et html.escape.

## Lancer
```bash
python3 site_statique.py
```

## Exemple de sortie
```
Fichiers générés :
   index.html       768 octets
   services.html    715 octets
   contact.html     666 octets
   sitemap.xml      229 octets

Extrait de services.html :
   <h1>Nos services</h1>
   <p>Tarifs sur devis. Voir aussi la page <a href="contact.html">contact</a>.</p>
   <h2>Motion design</h2>
   <p>Animations de 30 s à 2 min.</p>
   </main><footer><small>© 2026 Studio Ange-Miguel — généré automatiquement</small></footer></body></html>
```
