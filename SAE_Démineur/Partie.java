import java.io.Serializable;

/**
 * Représente l'état complet d'une partie en cours.
 * Contient la grille de jeu et le temps écoulé depuis le début.
 *
 * @author Etudiant
 * @version 1.0
 */
public class Partie implements Serializable {

    /** Identifiant de version pour la sérialisation. */
    private static final long serialVersionUID = 1L;

    /** La grille associée à cette partie. */
    private Grille grille;

    /** Temps écoulé en secondes depuis le début de la partie. */
    private long tempsEcoule;

    /**
     * Construit une nouvelle partie avec une grille aux dimensions et mines données.
     *
     * @param nbLignes   le nombre de lignes de la grille
     * @param nbColonnes le nombre de colonnes de la grille
     * @param nbMines    le nombre de mines à placer
     */
    public Partie(int nbLignes, int nbColonnes, int nbMines) {
        this.grille = new Grille(nbLignes, nbColonnes, nbMines);
        this.tempsEcoule = 0;
    }

    /**
     * Retourne la grille de jeu.
     *
     * @return la grille associée à cette partie
     */
    public Grille getGrille() {
        return grille;
    }

    /**
     * Retourne le temps écoulé depuis le début de la partie.
     *
     * @return le temps en secondes
     */
    public long getTempsEcoule() {
        return tempsEcoule;
    }

    /**
     * Incrémente le temps écoulé d'une seconde.
     * Appelée chaque seconde par le minuteur de FenetreJeu.
     */
    public void incrementerTemps() {
        this.tempsEcoule++;
    }
}