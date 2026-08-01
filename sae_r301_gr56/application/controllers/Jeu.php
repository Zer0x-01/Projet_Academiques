<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Jeu extends CI_Controller {

    public function __construct() {
        parent::__construct();
        // Chargement du modèle et des helpers nécessaires
        $this->load->model('Game_model');
        $this->load->helper(['url', 'form']);
        $this->load->library('form_validation');
    }

    // L'action appelée lors du clic sur le bouton "Modifier"
    public function editer($id) {
        $data['game'] = $this->Game_model->get_game_by_id($id);

        // Si le jeu n'existe pas dans la BDD, on affiche une 404
        if (!$data['game']) {
            show_404();
        }

        $data['developers']      = $this->Game_model->get_all_developers();
        $data['all_categories']  = $this->Game_model->get_all_categories();
        $data['all_genres']      = $this->Game_model->get_all_genres();
        // IDs des catégories/genres déjà associés au jeu (pour pré-cocher les cases)
        $data['game_categories'] = array_column($this->Game_model->get_categories_by_game($id), 'id');
        $data['game_genres']     = array_column($this->Game_model->get_genres_by_game($id), 'id');
        $data['errors']          = [];

        // Définition des règles de validation du formulaire
        $this->form_validation->set_rules('name', 'Nom du jeu', 'required');
        $this->form_validation->set_rules('releaseYear', 'Année de sortie', 'required|numeric|exact_length[4]');
        $this->form_validation->set_rules('developerId', 'Développeur', 'required');

        if ($this->form_validation->run() === FALSE) {
            // Si le formulaire n'est pas soumis ou valide, on affiche la vue du formulaire
            $this->load->view('templates/header', ['title' => 'Modifier un jeu']);
            $this->load->view('jeu/form', $data);
            $this->load->view('templates/footer');
        } else {
            // Si le formulaire est valide, on prépare les données reçues en POST
            $update_data = [
                'name'             => $this->input->post('name'),
                'releaseYear'      => $this->input->post('releaseYear'),
                'developerId'      => $this->input->post('developerId'),
                'shortDescription' => $this->input->post('shortDescription'),
                'price'            => $this->input->post('price') ?: 0,
                'metacritic'       => $this->input->post('metacritic') ?: null,
                // Les hidden fields envoient 0 si décoché, 1 si coché
                'windows'          => (int)$this->input->post('windows'),
                'linux'            => (int)$this->input->post('linux'),
                'mac'              => (int)$this->input->post('mac'),
            ];

            // On met à jour en BDD
            $file = $_FILES['poster'] ?? null;
            if ($file && $file['error'] === UPLOAD_ERR_OK) {
                $image_data = file_get_contents($file['tmp_name']);
                $poster_id  = $this->Game_model->insert_poster($image_data);
                $update_data['posterId'] = $poster_id;
            }
            $this->Game_model->update_game($id, $update_data);
            // On synchronise les catégories et genres
            $this->Game_model->sync_categories($id, $this->input->post('categories') ?: []);
            $this->Game_model->sync_genres($id, $this->input->post('genres') ?: []);

            redirect('catalogue/game/' . $id);
        }
    }

    // Action pour ajouter un nouveau jeu
    public function creer() {
        $data['game']            = null; // Pas de jeu pré-rempli (création)
        $data['developers']      = $this->Game_model->get_all_developers();
        $data['all_categories']  = $this->Game_model->get_all_categories();
        $data['all_genres']      = $this->Game_model->get_all_genres();
        // Aucune case pré-cochée à la création
        $data['game_categories'] = [];
        $data['game_genres']     = [];
        $data['errors']          = [];

        $this->form_validation->set_rules('name', 'Nom du jeu', 'required');
        $this->form_validation->set_rules('releaseYear', 'Année de sortie', 'required|numeric|exact_length[4]');
        $this->form_validation->set_rules('developerId', 'Développeur', 'required');

        if ($this->form_validation->run() === FALSE) {
            $this->load->view('templates/header', ['title' => 'Ajouter un jeu']);
            $this->load->view('jeu/form', $data);
            $this->load->view('templates/footer');
        } else {
            $insert_data = [
                'name'             => $this->input->post('name'),
                'releaseYear'      => $this->input->post('releaseYear'),
                'developerId'      => $this->input->post('developerId'),
                'shortDescription' => $this->input->post('shortDescription'),
                'price'            => $this->input->post('price') ?: 0,
                'metacritic'       => $this->input->post('metacritic') ?: null,
                // Les hidden fields envoient 0 si décoché, 1 si coché
                'windows'          => (int)$this->input->post('windows'),
                'linux'            => (int)$this->input->post('linux'),
                'mac'              => (int)$this->input->post('mac'),
                'posterId'         => null,
            ];

            // Traitement du poster si une image est uploadée
            $file = $_FILES['poster'] ?? null;
            if ($file && $file['error'] === UPLOAD_ERR_OK) {
                $image_data = file_get_contents($file['tmp_name']);
                $poster_id  = $this->Game_model->insert_poster($image_data);
                $insert_data['posterId'] = $poster_id;
            }

            $new_id = $this->Game_model->insert_game($insert_data);
            // On synchronise les catégories et genres du nouveau jeu
            $this->Game_model->sync_categories($new_id, $this->input->post('categories') ?: []);
            $this->Game_model->sync_genres($new_id, $this->input->post('genres') ?: []);

            redirect('catalogue/game/' . $new_id);
        }
    }

    // Action pour supprimer un jeu
    public function supprimer($id) {
        $data['game'] = $this->Game_model->get_game_by_id($id);

        // Si le jeu n'existe pas dans la BDD, on affiche une 404
        if (!$data['game']) {
            show_404();
        }

        // Si l'utilisateur a confirmé la suppression via le formulaire
        if ($this->input->post('confirm')) {
            $this->Game_model->delete_game($id);
            redirect('catalogue');
        }

        // Sinon on affiche la page de confirmation
        $this->load->view('templates/header', ['title' => 'Supprimer un jeu']);
        $this->load->view('jeu/supprimer', $data);
        $this->load->view('templates/footer');
    }
}