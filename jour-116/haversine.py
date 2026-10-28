"""Jour 116 — Distance entre deux coordonnées GPS (formule de Haversine)

Problème : une application de livraison, de covoiturage ou de géolocalisation
de bouteilles de gaz doit calculer la distance entre deux points GPS et
trouver le point de vente le plus proche d'un client.
Solution : la Terre étant (presque) une sphère, la distance n'est pas celle
d'un plan. La formule de Haversine donne la distance du grand cercle à partir
des latitudes et longitudes en radians. On calcule aussi le cap initial
(direction à suivre), on compare avec l'approximation plane (valable sur de
courtes distances) et on classe des points de vente par proximité. Les
coordonnées sont approximatives, pour l'exemple.
On apprend : la trigonométrie sphérique, les radians, atan2, la précision
des approximations et le tri par distance.
"""
import math
import sys

RAYON_TERRE_KM = 6371.0

VILLES = {"Cotonou": (6.3654, 2.4183), "Abomey-Calavi": (6.4485, 2.3557), "Porto-Novo": (6.4969, 2.6289),
          "Parakou": (9.3372, 2.6303), "Natitingou": (10.3042, 1.3797), "Lagos": (6.5244, 3.3792),
          "Lomé": (6.1375, 1.2123), "Paris": (48.8566, 2.3522), "Guangzhou": (23.1291, 113.2644)}


def haversine(a: tuple[float, float], b: tuple[float, float]) -> float:
    lat1, lon1, lat2, lon2 = map(math.radians, (*a, *b))
    h = math.sin((lat2 - lat1) / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin((lon2 - lon1) / 2) ** 2
    return 2 * RAYON_TERRE_KM * math.asin(math.sqrt(h))


def approx_plane(a, b) -> float:
    x = math.radians(b[1] - a[1]) * math.cos(math.radians((a[0] + b[0]) / 2))
    y = math.radians(b[0] - a[0])
    return RAYON_TERRE_KM * math.hypot(x, y)


def cap(a, b) -> float:
    lat1, lon1, lat2, lon2 = map(math.radians, (*a, *b))
    x = math.sin(lon2 - lon1) * math.cos(lat2)
    y = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(lon2 - lon1)
    return (math.degrees(math.atan2(x, y)) + 360) % 360


def direction(degres: float) -> str:
    return ["N", "NE", "E", "SE", "S", "SO", "O", "NO"][round(degres / 45) % 8]


def main() -> int:
    if len(sys.argv) == 5:
        try:
            a, b = (float(sys.argv[1]), float(sys.argv[2])), (float(sys.argv[3]), float(sys.argv[4]))
            if not all(-90 <= p[0] <= 90 and -180 <= p[1] <= 180 for p in (a, b)):
                raise ValueError
        except ValueError:
            print("Usage : python3 haversine.py lat1 lon1 lat2 lon2 (degrés décimaux)", file=sys.stderr)
            return 1
        print(f"Distance : {haversine(a, b):.3f} km, cap {cap(a, b):.0f}° ({direction(cap(a, b))})")
        return 0
    if len(sys.argv) != 1:
        print("Usage : python3 haversine.py [lat1 lon1 lat2 lon2]", file=sys.stderr)
        return 1
    depart = VILLES["Cotonou"]
    print(f"{'De Cotonou à':15} {'Haversine':>11} {'Approx. plane':>14} {'Écart':>7}  Cap")
    for nom, pos in VILLES.items():
        if nom == "Cotonou":
            continue
        h, p = haversine(depart, pos), approx_plane(depart, pos)
        print(f"{nom:15} {h:9.1f} km {p:11.1f} km {abs(h - p) / h:6.2%}  {cap(depart, pos):3.0f}° {direction(cap(depart, pos))}")
    client = (6.4402, 2.3443)       # un client à Calavi (exemple)
    points = {"Point gaz Kpota": (6.4527, 2.3496), "Point gaz Zogbadjè": (6.4170, 2.3427),
              "Point gaz Godomey": (6.3912, 2.3301), "Point gaz Akassato": (6.5041, 2.3613)}
    print(f"\nPoints de vente les plus proches du client {client} :")
    for nom, pos in sorted(points.items(), key=lambda x: haversine(client, x[1])):
        print(f"   {nom:20} {haversine(client, pos) * 1000:6.0f} m vers le {direction(cap(client, pos))}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
