import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/**
 * Gère la sauvegarde et le chargement d'une partie via la sérialisation Java.
 * Les données sont stockées dans un fichier binaire local.
 *
 * @author Etudiant
 * @version 1.0
 */
public class Sauvegarde {

    /** Nom du fichier utilisé pour stocker la sauvegarde. */
    private static final String FICHIER_SAUVEGARDE = "sauvegarde.dat";

    /**
     * Sauvegarde la partie donnée dans le fichier de sauvegarde.
     *
     * @param partie la partie à sauvegarder
     * @throws IOException en cas d'erreur d'écriture
     */
    public static void sauvegarder(Partie partie) throws IOException {
        FileOutputStream fos = new FileOutputStream(FICHIER_SAUVEGARDE);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(partie);
        oos.close();
        fos.close();
    }

    /**
     * Charge et retourne la partie depuis le fichier de sauvegarde.
     *
     * @return la partie chargée
     * @throws IOException            en cas d'erreur de lecture
     * @throws ClassNotFoundException si la classe Partie est introuvable
     */
    public static Partie charger() throws IOException, ClassNotFoundException {
        FileInputStream fis = new FileInputStream(FICHIER_SAUVEGARDE);
        ObjectInputStream ois = new ObjectInputStream(fis);
        Partie partie = (Partie) ois.readObject();
        ois.close();
        fis.close();
        return partie;
    }

    /**
     * Vérifie si un fichier de sauvegarde lisible existe.
     *
     * @return true si une sauvegarde est disponible
     */
    public static boolean existeSauvegarde() {
        File f = new File(FICHIER_SAUVEGARDE);
        return f.exists() && f.isFile() && f.canRead();
    }

    /**
     * Supprime le fichier de sauvegarde s'il existe.
     */
    public static void supprimerSauvegarde() {
        File f = new File(FICHIER_SAUVEGARDE);
        if (f.exists()) {
            f.delete();
        }
    }
}