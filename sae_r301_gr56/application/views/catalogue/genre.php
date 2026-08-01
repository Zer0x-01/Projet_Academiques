<h1><?= htmlspecialchars($genre->description) ?></h1>

<div class="sort-bar">
    Trier par :
    <a href="?sort=name"        class="<?= $sort === 'name'        ? 'active' : '' ?>">Titre</a>
    <a href="?sort=releaseYear" class="<?= $sort === 'releaseYear' ? 'active' : '' ?>">Année</a>
</div>

<?php if (empty($games)): ?>
    <p>Aucun jeu dans ce genre.</p>
<?php else: ?>
    <ul class="game-list">
        <?php foreach ($games as $game): ?>
            <li>
                <a href="<?= base_url('index.php/catalogue/game/' . $game->id) ?>">
                    <?= htmlspecialchars($game->name) ?>
                </a>
                <span class="year"><?= $game->releaseYear ?></span>
            </li>
        <?php endforeach; ?>
    </ul>
<?php endif; ?>

<a href="<?= base_url('index.php/catalogue') ?>" class="btn btn-secondary" style="margin-top:16px">← Retour</a>