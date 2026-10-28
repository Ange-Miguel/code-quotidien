"""Jour 191 — Générateur de site statique minimal

Problème : un freelance, une association ou une petite boutique veut un site
vitrine rapide, gratuit à héberger (GitHub Pages, Netlify) et sans base de
données ni WordPress à maintenir. Écrire chaque page HTML à la main, avec le
même menu et le même pied de page, est répétitif et source d'erreurs.
Solution : un générateur de site statique : les pages sont écrites en
Markdown avec un en-tête (titre, ordre dans le menu) ; un gabarit HTML commun
contient le menu, généré automatiquement, et le pied de page. Le programme
convertit chaque page (titres, paragraphes, listes, gras, liens), l'insère
dans le gabarit, échappe le HTML, écrit les fichiers dans un dossier de
sortie et produit aussi un plan du site (sitemap.xml). En démo, tout est
généré dans un dossier temporaire.
On apprend : la séparation contenu / présentation, les gabarits avec
string.Template, la génération de fichiers avec pathlib et html.escape.
"""
import html
import re
import sys
import tempfile
from pathlib import Path
from string import Template

GABARIT = Template("""<!doctype html>
<html lang="fr"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width">
<title>$titre — $site</title><style>body{font-family:sans-serif;max-width:46rem;margin:auto;padding:1rem}
nav a{margin-right:1rem}nav a.actif{font-weight:bold}</style></head>
<body><nav>$menu</nav><main>
$contenu
</main><footer><small>© 2026 $site — généré automatiquement</small></footer></body></html>
""")

PAGES = {
    "index.md": "---\ntitre: Accueil\nordre: 1\n---\n# Studio Ange-Miguel\nCréation **graphique**, *motion design* et développement.\n\n- Logos et chartes\n- Vidéos animées\n- Sites et applications\n",
    "services.md": "---\ntitre: Services\nordre: 2\n---\n# Nos services\nTarifs sur devis. Voir aussi la page [contact](contact.html).\n\n## Motion design\nAnimations de 30 s à 2 min.\n",
    "contact.md": "---\ntitre: Contact\nordre: 3\n---\n# Contact\nÉcrivez-nous sur WhatsApp. Les balises <script> sont affichées, pas exécutées.\n",
}


def lire_page(texte: str) -> tuple[dict, str]:
    m = re.match(r"---\n(.*?)\n---\n(.*)", texte, re.S)
    if not m:
        raise ValueError("en-tête --- manquant")
    meta = dict(ligne.split(": ", 1) for ligne in m.group(1).splitlines())
    return meta, m.group(2)


def en_ligne(t: str) -> str:
    t = html.escape(t)
    t = re.sub(r"\*\*(.+?)\*\*", r"<strong>\1</strong>", t)
    t = re.sub(r"\*(.+?)\*", r"<em>\1</em>", t)
    return re.sub(r"\[(.+?)\]\((.+?)\)", r'<a href="\2">\1</a>', t)


def markdown(md: str) -> str:
    sortie, liste = [], False
    for bloc in md.strip().split("\n"):
        if bloc.startswith("- "):
            if not liste:
                sortie.append("<ul>"); liste = True
            sortie.append(f"  <li>{en_ligne(bloc[2:])}</li>")
            continue
        if liste:
            sortie.append("</ul>"); liste = False
        if m := re.match(r"(#{1,3}) (.*)", bloc):
            n = len(m.group(1))
            sortie.append(f"<h{n}>{en_ligne(m.group(2))}</h{n}>")
        elif bloc.strip():
            sortie.append(f"<p>{en_ligne(bloc)}</p>")
    if liste:
        sortie.append("</ul>")
    return "\n".join(sortie)


def generer(pages: dict[str, str], sortie: Path, site: str, url: str) -> list[Path]:
    infos = sorted(((lire_page(t), nom) for nom, t in pages.items()), key=lambda x: int(x[0][0]["ordre"]))
    fichiers = []
    for (meta, contenu), nom in infos:
        cible = nom.replace(".md", ".html")
        menu = " ".join(f'<a href="{n.replace(".md", ".html")}"{" class=actif" if n == nom else ""}>{html.escape(m["titre"])}</a>'
                        for (m, _), n in infos)
        page = GABARIT.substitute(titre=html.escape(meta["titre"]), site=site, menu=menu, contenu=markdown(contenu))
        (sortie / cible).write_text(page, encoding="utf-8")
        fichiers.append(sortie / cible)
    plan = "\n".join(f"  <url><loc>{url}/{f.name}</loc></url>" for f in fichiers)
    (sortie / "sitemap.xml").write_text(f'<?xml version="1.0"?>\n<urlset>\n{plan}\n</urlset>\n', encoding="utf-8")
    return fichiers + [sortie / "sitemap.xml"]


def main() -> int:
    if len(sys.argv) == 2:
        dossier = Path(sys.argv[1])
        if not dossier.is_dir():
            print(f"Dossier introuvable : {dossier}", file=sys.stderr)
            return 1
        pages = {p.name: p.read_text(encoding="utf-8") for p in dossier.glob("*.md")}
        sortie = dossier / "site"
        sortie.mkdir(exist_ok=True)
        print("\n".join(str(f) for f in generer(pages, sortie, "Mon site", "https://exemple.bj")))
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 site_statique.py [dossier contenant des pages .md]", file=sys.stderr)
        return 1
    with tempfile.TemporaryDirectory() as d:
        fichiers = generer(PAGES, Path(d), "Studio Ange-Miguel", "https://exemple.github.io")
        print("Fichiers générés :")
        for f in fichiers:
            print(f"   {f.name:14} {f.stat().st_size:5d} octets")
        print("\nExtrait de services.html :")
        lignes = (Path(d) / "services.html").read_text(encoding="utf-8").splitlines()
        print("\n".join("   " + l for l in lignes[5:11]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
