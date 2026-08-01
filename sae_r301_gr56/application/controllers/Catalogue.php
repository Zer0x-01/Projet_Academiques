<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Catalogue extends CI_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('Game_model');
        $this->load->helper('url');
    }

    public function index() {
        $data['categories'] = $this->Game_model->get_all_categories();
        $data['genres']     = $this->Game_model->get_all_genres();
        $this->load->view('templates/header', ['title' => 'Catalogue']);
        $this->load->view('catalogue/index', $data);
        $this->load->view('templates/footer');
    }

    public function category($id) {
        $sort = $this->input->get('sort') ?? 'name';
        $data['category'] = $this->Game_model->get_category_by_id($id);
        $data['games']    = $this->Game_model->get_games_by_category($id, $sort);
        $data['sort']     = $sort;
        $this->load->view('templates/header', ['title' => $data['category']->description ?? 'Catégorie']);
        $this->load->view('catalogue/category', $data);
        $this->load->view('templates/footer');
    }

    public function genre($id) {
        $sort = $this->input->get('sort') ?? 'name';
        $data['genre'] = $this->Game_model->get_genre_by_id($id);
        $data['games'] = $this->Game_model->get_games_by_genre($id, $sort);
        $data['sort']  = $sort;
        $this->load->view('templates/header', ['title' => $data['genre']->description ?? 'Genre']);
        $this->load->view('catalogue/genre', $data);
        $this->load->view('templates/footer');
    }

    public function game($id) {
        $data['game']       = $this->Game_model->get_game_by_id($id);
        $data['genres']     = $this->Game_model->get_genres_by_game($id);
        $data['categories'] = $this->Game_model->get_categories_by_game($id);
        $this->load->view('templates/header', ['title' => $data['game']->name ?? 'Jeu']);
        $this->load->view('catalogue/game', $data);
        $this->load->view('templates/footer');
    }

    public function recherche() {
        $q = $this->input->get('q');
        $data['query'] = $q;
        $data['games'] = $q ? $this->Game_model->search_games($q) : [];
        $this->load->view('templates/header', ['title' => 'Recherche']);
        $this->load->view('catalogue/recherche', $data);
        $this->load->view('templates/footer');
    }
    public function poster($id) {
	$poster = $this->Game_model->get_poster($id);

    	if (!$poster) {
        	show_404();
        	return;
    	}

    	header("Content-Type: image/jpeg");
    	echo $poster->jpeg;
    }
    // Retourne 3 suggestions JSON triées par pertinence
    public function suggest() {
        $q = $this->input->get('q');
        if (!$q) { echo json_encode([]); return; }

        $games = $this->Game_model->search_games($q);
        $q_lower = strtolower($q);

        // Scoring
        foreach ($games as $game) {
            $name_lower = strtolower($game->name);
            if ($name_lower === $q_lower)                                          $game->score = 100;
            elseif (strpos($name_lower, $q_lower) === 0)                           $game->score = 75;
            elseif (preg_match('/\b'.preg_quote($q_lower,'/').'\b/', $name_lower)) $game->score = 50;
            else                                                                    $game->score = 25;
        }

        usort($games, fn($a, $b) => $b->score - $a->score);

        // On garde les 3 premiers
        $top3 = array_slice($games, 0, 3);

        // On retourne uniquement id, name, releaseYear
        echo json_encode(array_map(fn($g) => [
            'id'          => $g->id,
            'name'        => $g->name,
            'releaseYear' => $g->releaseYear,
        ], $top3));
    }
}
