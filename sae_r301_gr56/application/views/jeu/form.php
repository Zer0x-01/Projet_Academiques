<style>
    .tag-selector {
        border: 1px solid #555;
        border-radius: 4px;
        background: #2a2a2a;
        padding: 6px;
    }
    .tag-selected {
        display: flex;
        flex-wrap: wrap;
        gap: 6px;
        min-height: 32px;
        margin-bottom: 6px;
    }
    .tag-selected span {
        background: #444;
        color: #e0e0e0;
        padding: 2px 8px;
        border-radius: 12px;
        font-size: 0.85rem;
        display: flex;
        align-items: center;
        gap: 4px;
    }
    .tag-selected span button {
        background: none;
        border: none;
        color: #aaa;
        cursor: pointer;
        font-size: 0.9rem;
        padding: 0;
        line-height: 1;
    }
    .tag-selected span button:hover {
        color: #f44336;
    }
    .tag-selector select {
        width: 100%;
        background: #1e1e1e;
        color: #e0e0e0;
        border: 1px solid #444;
        border-radius: 4px;
        padding: 4px;
        height: 34px;
    }
</style>

<?php $is_edit = isset($game) && $game !== null && isset($game->id); ?>

<h1><?= $is_edit ? 'Modifier : ' . htmlspecialchars($game->name) : 'Ajouter un jeu' ?></h1>

<?php if (!empty($errors)): ?>
    <div class="error-box">
        <ul style="margin:0; padding-left:16px;">
            <?php foreach ($errors as $err): ?>
                <li><?= htmlspecialchars($err) ?></li>
            <?php endforeach; ?>
        </ul>
    </div>
<?php endif; ?>

<?php
$action = $is_edit
    ? base_url('index.php/jeu/editer/' . $game->id)
    : base_url('index.php/jeu/creer');
?>

<div class="form-box">
<form method="post" action="<?= $action ?>" enctype="multipart/form-data">

    <div class="form-group">
        <label>Nom *</label>
        <input type="text" name="name"
               value="<?= htmlspecialchars($game->name ?? '') ?>">
    </div>

    <div class="form-group">
        <label>Année de sortie *</label>
        <input type="number" name="releaseYear" min="1970" max="2100"
               value="<?= htmlspecialchars($game->releaseYear ?? '') ?>"
               style="width:140px;">
    </div>

    <div class="form-group">
        <label>Développeur *</label>
        <select name="developerId">
            <option value="">-- Choisir --</option>
            <?php foreach ($developers as $dev): ?>
                <option value="<?= $dev->id ?>"
                    <?= (isset($game->developerId) && $game->developerId == $dev->id) ? 'selected' : '' ?>>
                    <?= htmlspecialchars($dev->name) ?>
                </option>
            <?php endforeach; ?>
        </select>
    </div>

    <div class="form-group">
        <label>Description courte</label>
        <textarea name="shortDescription" rows="4"><?= htmlspecialchars($game->shortDescription ?? '') ?></textarea>
    </div>

    <div class="form-group">
        <label>Prix (€)</label>
        <input type="number" name="price" step="0.01" min="0"
               value="<?= htmlspecialchars($game->price ?? '') ?>"
               style="width:140px;">
    </div>

    <div class="form-group">
        <label>Score Metacritic</label>
        <input type="number" name="metacritic" min="0" max="100"
               value="<?= htmlspecialchars($game->metacritic ?? '') ?>"
               style="width:100px;">
    </div>

    <div class="form-group">
        <label>Plateformes</label>
        <div style="display:flex; gap:20px; margin-top:6px;">
            <label style="color:#e0e0e0;">
                <input type="checkbox" name="windows" value="1"
                    <?= !empty($game->windows) ? 'checked' : '' ?>>
                Windows
            </label>
            <label style="color:#e0e0e0;">
                <input type="checkbox" name="linux" value="1"
                    <?= !empty($game->linux) ? 'checked' : '' ?>>
                Linux
            </label>
            <label style="color:#e0e0e0;">
                <input type="checkbox" name="mac" value="1"
                    <?= !empty($game->mac) ? 'checked' : '' ?>>
                Mac
            </label>
        </div>
    </div>

    <?php if ($is_edit && !empty($game->posterId)): ?>
    <div class="form-group">
        <label>Poster</label>
        <div>
            <img src="<?= base_url('index.php/catalogue/poster/' . $game->posterId) ?>"
                style="max-width:150px; border-radius:4px;">
        </div>
    </div>
    <?php endif; ?>
    <?php if (!$is_edit || empty($game->posterId)): ?>
    <div class="form-group">
        <label>Poster (facultatif)</label>
        <input type="file" name="poster" accept="image/jpeg">
    </div>
    <?php endif; ?>

    <!-- Catégories -->
    <div class="form-group">
        <label>Catégories</label>
        <div class="tag-selector">
            <div class="tag-selected" id="selected-categories"></div>
            <select id="cat-select" onchange="addTag('categories', this)">
                <option value="">-- Choisir une catégorie --</option>
                <?php foreach ($all_categories as $cat): ?>
                    <option value="<?= $cat->id ?>" data-label="<?= htmlspecialchars($cat->description) ?>">
                        <?= htmlspecialchars($cat->description) ?>
                    </option>
                <?php endforeach; ?>
            </select>
        </div>
        <div id="inputs-categories"></div>
    </div>

    <!-- Genres -->
    <div class="form-group">
        <label>Genres</label>
        <div class="tag-selector">
            <div class="tag-selected" id="selected-genres"></div>
            <select id="genre-select" onchange="addTag('genres', this)">
                <option value="">-- Choisir un genre --</option>
                <?php foreach ($all_genres as $genre): ?>
                    <option value="<?= $genre->id ?>" data-label="<?= htmlspecialchars($genre->description) ?>">
                        <?= htmlspecialchars($genre->description) ?>
                    </option>
                <?php endforeach; ?>
            </select>
        </div>
        <div id="inputs-genres"></div>
    </div>

    <div style="display:flex; gap:12px; margin-top:8px;">
        <button type="submit" class="btn btn-primary">
            <?= $is_edit ? 'Enregistrer' : 'Créer le jeu' ?>
        </button>
        <?php
        $annuler_url = $is_edit
            ? base_url('index.php/catalogue/game/' . $game->id)
            : base_url('index.php/catalogue');
        ?>
        <a href="<?= $annuler_url ?>" class="btn btn-secondary">
            Annuler
        </a>
    </div>

</form>
</div>

<script>
    const selected = { categories: {}, genres: {} };

    function addTag(type, selectEl) {
        const id    = selectEl.value;
        const label = selectEl.options[selectEl.selectedIndex].dataset.label;
        if (!id || selected[type][id]) return;
        selected[type][id] = label;
        const tag = document.createElement('span');
        tag.id = `tag-${type}-${id}`;
        tag.innerHTML = `${label} <button type="button" onclick="removeTag('${type}', '${id}')">✕</button>`;
        document.getElementById(`selected-${type}`).appendChild(tag);
        const input = document.createElement('input');
        input.type  = 'hidden';
        input.name  = `${type}[]`;
        input.value = id;
        input.id    = `input-${type}-${id}`;
        document.getElementById(`inputs-${type}`).appendChild(input);
        selectEl.value = '';
    }

    function removeTag(type, id) {
        delete selected[type][id];
        document.getElementById(`tag-${type}-${id}`)?.remove();
        document.getElementById(`input-${type}-${id}`)?.remove();
    }

    // Pré-charger les sélections existantes (mode édition)
    <?php foreach ($game_categories as $cat_id): ?>
        <?php
            $cat = array_filter($all_categories, fn($c) => $c->id == $cat_id);
            $cat = reset($cat);
        ?>
        <?php if ($cat): ?>
        (function() {
            selected['categories']['<?= $cat->id ?>'] = '<?= addslashes($cat->description) ?>';
            const tag = document.createElement('span');
            tag.id = `tag-categories-<?= $cat->id ?>`;
            tag.innerHTML = `<?= addslashes($cat->description) ?> <button type="button" onclick="removeTag('categories', '<?= $cat->id ?>')">✕</button>`;
            document.getElementById('selected-categories').appendChild(tag);
            const input = document.createElement('input');
            input.type  = 'hidden';
            input.name  = 'categories[]';
            input.value = '<?= $cat->id ?>';
            input.id    = `input-categories-<?= $cat->id ?>`;
            document.getElementById('inputs-categories').appendChild(input);
        })();
        <?php endif; ?>
    <?php endforeach; ?>

    <?php foreach ($game_genres as $genre_id): ?>
        <?php
            $genre = array_filter($all_genres, fn($g) => $g->id == $genre_id);
            $genre = reset($genre);
        ?>
        <?php if ($genre): ?>
        (function() {
            selected['genres']['<?= $genre->id ?>'] = '<?= addslashes($genre->description) ?>';
            const tag = document.createElement('span');
            tag.id = `tag-genres-<?= $genre->id ?>`;
            tag.innerHTML = `<?= addslashes($genre->description) ?> <button type="button" onclick="removeTag('genres', '<?= $genre->id ?>')">✕</button>`;
            document.getElementById('selected-genres').appendChild(tag);
            const input = document.createElement('input');
            input.type  = 'hidden';
            input.name  = 'genres[]';
            input.value = '<?= $genre->id ?>';
            input.id    = `input-genres-<?= $genre->id ?>`;
            document.getElementById('inputs-genres').appendChild(input);
        })();
        <?php endif; ?>
    <?php endforeach; ?>
</script>