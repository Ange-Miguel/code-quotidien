/*
 * Jour 033 — Recherche du maximum et du minimum dans une liste de prix
 *
 * Problème : on compare les prix d'un même produit chez plusieurs fournisseurs
 * ou vendeurs : qui est le moins cher, le plus cher, et quel est l'écart ?
 * Solution : un seul parcours de la liste suffit pour trouver à la fois le
 * minimum et le maximum (et leur position). On calcule aussi l'économie
 * réalisée en choisissant le moins cher et le prix médian du marché. On
 * compare avec l'API Stream de Java.
 * On apprend : l'algorithme de recherche min/max en un passage, les records,
 * les comparateurs et l'API Stream.
 */
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

