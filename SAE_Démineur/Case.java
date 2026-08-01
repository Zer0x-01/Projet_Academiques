import java.io.Serializable;

/**
 * Représente une case de la grille du Démineur.
 * Une case peut être minée, révélée, et porter un marqueur.
 *
 * @author Etudiant
 * @version 1.0
 */
public class Case implements Serializable {

    /** Identifiant de version pour la sérialisation. */
    private static final long serialVersionUID = 1L;

    /** Indique si cette case contient une mine. */
    private boolean estMinee;

    /** Indique si cette case a été révélée par le joueur. */
    private boolean estRevelee;

    /**
     * Marqueur posé par le joueur.
     * 0 = aucun, 1 = certitude étoile, 2 = soupçon ?
     */
    private int marqueur;

    /** Nombre de mines dans les cases adjacentes (0 à 8). */
    private int minesAdjacentes;

    /**
     * Construit une case initialement cachée, saine et sans marqueur.
     */
    public Case() {
        this.estMinee = false;
        this.estRevelee = false;
        this.marqueur = 0;
        this.minesAdjacentes = 0;
    }

    /**
     * Retourne vrai si la case est minée.
     *
     * @return true si la case contient une mine
     */
    public boolean isEstMinee() {
        return estMinee;
    }

    /**
     * Définit si la case est minée.
     *
     * @param estMinee true si la case doit contenir une mine
     */
    public void setEstMinee(boolean estMinee) {
        this.estMinee = estMinee;
    }

    /**
     * Retourne vrai si la case a été révélée.
     *
     * @return true si la case est révélée
     */
    public boolean isEstRevelee() {
        return estRevelee;
    }

    /**
     * Définit si la case est révélée.
     *
     * @param estRevelee true si la case doit être révélée
     */
    public void setEstRevelee(boolean estRevelee) {
        this.estRevelee = estRevelee;
    }

    /**
     * Retourne le marqueur actuel de la case.
     *
     * @return 0 (aucun), 1 (étoile) ou 2 (?)
     */
    public int getMarqueur() {
        return marqueur;
    }

    /**
     * Retourne le nombre de mines adjacentes à cette case.
     *
     * @return nombre de mines voisines, entre 0 et 8
     */
    public int getMinesAdjacentes() {
        return minesAdjacentes;
    }

    /**
     * Définit le nombre de mines adjacentes.
     *
     * @param minesAdjacentes le nombre calculé par la Grille
     */
    public void setMinesAdjacentes(int minesAdjacentes) {
        this.minesAdjacentes = minesAdjacentes;
    }

    /**
     * Fait avancer le marqueur dans le cycle :
     * 0 (vide) → 1 (étoile) → 2 (?) → 0 (vide).
     */
    public void cyclerMarqueur() {
        this.marqueur = (this.marqueur + 1) % 3;
    }
}