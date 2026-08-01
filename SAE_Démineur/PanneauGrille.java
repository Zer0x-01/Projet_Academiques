import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

/**
 * Panneau graphique qui dessine la grille de jeu et gère les clics souris.
 * Chaque case est représentée par un carré de taille fixe.
 *
 * @author Etudiant
 * @version 1.0
 */
public class PanneauGrille extends JPanel implements MouseListener {

    /** Taille en pixels d'une case de la grille. */
    private static final int TAILLE_CASE = 40;

    /** La grille logique du jeu. */
    private Grille grille;

    /** Référence à la fenêtre de jeu pour signaler les fins de partie. */
    private FenetreJeu fenetreJeu;

    /** Indique si la partie est terminée (plus aucun clic accepté). */
    private boolean partieTerminee;

    /** Ligne de la mine sur laquelle le joueur a cliqué (explosion). */
    private int ligneExplosion;

    /** Colonne de la mine sur laquelle le joueur a cliqué (explosion). */
    private int colonneExplosion;

    /**
     * Construit le panneau graphique associé à la grille donnée.
     *
     * @param grille     la grille logique à afficher
     * @param fenetreJeu la fenêtre de jeu parente
     */
    public PanneauGrille(Grille grille, FenetreJeu fenetreJeu) {
        this.grille      = grille;
        this.fenetreJeu  = fenetreJeu;
        this.partieTerminee  = false;
        this.ligneExplosion  = -1;
        this.colonneExplosion = -1;

        int largeur = grille.getNbColonnes() * TAILLE_CASE;
        int hauteur = grille.getNbLignes()   * TAILLE_CASE;
        setPreferredSize(new Dimension(largeur, hauteur));
        addMouseListener(this);
    }

    /**
     * Dessine l'ensemble des cases de la grille.
     *
     * @param g le contexte graphique
     */
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        for (int i = 0; i < grille.getNbLignes(); i++) {
            for (int j = 0; j < grille.getNbColonnes(); j++) {
                dessinerCase(g, i, j);
            }
        }
    }

    /**
     * Dessine une case individuelle selon son état (révélée, minée, marquée, etc.).
     *
     * @param g    le contexte graphique
     * @param ligne la ligne de la case
     * @param col   la colonne de la case
     */
    private void dessinerCase(Graphics g, int ligne, int col) {
        int x = col  * TAILLE_CASE;
        int y = ligne * TAILLE_CASE;
        Case c = grille.getCase(ligne, col);

        // Couleur de fond selon l'état de la case
        if (c.isEstRevelee()) {
            if (c.isEstMinee()) {
                if (ligne == ligneExplosion && col == colonneExplosion) {
                    g.setColor(new Color(220, 50, 50));   // mine qui a explosé
                } else {
                    g.setColor(new Color(255, 165, 60));  // autre mine révélée
                }
            } else {
                g.setColor(new Color(220, 220, 220));
            }
        } else {
            if (partieTerminee && c.isEstMinee() && c.getMarqueur() != 1) {
                g.setColor(new Color(255, 200, 100));     // mine non signalée
            } else if (partieTerminee && !c.isEstMinee() && c.getMarqueur() == 1) {
                g.setColor(new Color(255, 180, 180));     // faux marqueur
            } else {
                g.setColor(new Color(190, 190, 190));
            }
        }
        g.fillRect(x, y, TAILLE_CASE, TAILLE_CASE);

        // Effet 3D pour les cases non révélées
        if (!c.isEstRevelee() && !partieTerminee) {
            g.setColor(Color.WHITE);
            g.drawLine(x, y, x + TAILLE_CASE - 1, y);
            g.drawLine(x, y, x, y + TAILLE_CASE - 1);
            g.setColor(new Color(100, 100, 100));
            g.drawLine(x + TAILLE_CASE - 1, y, x + TAILLE_CASE - 1, y + TAILLE_CASE - 1);
            g.drawLine(x, y + TAILLE_CASE - 1, x + TAILLE_CASE - 1, y + TAILLE_CASE - 1);
        } else {
            g.setColor(new Color(150, 150, 150));
            g.drawRect(x, y, TAILLE_CASE - 1, TAILLE_CASE - 1);
        }

        g.setFont(new Font("SansSerif", Font.BOLD, 16));
        FontMetrics fm = g.getFontMetrics();

        if (c.isEstRevelee()) {
            if (c.isEstMinee()) {
                g.setColor(Color.BLACK);
                g.fillOval(x + 10, y + 10, TAILLE_CASE - 20, TAILLE_CASE - 20);
                g.setColor(new Color(80, 80, 80));
                g.fillOval(x + 14, y + 14, 6, 6);
            } else if (c.getMinesAdjacentes() > 0) {
                String s = String.valueOf(c.getMinesAdjacentes());
                g.setColor(getCouleurChiffre(c.getMinesAdjacentes()));
                int sx = x + (TAILLE_CASE - fm.stringWidth(s)) / 2;
                int sy = y + (TAILLE_CASE + fm.getAscent() - fm.getDescent()) / 2;
                g.drawString(s, sx, sy);
            }
        } else {
            if (c.getMarqueur() == 1) {
                g.setColor(new Color(200, 0, 0));
                String s = "\u2605"; // ★
                int sx = x + (TAILLE_CASE - fm.stringWidth(s)) / 2;
                int sy = y + (TAILLE_CASE + fm.getAscent() - fm.getDescent()) / 2;
                g.drawString(s, sx, sy);
            } else if (c.getMarqueur() == 2) {
                g.setColor(new Color(0, 100, 200));
                String s = "?";
                int sx = x + (TAILLE_CASE - fm.stringWidth(s)) / 2;
                int sy = y + (TAILLE_CASE + fm.getAscent() - fm.getDescent()) / 2;
                g.drawString(s, sx, sy);
            }
            // En fin de partie, on révèle les mines non marquées
            if (partieTerminee && c.isEstMinee() && c.getMarqueur() != 1) {
                g.setColor(Color.BLACK);
                g.fillOval(x + 10, y + 10, TAILLE_CASE - 20, TAILLE_CASE - 20);
            }
        } if (partieTerminee && !c.isEstMinee() && c.getMarqueur() == 1) {
            // Croix rouge sur les faux marqueurs
            g.setColor(Color.RED);
            g.drawLine(x + 8, y + 8, x + TAILLE_CASE - 8, y + TAILLE_CASE - 8);
            g.drawLine(x + 9, y + 8, x + TAILLE_CASE - 7, y + TAILLE_CASE - 8);
            g.drawLine(x + 8, y + 9, x + TAILLE_CASE - 8, y + TAILLE_CASE - 7);
            g.drawLine(x + TAILLE_CASE - 8, y + 8, x + 8, y + TAILLE_CASE - 8);
            g.drawLine(x + TAILLE_CASE - 9, y + 8, x + 7, y + TAILLE_CASE - 8);
            g.drawLine(x + TAILLE_CASE - 8, y + 9, x + 8, y + TAILLE_CASE - 7);   
        }         
        
    }

    /**
     * Retourne la couleur associée au chiffre affiché sur une case révélée,
     * selon les conventions classiques du Démineur.
     *
     * @param n le nombre de mines adjacentes (1 à 8)
     * @return la couleur correspondante
     */
    private Color getCouleurChiffre(int n) {
        switch (n) {
            case 1: return new Color(0,   0,   200);
            case 2: return new Color(0,   130, 0);
            case 3: return new Color(200, 0,   0);
            case 4: return new Color(0,   0,   128);
            case 5: return new Color(128, 0,   0);
            case 6: return new Color(0,   128, 128);
            case 7: return new Color(50,  50,  50);
            case 8: return new Color(120, 120, 120);
            default: return Color.BLACK;
        }
    }

    /**
     * Gère les clics souris sur la grille.
     * Un clic gauche révèle la case, un clic droit fait cycler son marqueur.
     *
     * @param e l'événement souris
     */
    public void mouseClicked(MouseEvent e) {
        if (partieTerminee) {
            return;
        }

        int col   = e.getX() / TAILLE_CASE;
        int ligne = e.getY() / TAILLE_CASE;

        if (ligne < 0 || ligne >= grille.getNbLignes()
                || col < 0 || col >= grille.getNbColonnes()) {
            return;
        }

        if (SwingUtilities.isRightMouseButton(e)) {
            grille.cliquerDroit(ligne, col);
            repaint();
            fenetreJeu.mettreAJourCompteur();

        } else if (SwingUtilities.isLeftMouseButton(e)) {
            int resultat = grille.cliquerGauche(ligne, col);
            repaint();
            fenetreJeu.mettreAJourCompteur();

            if (resultat == -1) {
                ligneExplosion  = ligne;
                colonneExplosion = col;
                partieTerminee  = true;
                repaint();
                fenetreJeu.finDePartie(false);
            } else if (resultat == 1) {
                partieTerminee = true;
                repaint();
                fenetreJeu.finDePartie(true);
            }
        }
    }

    public void mousePressed(MouseEvent e) { }
    public void mouseReleased(MouseEvent e) { }
    public void mouseEntered(MouseEvent e) { }
    public void mouseExited(MouseEvent e) { }
}