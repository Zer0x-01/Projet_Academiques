<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Game_model extends CI_Model {
    // Chargement de la bd
    public function __construct() {
        parent::__construct();
        $this->load->database();
    }
    // Retourne toutes les catégories
    public function get_all_categories() {
        $this->db->order_by('description');
        return $this->db->get('category')->result();
    }

    // Retourne tous les genres
    public function get_all_genres() {
        $this->db->order_by('description');
        return $this->db->get('genre')->result();
    }
    // Retourne tous les jeux avec tri
    public function get_all_games($sort = 'name') {
        $allowed = ['name', 'releaseYear'];
        if (!in_array($sort, $allowed)) $sort = 'name';
        $this->db->select('game.*, developer.name as developer_name');
        $this->db->from('game');
        $this->db->join('developer', 'developer.id = game.developerId', 'left');
        $this->db->order_by($sort);
        return $this->db->get()->result();
    }

    // Retourne un jeu par son id avec son développeur
    public function get_game_by_id($id) {
        $this->db->select('game.*, developer.name as developer_name');
        $this->db->from('game');
        $this->db->join('developer', 'developer.id = game.developerId', 'left');
        $this->db->where('game.id', $id);
        return $this->db->get()->row();
    }

    // Retourne une catégorie par son id
    public function get_category_by_id($id) {
        return $this->db->get_where('category', ['id' => $id])->row();
    }

    // Retourne un genre par son id
    public function get_genre_by_id($id) {
        return $this->db->get_where('genre', ['id' => $id])->row();
    }

    // Retourne les jeux d'une catégorie
    public function get_games_by_category($category_id, $sort = 'name') {
        $allowed = ['name', 'releaseYear'];
        if (!in_array($sort, $allowed)) $sort = 'name';
        $this->db->select('game.*');
        $this->db->from('game');
        $this->db->join('game_category', 'game_category.gameId = game.id');
        $this->db->where('game_category.categoryId', $category_id);
        $this->db->order_by($sort);
        return $this->db->get()->result();
    }

    // Retourne les jeux d'un genre
    public function get_games_by_genre($genre_id, $sort = 'name') {
        $allowed = ['name', 'releaseYear'];
        if (!in_array($sort, $allowed)) $sort = 'name';
        $this->db->select('game.*');
        $this->db->from('game');
        $this->db->join('game_genre', 'game_genre.gameId = game.id');
        $this->db->where('game_genre.genreId', $genre_id);
        $this->db->order_by($sort);
        return $this->db->get()->result();
    }

    // Retourne les genres d'un jeu
    public function get_genres_by_game($game_id) {
        $this->db->select('genre.*');
        $this->db->from('genre');
        $this->db->join('game_genre', 'game_genre.genreId = genre.id');
        $this->db->where('game_genre.gameId', $game_id);
        return $this->db->get()->result();
    }

    // Retourne les catégories d'un jeu
    public function get_categories_by_game($game_id) {
        $this->db->select('category.*');
        $this->db->from('category');
        $this->db->join('game_category', 'game_category.categoryId = category.id');
        $this->db->where('game_category.gameId', $game_id);
        return $this->db->get()->result();
    }

    // Recherche textuelle
    public function search_games($q) {
        $this->db->select('game.*, developer.name as developer_name');
        $this->db->from('game');
        $this->db->join('developer', 'developer.id = game.developerId', 'left');
        $this->db->like('game.name', $q);
        $this->db->order_by('game.name');
        return $this->db->get()->result();
    }

    // Retourne la liste de tous les développeurs (utile pour la liste déroulante)
    public function get_all_developers() {
        $this->db->order_by('name');
        return $this->db->get('developer')->result();
    }

    // Insérer un nouveau jeu
    public function insert_game($data) {
        $this->db->insert('game', $data);
        return $this->db->insert_id(); // Retourne l'ID du jeu fraîchement créé
    }

    // Mettre à jour un jeu existant
    public function update_game($id, $data) {
        $this->db->where('id', $id);
        return $this->db->update('game', $data);
    }

    // Supprimer un jeu (suppression cascade requise en BDD pour les associations)
    public function delete_game($id) {
        $this->db->where('id', $id);
        return $this->db->delete('game');
    }

    public function get_poster($id) {
	    return $this->db->get_where('poster', ['id' => $id])->row();
    }

    // Remplace toutes les catégories d'un jeu
    public function sync_categories($game_id, $category_ids) {
        $this->db->where('gameId', $game_id);
        $this->db->delete('game_category');
        foreach ((array)$category_ids as $cat_id) {
            $this->db->insert('game_category', ['gameId' => $game_id, 'categoryId' => $cat_id]);
        }
    }

    // Remplace tous les genres d'un jeu
    public function sync_genres($game_id, $genre_ids) {
        $this->db->where('gameId', $game_id);
        $this->db->delete('game_genre');
        foreach ((array)$genre_ids as $genre_id) {
            $this->db->insert('game_genre', ['gameId' => $game_id, 'genreId' => $genre_id]);
        }
    
    }
    
    public function insert_poster($jpeg_data) {
        $this->db->insert('poster', ['jpeg' => $jpeg_data]);
        return $this->db->insert_id();
    }

}

