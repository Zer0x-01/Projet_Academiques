import java.io.Serializable;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;

/**
 * Représente la grille de jeu du Démineur.
 * Gère le placement des mines, le calcul des adjacences,
 * la révélation des cases et la détection de victoire.
 *
 * @author Etudiant
 * @version 1.0
 */
public class Grille implements Serializable {

    /** Identifiant de version pour la sérialisation. */
    private static final long serialVersionUID = 1L;

    /** Tableau bidimensionnel contenant toutes les cases de la grille. */
    private Case[][] cases;

    /** Nombre de lignes de la grille. */
    private int nbLignes;

    /** Nombre de colonnes de la grille. */
    private int nbColonnes;

    /** Nombre total de mines placées dans la grille. */
    private int nbMines;

    /**
     * Construit une grille avec les dimensions et le nombre de mines donnés.
     * Les mines sont placées aléatoirement et les adjacences sont calculées.
     *
     * @param nbLignes   le nombre de lignes
     * @param nbColonnes le nombre de colonnes
     * @param nbMines    le nombre de mines à placer
     */
    public Grille(int nbLignes, int nbColonnes, int nbMines) {
        this.nbLignes = nbLignes;
        this.nbColonnes = nbColonnes;
        this.nbMines = nbMines;
        this.cases = new Case[nbLignes][nbColonnes];

        for (int i = 0; i < nbLignes; i++) {
            for (int j = 0; j < nbColonnes; j++) {
                cases[i][j] = new Case();
            }
        }

        placerMines();
        calculerAdjacences();
    }

    /**
     * Place les mines aléatoirement dans la grille.
     */
    private void placerMines() {
        Random rand = new Random();
        int minesPlacees = 0;

        while (minesPlacees < nbMines) {
            int ligne = rand.nextInt(nbLignes);
            int col = rand.nextInt(nbColonnes);

            if (!cases[ligne][col].isEstMinee()) {
                cases[ligne][col].setEstMinee(true);
                minesPlacees++;
            }
        }
    }

    /**
     * Calcule et affecte le nombre de mines adjacentes pour chaque case non minée.
     */
    private void calculerAdjacences() {
        for (int i = 0; i < nbLignes; i++) {
            for (int j = 0; j < nbColonnes; j++) {
                if (!cases[i][j].isEstMinee()) {
                    cases[i][j].setMinesAdjacentes(compterMinesAutour(i, j));
                }
            }
        }
    }

    /**
     * Compte le nombre de mines dans les cases voisines de la case donnée.
     *
     * @param ligne   la ligne de la case
     * @param colonne la colonne de la case
     * @return le nombre de mines adjacentes
     */
    private int compterMinesAutour(int ligne, int colonne) {
        int compte = 0;
        for (int i = ligne - 1; i <= ligne + 1; i++) {
            for (int j = colonne - 1; j <= colonne + 1; j++) {
                if (i >= 0 && i < nbLignes && j >= 0 && j < nbColonnes) {
                    if (cases[i][j].isEstMinee()) {
                        compte++;
                    }
                }
            }
        }
        return compte;
    }

    /**
     * Traite un clic gauche sur la case aux coordonnées données.
     * Révèle la case et propage si nécessaire.
     *
     * @param ligne   la ligne cliquée
     * @param colonne la colonne cliquée
     * @return 0 si la partie continue, 1 si victoire, -1 si défaite
     */
    public int cliquerGauche(int ligne, int colonne) {
        Case c = cases[ligne][colonne];

        if (c.isEstRevelee() || c.getMarqueur() != 0) {
            return 0;
        }

        if (c.isEstMinee()) {
            c.setEstRevelee(true);
            return -1;
        }

        revelerBFS(ligne, colonne);

        if (estVictoire()) {
            return 1;
        }
        return 0;
    }

    /**
     * Révèle en cascade toutes les cases vides accessibles depuis la position donnée,
     * en utilisant un parcours en largeur (BFS).
     *
     * @param ligneDepart   la ligne de départ
     * @param colonneDepart la colonne de départ
     */
    private void revelerBFS(int ligneDepart, int colonneDepart) {
        Queue<int[]> file = new LinkedList<int[]>();
        file.add(new int[]{ligneDepart, colonneDepart});

        while (!file.isEmpty()) {
            int[] coord = file.poll();
            int l = coord[0];
            int c = coord[1];

            Case cas = cases[l][c];

            if (cas.isEstRevelee() || cas.getMarqueur() != 0) {
                continue;
            }

            cas.setEstRevelee(true);

            if (cas.getMinesAdjacentes() == 0) {
                for (int i = l - 1; i <= l + 1; i++) {
                    for (int j = c - 1; j <= c + 1; j++) {
                        if (i >= 0 && i < nbLignes && j >= 0 && j < nbColonnes) {
                            if (!cases[i][j].isEstRevelee()) {
                                file.add(new int[]{i, j});
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Traite un clic droit sur la case aux coordonnées données.
     * Fait cycler le marqueur de la case si elle n'est pas révélée.
     *
     * @param ligne   la ligne cliquée
     * @param colonne la colonne cliquée
     */
    public void cliquerDroit(int ligne, int colonne) {
        Case c = cases[ligne][colonne];
        if (!c.isEstRevelee()) {
            c.cyclerMarqueur();
        }
    }

    /**
     * Vérifie si la partie est gagnée, c'est-à-dire si toutes les cases
     * sans mine ont été révélées.
     *
     * @return true si le joueur a gagné
     */
    public boolean estVictoire() {
        for (int i = 0; i < nbLignes; i++) {
            for (int j = 0; j < nbColonnes; j++) {
                if (!cases[i][j].isEstMinee() && !cases[i][j].isEstRevelee()) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Compte le nombre de cases portant un marqueur étoile (marqueur = 1).
     *
     * @return le nombre de marqueurs étoile posés
     */
    public int getNbMarqueursEtoile() {
        int compte = 0;
        for (int i = 0; i < nbLignes; i++) {
            for (int j = 0; j < nbColonnes; j++) {
                if (cases[i][j].getMarqueur() == 1) {
                    compte++;
                }
            }
        }
        return compte;
    }

    /**
     * Retourne la case située aux coordonnées données.
     *
     * @param ligne   la ligne de la case
     * @param colonne la colonne de la case
     * @return la case correspondante
     */
    public Case getCase(int ligne, int colonne) {
        return cases[ligne][colonne];
    }

    /**
     * Retourne le nombre de lignes de la grille.
     *
     * @return le nombre de lignes
     */
    public int getNbLignes() { return nbLignes; }

    /**
     * Retourne le nombre de colonnes de la grille.
     *
     * @return le nombre de colonnes
     */
    public int getNbColonnes() { return nbColonnes; }

    /**
     * Retourne le nombre total de mines de la grille.
     *
     * @return le nombre de mines
     */
    public int getNbMines() { return nbMines; }
}