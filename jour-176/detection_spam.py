"""Jour 176 — Détection de spam par mots-clés

Problème : la boîte de réception d'une boutique ou le formulaire de contact
d'un site reçoit des dizaines de messages indésirables (« Vous avez gagné »,
« Cliquez ici », faux transferts d'argent…), qui noient les vrais clients.
Solution : un filtre simple et explicable. Chaque message reçoit un score :
mots et expressions suspects pondérés, nombre de liens, proportion de
MAJUSCULES, points d'exclamation répétés, montants d'argent mirobolants,
demande de code ou de mot de passe. Au-delà d'un seuil, le message est classé
spam, et le programme affiche les raisons, ce qui permet d'ajuster les
poids. On mesure enfin la précision sur un petit jeu de messages étiquetés.
On apprend : les expressions régulières, la normalisation des accents, les
scores à règles pondérées, et les mesures précision / rappel.
"""
import re
import sys
import unicodedata

REGLES = [
    (r"\bvous avez gagne\b", 3.0, "« vous avez gagné »"), (r"\bfelicitations\b", 1.0, "« félicitations »"),
    (r"\bcliquez (ici|sur ce lien)\b", 2.0, "« cliquez ici »"), (r"\burgent\b", 1.0, "« urgent »"),
    (r"\b(code|mot de passe|pin)\b.*\b(envoyez|communiquez|donnez)\b|\b(envoyez|communiquez|donnez)\b.*\b(code|mot de passe|pin)\b", 3.0, "demande de code secret"),
    (r"\bheritage\b|\btransfert de fonds\b", 2.5, "héritage / transfert"), (r"\bgratuit\b", 1.0, "« gratuit »"),
    (r"\b100 ?% (garanti|gratuit)\b", 1.5, "promesse « 100 % »"), (r"\bcrypto|bitcoin\b", 1.5, "cryptomonnaie"),
]
SEUIL = 4.0


def normaliser(texte: str) -> str:
    return unicodedata.normalize("NFD", texte.lower()).encode("ascii", "ignore").decode()


def score(message: str) -> tuple[float, list[str]]:
    t, raisons, s = normaliser(message), [], 0.0
    for motif, poids, libelle in REGLES:
        if re.search(motif, t):
            s += poids
            raisons.append(f"{libelle} (+{poids})")
    liens = len(re.findall(r"https?://|www\.", t))
    if liens >= 2:
        s += 1.5; raisons.append(f"{liens} liens (+1.5)")
    lettres = [c for c in message if c.isalpha()]
    if lettres and sum(c.isupper() for c in lettres) / len(lettres) > 0.5:
        s += 1.5; raisons.append("trop de MAJUSCULES (+1.5)")
    if re.search(r"!{3,}", message):
        s += 1.0; raisons.append("!!! (+1)")
    if re.search(r"\d[\d .]{6,}\s*(fcfa|f cfa|euros?|\$|usd)", t):
        s += 1.5; raisons.append("grosse somme d'argent (+1.5)")
    return s, raisons


MESSAGES = [
    ("Bonjour, avez-vous encore les écouteurs à 4500 ? Je passe demain à Calavi.", False),
    ("FÉLICITATIONS !!! VOUS AVEZ GAGNÉ 5 000 000 FCFA. Cliquez ici : http://gain.xyz", True),
    ("Votre colis est bloqué. Envoyez le code reçu par SMS pour le débloquer, c'est urgent.", True),
    ("Bonsoir, je voudrais un devis pour un logo et une affiche pour mon restaurant.", False),
    ("Héritage de 2 500 000 $ : transfert de fonds possible, répondez vite www.x.com www.y.com", True),
    ("Rappel : réunion de l'association samedi à 10 h. Gratuit pour les membres.", False),
    ("Investissez en bitcoin, 100 % garanti, doublez votre argent !!!", True),
    ("Urgent : la livraison de demain est décalée à 15 h, merci de confirmer.", False),
]


def main() -> int:
    if len(sys.argv) == 2:
        s, raisons = score(sys.argv[1])
        print(f"Score {s:.1f} → {'SPAM' if s >= SEUIL else 'normal'}" + (f" ({', '.join(raisons)})" if raisons else ""))
        return 0
    if len(sys.argv) > 2:
        print("Usage : python3 detection_spam.py [\"message\"]", file=sys.stderr)
        return 1
    vp = fp = fn = vn = 0
    for message, est_spam in MESSAGES:
        s, raisons = score(message)
        predit = s >= SEUIL
        vp += predit and est_spam; fp += predit and not est_spam; fn += (not predit) and est_spam; vn += not predit and not est_spam
        print(f"{'SPAM  ' if predit else 'normal'} {s:4.1f}  {message[:62]}{'…' if len(message) > 62 else ''}")
        if raisons:
            print(f"              {'; '.join(raisons)}")
    print(f"\nPrécision {vp / max(1, vp + fp):.0%} (spams détectés qui en sont vraiment) | rappel {vp / max(1, vp + fn):.0%} "
          f"(spams attrapés) | faux positifs {fp}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
