import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

/**
 * Fenêtre affichée à la fin d'une partie, indiquant la victoire ou la défaite
 * ainsi que le temps écoulé.
 *
 * @author Etudiant
 * @version 1.0
 */
public class FenetreFinPartie extends JDialog implements ActionListener {

    /** Fenêtre de jeu parente, utilisée pour retourner au menu. */
    private FenetreJeu fenetreJeu;

    /** Bouton permettant de retourner au menu principal. */
    private JButton btnRetour;

    /**
     * Construit et affiche la fenêtre de fin de partie.
     *
     * @param fenetreJeu la fenêtre de jeu parente
     * @param victoire   true si le joueur a gagné, false sinon
     * @param temps      le temps écoulé en secondes
     */
    public FenetreFinPartie(FenetreJeu fenetreJeu, boolean victoire, long temps) {
        super(fenetreJeu, "Fin de partie", true);
        this.fenetreJeu = fenetreJeu;

        setSize(320, 200);
        setLocationRelativeTo(fenetreJeu);
        setResizable(false);
        setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);

        JPanel panneau = new JPanel(new GridLayout(3, 1, 0, 10));
        panneau.setBorder(BorderFactory.createEmptyBorder(25, 30, 25, 30));

        String titreTexte;
        if (victoire) {
            titreTexte = "\u2705 Victoire !";
        } else {
            titreTexte = "\u274C Défaite !";
        }
        JLabel labelTitre = new JLabel(titreTexte, SwingConstants.CENTER);
        labelTitre.setFont(new Font("SansSerif", Font.BOLD, 22));

        String messageTexte;
        if (victoire) {
            messageTexte = "Bravo ! Toutes les cases révélées en " + temps + "s.";
        } else {
            messageTexte = "Vous avez sauté sur une mine ! (" + temps + "s)";
        }
        JLabel labelMessage = new JLabel(messageTexte, SwingConstants.CENTER);
        labelMessage.setFont(new Font("SansSerif", Font.PLAIN, 14));

        btnRetour = new JButton("Retour au menu");
        btnRetour.setFont(new Font("SansSerif", Font.PLAIN, 14));
        btnRetour.addActionListener(this);

        panneau.add(labelTitre);
        panneau.add(labelMessage);
        panneau.add(btnRetour);

        add(panneau);
        setVisible(true);
    }

    /**
     * Gère le clic sur le bouton "Retour au menu".
     *
     * @param e l'événement d'action
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == btnRetour) {
            dispose();
            fenetreJeu.retournerAuMenu();
        }
    }
}