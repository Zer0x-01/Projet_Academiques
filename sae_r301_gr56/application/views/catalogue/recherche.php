<h1>Résultats pour : "<?= htmlspecialchars($query) ?>"</h1>

<?php if (empty($games)): ?>
    <p>Aucun jeu trouvé.</p>
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