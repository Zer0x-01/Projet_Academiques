Séance 1 
Objectif de la séance :

Mettre en place l’environnement de travail pour le projet SAE : Git, CodeIgniter et base de données.

Travail réalisé :

- Création du dépôt Git et de la branche de la séance.
- Configuration de PHPMyAdmin et import de la base de données de jeux vidéo.
- Vérification que CodeIgniter est correctement relié à la base via un contrôleur Test.php qui affiche le nombre de jeux dans la base de données.

Difficultés rencontrées :

- Comprendre comment tester la connexion à la base avec CodeIgniter.

Décisions prises :

- Utiliser un mini-contrôleur Test.php pour vérifier l’affichage des premières données.
- Organiser les dossiers pour les futurs contrôleurs, modèles et vues.

Travail de chacun dans le binôme :

Membre 1 : configuration Git et création de la branche, tests sur CodeIgniter.

Membre 2 : configuration de PHPMyAdmin et import de la base, rédaction des fichiers de configuration.

Points à reprendre à la séance suivante :

- Commencer à mettre en place les vues HTML pour la consultation des jeux.

Usage de l’IA :

- On a demandé de l’aide à l’IA pour rédiger le code de test de connexion à la base avec CodeIgniter (Test.php).


Séance 2
Objectif de la séance :

Mettre en place le modèle Game_model et tester les requêtes SQL sur la base de données.

Travail réalisé :

- Tests des requêtes SQL dans Test.php.
- Création du modèle Game_model avec toutes les méthodes de requêtes.
- Création du layout partagé header.php et footer.php en dark mode CSS pur.
- Mise en place du .gitignore pour ne versionner que les fichiers créés ou modifiés par nous.
- Création du contrôleur Catalogue avec les actions index, category, genre et game.

Difficultés rencontrées :

- Difficulté à intégrer les requêtes SQL dans le modèle avec la syntaxe CodeIgniter (join, where, order_by).
- Difficulté à comprendre la structure de la base de données, notamment les tables de liaison game_category et game_genre.
- Difficulté à synchroniser le serveur avec la branche b2 après le déplacement des fichiers à la racine.

Décisions prises :

- Toutes les requêtes SQL sont centralisées dans le modèle et jamais dans le contrôleur, ce qui facilite la maintenance et évite la duplication de code (principe MVC).
- Nettoyage du repo pour ne garder que les fichiers qu'on a créés ou modifiés.

Travail de chacun dans le binôme :

Membre 1 : layout header.php et footer.php, création du controlleur Catalogue.

Membre 2 : tests SQL dans Test.php, création du Game_model, mise en place du .gitignore..

Points à reprendre à la séance suivante :

- Créer les vues : index, category, genre, game.
- Ajouter la recherche textuelle.

Usage de l'IA :

- On a utilisé l'IA pour nous aider avec le code du Game_model.php et Catalogue.php.
- On a utilisé l'IA pour générer le layout header.php.
- On a utilisé l'IA pour diagnostiquer les erreurs rencontrées et nous expliquer les solutions.

Séance 3
Objectif de la séance :

Créer les vues de consultation du catalogue, mettre en place la recherche textuelle et développer le CRUD des jeux.

Travail réalisé :

- Création des vues de consultation : category.php, genre.php, game.php, recherche.php.
- Ajout de la méthode search_games() dans Game_model pour la recherche textuelle.
- Ajout de l'action recherche() dans Catalogue.php.
- Ajout des méthodes CRUD dans Game_model : insert_game(), update_game(), delete_game(), get_all_developers().
- Création du contrôleur Jeu.php avec les actions creer(), editer(), supprimer().
- Création des vues jeu/form.php (formulaire partagé création/édition) et jeu/supprimer.php.
- Modification de game.php pour afficher le poster du jeu et ajouts des champ de description, platforme, coût des jeux.
- Ajout de l'action poster() dans Catalogue.php pour servir les images des posters depuis la base de données.

Difficultés rencontrées :

- Difficulté à partager le même formulaire pour la création et l'édition sans dupliquer le code.
- Difficulté à afficher correctement les posters (récupération depuis la BD et mise en page CSS).
- Difficulté à gérer le pré-remplissage du formulaire d'édition avec les données existantes.

Décisions prises :

- Un seul formulaire form.php détecte le mode création ou édition via la variable $is_edit.
- La suppression passe par une page de confirmation pour éviter les suppressions accidentelles.
- La modification du poster est volontairement désactivée conformément aux consignes.

Travail de chacun dans le binôme :

Membre 1 : vues catalogue (index, category, genre, game, recherche), méthodes CRUD dans Game_model.

Membre 2 : contrôleur Jeu.php, vues jeu/form.php et jeu/supprimer.php, méthodes CRUD dans Game_model, affichages des posters, mise à jour de la vue game.

Points à reprendre à la séance suivante :

- Ajouter le tri alphabétique sur les catégories et genres de la page index.
- Ajouter un bouton dark/light mode avec mémorisation en JavaScript.
- Ajouter un système de scoring pour la barre de recherche.

Usage de l'IA :

Les fichiers suivants ont été envoyés à l'IA comme contexte : Game_model.php, Catalogue.php, game.php, header.php, le schéma de la base de données et l'énoncé de la SAE afin qu'elle posséde tout le contexte.

Prompts utilisés :

Prompt 1 — contrôleur Jeu.php :
"Je développe une application web en PHP avec CodeIgniter 3. J'ai déjà un modèle Game_model avec les méthodes insert_game(), update_game(), delete_game() et get_all_developers(). La base de données contient une table game avec les colonnes : id, name, releaseYear, shortDescription, price, windows, linux, mac, metacritic, developerId, posterId. Je veux créer un contrôleur Jeu.php avec trois actions : creer() pour afficher et traiter un formulaire de création, editer($id) pour afficher et traiter un formulaire de modification (sans toucher au poster), et supprimer($id) pour afficher une confirmation puis supprimer le jeu. Le contrôleur doit valider les données côté serveur (nom obligatoire, année entre 1970 et 2100, développeur obligatoire) et rediriger vers la page du jeu après création ou modification."

Prompt 2 — vue jeu/form.php :
"Crée une vue PHP CodeIgniter pour un formulaire partagé création/édition d'un jeu vidéo en dark mode. Le formulaire doit détecter automatiquement si on est en mode création ou édition grâce à une variable $is_edit. Les champs sont : nom, année de sortie, développeur (liste déroulante), description courte, prix, score metacritic, plateformes (checkboxes windows/linux/mac). En mode édition les champs doivent être pré-remplis avec les données existantes. En mode création afficher une note indiquant que le poster ne peut pas être ajouté. Utiliser les classes CSS déjà définies dans le header : form-box, form-group, btn, btn-primary, btn-secondary."

Prompt 3 — vue jeu/supprimer.php :
"Crée une vue PHP CodeIgniter pour une page de confirmation de suppression d'un jeu en dark mode. La page doit afficher le nom du jeu et demander confirmation avant suppression. Utiliser un formulaire POST avec un champ hidden confirm=1. Ajouter un bouton annuler qui redirige vers la page du jeu. Utiliser les classes CSS déjà définies : form-box, btn-danger, btn-secondary."

Ce qui a été vérifié :
- Testé manuellement la création, l'édition et la suppression d'un jeu.
- Vérifié que le poster n'est pas modifiable en édition.


Séance 4 :
Objectif de la séance :

Finaliser la sae en ajoutant le tri alphabétique des catégories et genres, l'intégration des catégories/genres dans le formulaire de jeu, et la barre de recherche depuis la page d'accueil.
Travail réalisé :

- Tri alphabétique des catégories et genres affichés sur la page d'accueil (index).
- Ajout de listes déroulantes pour les catégories et genres dans le formulaire de création/modification de jeu
  avec affichage des étiquettes sélectionnées juste au-dessus de la liste pour un retour visuel immédiat.
- Ajout de suggestion en fonction de la pertinence de la recherche.
- Mise en page CSS des nouveaux éléments pour un design propre et cohérent avec le reste de l'application.
- Nombreux tests manuels de la création et de la modification de jeu pour s'assurer que l'ensemble des fonctionnalités est stable et terminé.

Difficultés rencontrées :

- Gérer la sélection multiple pour les catégories et genres dans le formulaire tout en affichant les étiquettes de manière dynamique.
- S'assurer que les catégories et genres existants sont bien pré-cochés en mode édition.

Décisions prises :

- Les étiquettes des catégories/genres sélectionnés s'affichent directement au-dessus de la liste déroulante pour rendre la sélection plus lisible sans surcharger le formulaire.
- La barre de recherche réutilise la méthode search_games() déjà existante dans Game_model.

Travail de chacun dans le binôme :

Membre 1 : ajout des listes déroulantes catégories/genres dans le formulaire, gestion des étiquettes, tests de création et modification.

Membre 2 : tri alphabétique sur la page d'accueil, intégration de la barre de recherche, ajout de la possibilité d'ajouter un poster si on crée un jeu ou bien si un jeu pre éxistant n'en possédait pas.

Points à reprendre à la séance suivante :

On a tout finis.

Usage de l'IA :
Les fichiers suivants ont été fournis à l'IA comme contexte : Game_model.php, Jeu.php, Catalogue.php, jeu/form.php, header.php, le schéma de la base de données.
Prompts utilisés :
Prompt 1 — listes déroulantes avec étiquettes :
"Dans mon formulaire CodeIgniter de création/modification de jeu, je veux que les champs catégories et genres fonctionnent comme le champ développeur : une liste déroulante à sélection multiple où chaque élément sélectionné s'ajoute sous forme d'étiquette dans une zone dédiée juste au-dessus du menu déroulant. Chaque étiquette doit afficher le nom de l'élément avec une croix permettant de le retirer. On doit pouvoir en ajouter autant qu'on veut. Le comportement doit être cohérent entre les deux listes."

Prompt 2 — CSS des étiquettes et des listes :
"Génère le CSS pour les étiquettes de sélection (tags) affichées au-dessus des listes déroulantes de catégories et genres dans mon formulaire dark mode. Le style doit être propre, cohérent avec le reste de l'application, avec un badge arrondi et une croix pour désélectionner."

Ce qui a été vérifié :

- Testé manuellement la création et la modification de plusieurs jeux avec des catégories et genres variés.
- Vérifié que le tri alphabétique s'applique correctement sur toutes les listes de la page d'accueil.
- Vérifié que la barre de recherche retourne des résultats cohérents et redirige correctement.
- Vérifié qu'aucune régression n'est apparue sur les fonctionnalités des séances précédentes.