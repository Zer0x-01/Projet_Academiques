import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.Timer;

/**
 * Fenêtre principale du jeu affichant la grille, le chronomètre
 * et le compteur de mines restantes.
 *
 * @author Etudiant
 * @version 1.0
 */
public class FenetreJeu extends JFrame implements ActionListener, WindowListener {

    /** La partie en cours. */
    private Partie partie;

    /** Référence au menu principal pour y retourner. */
    private FenetreMenu menu;

    /** Panneau graphique représentant la grille. */
    private PanneauGrille panneauGrille;

    /** Label affichant le nombre de mines restantes. */
    private JLabel labelMines;

    /** Label affichant le temps écoulé. */
    private JLabel labelTemps;

    /** Bouton pour sauvegarder et quitter la partie. */
    private JButton btnSauverQuitter;

    /** Minuteur qui s'incrémente chaque seconde. */
    private Timer minuteur;

    /**
     * Construit et affiche la fenêtre de jeu pour la partie donnée.
     *
     * @param partie la partie à jouer
     * @param menu   la fenêtre du menu principal
     */
    public FenetreJeu(Partie partie, FenetreMenu menu) {
        super("Démineur");
        this.partie = partie;
        this.menu   = menu;

        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        addWindowListener(this);
        setLayout(new BorderLayout(0, 0));

        // Barre du haut avec le compteur, le chrono et le bouton sauvegarde
        JPanel barreHaut = new JPanel(new FlowLayout(FlowLayout.CENTER, 30, 6));
        barreHaut.setBorder(BorderFactory.createEtchedBorder());

        int restantes = partie.getGrille().getNbMines()
                      - partie.getGrille().getNbMarqueursEtoile();
        labelMines = new JLabel("Mines : " + restantes);
        labelMines.setFont(new Font("SansSerif", Font.BOLD, 15));

        labelTemps = new JLabel("Temps : " + partie.getTempsEcoule() + "s");
        labelTemps.setFont(new Font("SansSerif", Font.BOLD, 15));

        btnSauverQuitter = new JButton("Sauver et Quitter");
        btnSauverQuitter.setFont(new Font("SansSerif", Font.PLAIN, 13));
        btnSauverQuitter.addActionListener(this);

        barreHaut.add(labelMines);
        barreHaut.add(labelTemps);
        barreHaut.add(btnSauverQuitter);
        add(barreHaut, BorderLayout.NORTH);

        panneauGrille = new PanneauGrille(partie.getGrille(), this);
        JScrollPane scroll = new JScrollPane(panneauGrille);
        add(scroll, BorderLayout.CENTER);

        // Le minuteur s'incrémente chaque seconde
        minuteur = new Timer(1000, this);
        minuteur.start();

        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    /**
     * Gère les événements du minuteur et du bouton de sauvegarde.
     *
     * @param e l'événement d'action
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == minuteur) {
            partie.incrementerTemps();
            labelTemps.setText("Temps : " + partie.getTempsEcoule() + "s");
        } else if (e.getSource() == btnSauverQuitter) {
            sauverEtQuitter();
        }
    }

    /**
     * Met à jour le compteur de mines restantes.
     * Appelée depuis le panneau de grille après un clic droit.
     */
    public void mettreAJourCompteur() {
        int restantes = partie.getGrille().getNbMines()
                      - partie.getGrille().getNbMarqueursEtoile();
        labelMines.setText("Mines : " + restantes);
    }

    /**
     * Termine la partie en arrêtant le minuteur et en ouvrant la fenêtre de fin.
     *
     * @param victoire true si le joueur a gagné, false sinon
     */
    public void finDePartie(boolean victoire) {
        minuteur.stop();
        btnSauverQuitter.setEnabled(false);
        new FenetreFinPartie(this, victoire, partie.getTempsEcoule());
    }

    /**
     * Sauvegarde la partie en cours puis retourne au menu principal.
     */
    private void sauverEtQuitter() {
        minuteur.stop();
        try {
            Sauvegarde.sauvegarder(partie);
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(
                this,
                "Erreur lors de la sauvegarde.",
                "Erreur",
                JOptionPane.ERROR_MESSAGE
            );
        }
        dispose();
        menu.afficher();
    }

    /**
     * Ferme la fenêtre de jeu et retourne au menu principal.
     */
    public void retournerAuMenu() {
        dispose();
        menu.afficher();
    }

    /**
     * Intercepte la fermeture de la fenêtre pour sauvegarder automatiquement.
     *
     * @param e l'événement de fenêtre
     */
    public void windowClosing(WindowEvent e) {
        sauverEtQuitter();
    }

    public void windowOpened(WindowEvent e) { }
    public void windowClosed(WindowEvent e) { }
    public void windowIconified(WindowEvent e) { }
    public void windowDeiconified(WindowEvent e) { }
    public void windowActivated(WindowEvent e) { }
    public void windowDeactivated(WindowEvent e) { }
}