<h1>Supprimer un jeu</h1>

<div class="form-box">
    <p style="margin-bottom:16px;">
        Êtes-vous sûr de vouloir supprimer le jeu
        <strong style="color:#fff;"><?= htmlspecialchars($game->name) ?></strong> ?
    </p>
    <p style="color:#888; font-size:0.9rem; margin-bottom:20px;">
        Cette action est irréversible.
    </p>

    <form method="post" action="<?= base_url('index.php/jeu/supprimer/' . $game->id) ?>">
        <input type="hidden" name="confirm" value="1">
        <div style="display:flex; gap:12px;">
            <button type="submit" class="btn btn-danger">
                 Confirmer la suppression
            </button>
            <a href="<?= base_url('index.php/catalogue/game/' . $game->id) ?>"
               class="btn btn-secondary">
                Annuler
            </a>
        </div>
    </form>
</div>
