<?php if (!$game): ?>
    <div class="error-box">Jeu introuvable.</div>
<?php else: ?>

<style>
    .game-card {
        display: flex;
        gap: 30px;
        align-items: flex-start;
    }

    .poster-container {
        /* On retire les contraintes de taille et le ratio */
        width: auto; 
        max-width: 300px; /* On garde juste une largeur max pour ne pas exploser le design */
        border: 1px solid #ccc;
        border-radius: 8px;
        background-color: #f0f0f0;
        flex-shrink: 0;
        display: flex;
        align-items: center;
        justify-content: center;
        overflow: hidden;
    }

    .poster-container img {
        /* L'image définit sa propre taille */
        width: 100%;
        height: auto;
        display: block;
        /* Plus besoin de object-fit ici car l'image dicte la forme du cadre */
    }

    table {
        border-collapse: collapse;
        width: 100%;
    }

    th {
        text-align: left;
        padding-right: 20px;
    }
</style>

<h1><?= htmlspecialchars($game->name) ?></h1>

<div class="game-card">
    <div class="poster-container">
        <?php if ($game->posterId): ?>
            <img src="<?= base_url('index.php/catalogue/poster/' . $game->posterId) ?>" 
                 alt="Poster de <?= htmlspecialchars($game->name) ?>">
        <?php else: ?>
            <div class="no-poster">Pas d'image</div>
        <?php endif; ?>
    </div>

    <div>
        <table>
            <tr>
                <th>Titre</th>
                <td><?= htmlspecialchars($game->name) ?></td>
            </tr>
            <tr>
                <th>Année</th>
                <td><?= $game->releaseYear ?></td>
            </tr>
            <tr>
                <th>Développeur</th>
                <td><?= htmlspecialchars($game->developer_name ?? 'Inconnu') ?></td>
            </tr>
            <tr>
                <th>Prix</th>
                <td>
                    <?php if ($game->price == 0): ?>
                        <span style="color:#4caf50;">Gratuit</span>
                    <?php else: ?>
                        <?= number_format($game->price / 100, 2) ?> €
                    <?php endif; ?>
                </td>
            </tr>
            <tr>
                <th>Score</th>
                <td>
                    <?php if ($game->metacritic): ?>
                        <span style="background-color:<?= $game->metacritic >= 75 ? '#4caf50' : ($game->metacritic >= 50 ? '#ff9800' : '#f44336') ?>;
                                     color:white; padding:2px 8px; border-radius:4px; font-weight:bold;">
                            <?= $game->metacritic ?>
                        </span>
                    <?php else: ?>
                        N/A
                    <?php endif; ?>
                </td>
            </tr>
            <?php if (!empty($game->shortDescription)): ?>
            <tr>
                <th style="vertical-align:top;">Description</th>
                <td style="line-height:1.6;">
                    <?= nl2br(htmlspecialchars($game->shortDescription)) ?>
                </td>
            </tr>
            <?php endif; ?>
            <tr>
                <th>Platformes</th>
                <td>
                    <?= $game->windows ? '<span style="margin-right:8px;"> Windows</span>' : '' ?>
                    <?= $game->linux   ? '<span style="margin-right:8px;"> Linux</span>'   : '' ?>
                    <?= $game->mac     ? '<span> Mac</span>'                                : '' ?> 
                </td>
            </tr>
            <tr>
                <th>Catégories</th>
                <td>
                    <?php foreach ($categories as $cat): ?>
                        <a href="<?= base_url('index.php/catalogue/category/' . $cat->id) ?>">
                            <?= htmlspecialchars($cat->description) ?>
                        </a>
                        &nbsp;
                    <?php endforeach; ?>
                </td>
            </tr>
            <tr>
                <th>Genres</th>
                <td>
                    <?php foreach ($genres as $genre): ?>
                        <a href="<?= base_url('index.php/catalogue/genre/' . $genre->id) ?>">
                            <?= htmlspecialchars($genre->description) ?>
                        </a>
                        &nbsp;
                    <?php endforeach; ?>
                </td>
            </tr>
        </table>

        <div style="margin-top: 20px; display: flex; gap: 10px;">
            <a href="<?= base_url('index.php/jeu/editer/' . $game->id) ?>" class="btn btn-primary">Modifier</a>
            <a href="<?= base_url('index.php/jeu/supprimer/' . $game->id) ?>" class="btn btn-danger">Supprimer</a>
            <a href="<?= base_url('index.php/catalogue') ?>" class="btn btn-secondary">← Retour</a>
        </div>
    </div>
</div>

<?php endif; ?>
