import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

// Menu principal du jeu
public class FenetreMenu extends JFrame implements ActionListener {

    private JButton btnNouvellePartie;
    private JButton btnReprendre;
    private JButton btnQuitter;

    public FenetreMenu() {
        super("Démineur");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(340, 280);
        setLocationRelativeTo(null);
        setResizable(false);

        JPanel panneauPrincipal = new JPanel();
        panneauPrincipal.setBorder(BorderFactory.createEmptyBorder(20, 40, 20, 40));
        panneauPrincipal.setLayout(new GridLayout(4, 1, 0, 14));

        JLabel titre = new JLabel("💣 Démineur", SwingConstants.CENTER);
        titre.setFont(new Font("SansSerif", Font.BOLD, 26));

        btnNouvellePartie = new JButton("Nouvelle partie");
        btnReprendre      = new JButton("Reprendre la partie");
        btnQuitter        = new JButton("Quitter");

        btnNouvellePartie.setFont(new Font("SansSerif", Font.PLAIN, 15));
        btnReprendre.setFont(new Font("SansSerif", Font.PLAIN, 15));
        btnQuitter.setFont(new Font("SansSerif", Font.PLAIN, 15));

        btnNouvellePartie.setMargin(new Insets(8, 8, 8, 8));
        btnReprendre.setMargin(new Insets(8, 8, 8, 8));
        btnQuitter.setMargin(new Insets(8, 8, 8, 8));

        btnNouvellePartie.addActionListener(this);
        btnReprendre.addActionListener(this);
        btnQuitter.addActionListener(this);

        // Bouton "Reprendre" désactivé s'il n'y a pas de sauvegarde
        btnReprendre.setEnabled(Sauvegarde.existeSauvegarde());

        panneauPrincipal.add(titre);
        panneauPrincipal.add(btnNouvellePartie);
        panneauPrincipal.add(btnReprendre);
        panneauPrincipal.add(btnQuitter);

        add(panneauPrincipal);
        setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == btnNouvellePartie) {
            new FenetreConfig(this);
        } else if (source == btnReprendre) {
            reprendrePartie();
        } else if (source == btnQuitter) {
            System.exit(0);
        }
    }

    private void reprendrePartie() {
        try {
            Partie partie = Sauvegarde.charger();
            Sauvegarde.supprimerSauvegarde();
            setVisible(false);
            new FenetreJeu(partie, this);
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(
                this,
                "Impossible de charger la sauvegarde.",
                "Erreur",
                JOptionPane.ERROR_MESSAGE
            );
            btnReprendre.setEnabled(false);
        }
    }

    // Appelée au retour d'une partie pour remettre à jour le bouton reprendre
    public void afficher() {
        btnReprendre.setEnabled(Sauvegarde.existeSauvegarde());
        setVisible(true);
    }
}