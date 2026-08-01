import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

/**
 * Fenêtre de configuration affichée avant de démarrer une nouvelle partie.
 * Permet au joueur de choisir le nombre de lignes, de colonnes et de mines.
 *
 * @author Etudiant
 * @version 1.0
 */
public class FenetreConfig extends JDialog implements ActionListener, ChangeListener {

    /** Fenêtre du menu principal, utilisée pour y revenir après configuration. */
    private FenetreMenu parent;

    /** Spinner pour choisir le nombre de lignes de la grille. */
    private JSpinner spinnerLignes;

    /** Spinner pour choisir le nombre de colonnes de la grille. */
    private JSpinner spinnerColonnes;

    /** Spinner pour choisir le nombre de mines placées. */
    private JSpinner spinnerMines;

    /** Bouton pour valider la configuration et lancer la partie. */
    private JButton btnCommencer;

    /** Bouton pour annuler et revenir au menu. */
    private JButton btnAnnuler;

    /**
     * Construit et affiche la fenêtre de configuration.
     *
     * @param parent la fenêtre du menu principal
     */
    public FenetreConfig(FenetreMenu parent) {
        super(parent, "Nouvelle partie", true);
        this.parent = parent;

        setSize(320, 230);
        setLocationRelativeTo(parent);
        setResizable(false);

        setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 10, 8, 10);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        Font labelFont = new Font("SansSerif", Font.PLAIN, 14);

        JLabel lblLignes = new JLabel("Nombre de lignes :");
        lblLignes.setFont(labelFont);
        spinnerLignes = new JSpinner(new SpinnerNumberModel(9, 4, 30, 1));
        spinnerLignes.addChangeListener(this);

        gbc.gridx = 0; gbc.gridy = 0;
        add(lblLignes, gbc);
        gbc.gridx = 1;
        add(spinnerLignes, gbc);

        JLabel lblColonnes = new JLabel("Nombre de colonnes :");
        lblColonnes.setFont(labelFont);
        spinnerColonnes = new JSpinner(new SpinnerNumberModel(9, 4, 30, 1));
        spinnerColonnes.addChangeListener(this);

        gbc.gridx = 0; gbc.gridy = 1;
        add(lblColonnes, gbc);
        gbc.gridx = 1;
        add(spinnerColonnes, gbc);

        JLabel lblMines = new JLabel("Nombre de mines :");
        lblMines.setFont(labelFont);
        int maxMines = calculerMaxMines();
        spinnerMines = new JSpinner(new SpinnerNumberModel(10, 1, maxMines, 1));

        gbc.gridx = 0; gbc.gridy = 2;
        add(lblMines, gbc);
        gbc.gridx = 1;
        add(spinnerMines, gbc);

        btnCommencer = new JButton("Commencer");
        btnAnnuler   = new JButton("Annuler");
        btnCommencer.addActionListener(this);
        btnAnnuler.addActionListener(this);

        gbc.gridx = 0; gbc.gridy = 3;
        add(btnAnnuler, gbc);
        gbc.gridx = 1;
        add(btnCommencer, gbc);

        setVisible(true);
    }

    /**
     * Calcule le nombre maximum de mines autorisées, soit le nombre de cases moins une.
     *
     * @return le nombre maximum de mines
     */
    private int calculerMaxMines() {
        int lignes   = (Integer) spinnerLignes.getValue();
        int colonnes = (Integer) spinnerColonnes.getValue();
        return lignes * colonnes - 1;
    }

    /**
     * Met à jour le maximum du spinner de mines lorsque les dimensions changent.
     *
     * @param e l'événement de changement
     */
    public void stateChanged(ChangeEvent e) {
        int max = calculerMaxMines();
        SpinnerNumberModel model = (SpinnerNumberModel) spinnerMines.getModel();
        model.setMaximum(max);
        if ((Integer) spinnerMines.getValue() > max) {
            spinnerMines.setValue(max);
        }
    }

    /**
     * Gère les clics sur les boutons "Commencer" et "Annuler".
     *
     * @param e l'événement d'action
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == btnCommencer) {
            int lignes   = (Integer) spinnerLignes.getValue();
            int colonnes = (Integer) spinnerColonnes.getValue();
            int mines    = (Integer) spinnerMines.getValue();

            Partie partie = new Partie(lignes, colonnes, mines);
            dispose();
            parent.setVisible(false);
            new FenetreJeu(partie, parent);
        } else if (e.getSource() == btnAnnuler) {
            dispose();
        }
    }
}