/*
 * Jour 129 — Simulation d'une EEPROM avec usure (wear leveling)
 *
 * Problème : un compteur d'eau ou d'énergie enregistre son index en mémoire
 * non volatile (EEPROM ou Flash) pour ne pas le perdre en cas de coupure.
 * Mais chaque case ne supporte qu'environ 100 000 écritures : en écrivant
 * toujours à la même adresse toutes les minutes, la mémoire meurt en
 * 70 jours !
 * Solution : la répartition d'usure (wear leveling). Au lieu d'une seule
 * case, on utilise un anneau de N emplacements et on écrit chaque nouvelle
 * valeur dans l'emplacement suivant, avec un numéro de séquence. Au
 * démarrage, on retrouve la valeur la plus récente en cherchant le plus grand
 * numéro de séquence valide (avec somme de contrôle). On simule l'usure, une
 * coupure pendant une écriture, et on compare la durée de vie.
 * On apprend : les limites physiques des mémoires, le stockage circulaire,
 * les numéros de séquence, la reprise après coupure et l'estimation de durée
 * de vie.
 */
import java.util.Locale;

public class Main {
    static final int CYCLES_MAX = 100_000;

    static class Eeprom {
        final long[] valeur, sequence; final int[] controle, ecritures;
        Eeprom(int cases) { valeur = new long[cases]; sequence = new long[cases]; controle = new int[cases]; ecritures = new int[cases]; }
        void ecrire(int i, long v, long seq, boolean coupure) {
            if (ecritures[i] >= CYCLES_MAX) throw new IllegalStateException("case " + i + " usée");
            ecritures[i]++;
            valeur[i] = v; sequence[i] = seq;
            controle[i] = coupure ? 0xDEAD : somme(v, seq);   // coupure : contrôle non écrit
        }
        static int somme(long v, long seq) { return (int) ((v * 31 + seq * 17) & 0xFFFF) ^ 0x5A5A; }
        boolean valide(int i) { return sequence[i] > 0 && controle[i] == somme(valeur[i], sequence[i]); }
    }

    static class Stockage {
        final Eeprom mem; long seq = 0; int position = -1;
        Stockage(Eeprom mem) { this.mem = mem; }
        void sauver(long v, boolean coupure) { position = (position + 1) % mem.valeur.length; mem.ecrire(position, v, ++seq, coupure); }
        long relire() {                                         // au démarrage
            long meilleurSeq = 0, v = 0;
            for (int i = 0; i < mem.valeur.length; i++)
                if (mem.valide(i) && mem.sequence[i] > meilleurSeq) { meilleurSeq = mem.sequence[i]; v = mem.valeur[i]; position = i; }
            seq = meilleurSeq;
            return v;
        }
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.FRANCE);
        int cases = 64;
        if (args.length == 1) {
            try { cases = Integer.parseInt(args[0]); if (cases < 1 || cases > 4096) throw new NumberFormatException(); }
            catch (NumberFormatException e) { System.err.println("Usage : java Main [nombre d'emplacements 1..4096]"); System.exit(1); }
        }
        Stockage s = new Stockage(new Eeprom(cases));
        for (long index = 1; index <= 1000; index++) s.sauver(index * 10, false);   // index du compteur en litres
        s.sauver(10_010, true);                                                    // coupure pendant l'écriture
        Stockage apresCoupure = new Stockage(s.mem);
        System.out.printf("Après redémarrage, valeur relue : %,d L (la dernière écriture, interrompue, est ignorée)%n", apresCoupure.relire());
        apresCoupure.sauver(10_010, false);
        System.out.printf("Nouvelle écriture correcte, valeur relue : %,d L%n%n", new Stockage(s.mem).relire());

        int min = Integer.MAX_VALUE, max = 0;
        for (int e : s.mem.ecritures) { min = Math.min(min, e); max = Math.max(max, e); }
        System.out.printf("Usure après 1 002 écritures sur %d emplacements : de %d à %d écritures par case%n%n", cases, min, max);

        double parJour = 24 * 60;   // une sauvegarde par minute
        System.out.printf("Durée de vie avec une sauvegarde par minute (%,d cycles par case) :%n", CYCLES_MAX);
        for (int n : new int[]{1, 8, 64, 512}) {
            double jours = (double) CYCLES_MAX * n / parJour;
            System.out.printf("   %4d emplacement(s) : %8.0f jours (%.1f ans)%n", n, jours, jours / 365);
        }
    }
}
