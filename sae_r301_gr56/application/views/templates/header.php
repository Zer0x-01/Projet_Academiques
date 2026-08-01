<!DOCTYPE html>

<html lang="fr">

<head>

    <meta charset="UTF-8">

    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title><?= htmlspecialchars($title) ?> — GameDB</title>

    <style>

        /* ── Reset ── */

        * { box-sizing: border-box; margin: 0; padding: 0; }



        /* ── Body ── */

        body {

            font-family: 'Segoe UI', Arial, sans-serif;

            background: #0f0f0f;

            color: #e0e0e0;

            min-height: 100vh;

        }



        /* ── Header ── */

        header {

            background: #1a1a1a;

            border-bottom: 2px solid #e94560;

            padding: 16px 40px;

            display: flex;

            align-items: center;

        }

        header .logo {

            color: #e94560;

            text-decoration: none;

            font-size: 1.6rem;

            font-weight: bold;

            letter-spacing: 1px;

        }



        /* ── Nav ── */

        nav {

            background: #141414;

            padding: 10px 40px;

            display: flex;

            align-items: center;

            gap: 24px;

            border-bottom: 1px solid #2a2a2a;

        }

        nav a {

            color: #aaa;

            text-decoration: none;

            font-size: 0.95rem;

            transition: color 0.2s;

        }

        nav a:hover { color: #e94560; }



        /* ── Barre de recherche ── */

        .search-form {

            display: flex;

            gap: 6px;

            margin-left: auto;

        }

        .search-form input {

            background: #1e1e1e;

            border: 1px solid #333;

            border-radius: 4px;

            color: #e0e0e0;

            padding: 6px 12px;

            font-size: 0.9rem;

            width: 220px;

            outline: none;

            transition: border 0.2s;

        }

        .search-form input:focus { border-color: #e94560; }

        .search-form button {

            background: #e94560;

            border: none;

            border-radius: 4px;

            color: #fff;

            padding: 6px 14px;

            cursor: pointer;

            font-size: 0.9rem;

            transition: background 0.2s;

        }

        .search-form button:hover { background: #c73652; }



        /* ── Main ── */

        main {

            max-width: 1100px;

            margin: 36px auto;

            padding: 0 24px;

        }



        /* ── Titres ── */

        h1 {

            font-size: 1.8rem;

            color: #fff;

            margin-bottom: 24px;

            border-left: 4px solid #e94560;

            padding-left: 12px;

        }

        h2 {

            font-size: 1.2rem;

            color: #ccc;

            margin: 28px 0 14px;

        }



        /* ── Liens ── */

        a { color: #e94560; text-decoration: none; }

        a:hover { text-decoration: underline; }



        /* ── Tags (catégories / genres) ── */

        .tag-list {

            display: flex;

            flex-wrap: wrap;

            gap: 10px;

            margin-bottom: 24px;

        }

        .tag {

            background: #1e1e1e;

            border: 1px solid #333;

            border-radius: 20px;

            padding: 6px 16px;

            font-size: 0.88rem;

            transition: border-color 0.2s;

        }

        .tag:hover { border-color: #e94560; }

        .tag a { color: #ccc; }

        .tag a:hover { color: #e94560; text-decoration: none; }



        /* ── Liste de jeux ── */

        .game-list { list-style: none; padding: 0; }

        .game-list li {

            background: #1a1a1a;

            border-left: 3px solid #e94560;

            border-radius: 4px;

            margin-bottom: 8px;

            padding: 10px 16px;

            transition: background 0.2s;

        }

        .game-list li:hover { background: #222; }

        .game-list li a { color: #e0e0e0; }

        .game-list li a:hover { color: #e94560; text-decoration: none; }

        .year {

            color: #666;

            font-size: 0.85rem;

            margin-left: 8px;

        }



        /* ── Tri ── */

        .sort-bar {

            margin-bottom: 16px;

            font-size: 0.9rem;

            color: #888;

        }

        .sort-bar a {

            color: #aaa;

            margin-left: 10px;

            padding: 4px 10px;

            border: 1px solid #333;

            border-radius: 4px;

        }

        .sort-bar a:hover {

            border-color: #e94560;

            color: #e94560;

            text-decoration: none;

        }

        .sort-bar a.active {

            border-color: #e94560;

            color: #e94560;

        }



        /* ── Boutons ── */

        .btn {

            display: inline-block;

            padding: 8px 20px;

            border-radius: 4px;

            font-size: 0.95rem;

            cursor: pointer;

            text-decoration: none;

            border: none;

        }

        .btn-primary { background: #e94560; color: #fff; }

        .btn-primary:hover { background: #c73652; text-decoration: none; }

        .btn-secondary { background: #2a2a2a; color: #ccc; }

        .btn-secondary:hover { background: #333; text-decoration: none; }

        .btn-danger { background: #7a1a2a; color: #fff; }

        .btn-danger:hover { background: #9a2a3a; text-decoration: none; }



        /* ── Carte jeu ── */

        .game-card {

            background: #1a1a1a;

            border: 1px solid #2a2a2a;

            border-radius: 8px;

            padding: 24px;

            max-width: 800px;

            display: flex;

            gap: 28px;

        }

        .game-card img {

            width: 180px;

            border-radius: 6px;

            object-fit: cover;

            flex-shrink: 0;

        }

        .game-card table { border-collapse: collapse; width: 100%; }

        .game-card th {

            text-align: left;

            padding: 8px 20px 8px 0;

            color: #888;

            font-weight: normal;

            width: 160px;

            vertical-align: top;

        }

        .game-card td { padding: 8px 0; color: #e0e0e0; }



        /* ── Messages d'erreur ── */

        .error-box {

            background: #2a1a1a;

            border-left: 4px solid #e94560;

            border-radius: 4px;

            padding: 12px 16px;

            margin-bottom: 20px;

            color: #f0a0a0;

        }



        /* ── Formulaire ── */

        .form-box {

            background: #1a1a1a;

            border: 1px solid #2a2a2a;

            border-radius: 8px;

            padding: 28px;

            max-width: 640px;

        }

        .form-group { margin-bottom: 16px; }

        .form-group label {

            display: block;

            margin-bottom: 6px;

            color: #aaa;

            font-size: 0.95rem;

        }

        .form-group input,

        .form-group select,

        .form-group textarea {

            width: 100%;

            background: #1e1e1e;

            border: 1px solid #333;

            border-radius: 4px;

            color: #e0e0e0;

            padding: 8px 12px;

            font-size: 0.95rem;

            outline: none;

            transition: border 0.2s;

        }

        .form-group input:focus,

        .form-group select:focus,

        .form-group textarea:focus { border-color: #e94560; }

        .form-group textarea { resize: vertical; }



        /* ── Footer ── */

        footer {

            text-align: center;

            padding: 24px;

            color: #444;

            font-size: 0.85rem;

            margin-top: 60px;

            border-top: 1px solid #1e1e1e;

        }

    </style>

</head>

<body>

<header>

    <a class="logo" href="<?= base_url('index.php/catalogue') ?>">🎮 GameDB</a>

</header>

<nav>
    <form class="search-form" method="get"
          action="<?= base_url('index.php/catalogue/recherche') ?>">
        <div style="position:relative;">
            <input type="text" name="q" id="search-input"
                   placeholder="Rechercher un jeu..." autocomplete="off">
            <div id="suggestions" style="
                display:none;
                position:absolute;
                top:100%;
                left:0;
                width:100%;
                background:#1e1e1e;
                border:1px solid #333;
                border-radius:4px;
                z-index:999;
            "></div>
        </div>
        <button type="submit">🔍</button>
    </form>
</nav>

<script>
const input = document.getElementById('search-input');
const box   = document.getElementById('suggestions');

input.addEventListener('input', function() {
    const q = this.value.trim();
    if (q.length < 2) { box.style.display = 'none'; return; }

    fetch('<?= base_url('index.php/catalogue/suggest') ?>?q=' + encodeURIComponent(q))
        .then(r => r.json())
        .then(games => {
            if (!games.length) { box.style.display = 'none'; return; }
            box.innerHTML = games.map(g => `
                <div onclick="window.location='<?= base_url('index.php/catalogue/game/') ?>${g.id}'"
                     style="padding:8px 12px; cursor:pointer; color:#e0e0e0; border-bottom:1px solid #2a2a2a;"
                     onmouseover="this.style.background='#2a2a2a'"
                     onmouseout="this.style.background='none'">
                    ${g.name}
                    <span style="color:#666; font-size:0.8rem; float:right;">${g.releaseYear}</span>
                </div>
            `).join('');
            box.style.display = 'block';
        });
});

document.addEventListener('click', function(e) {
    if (!input.contains(e.target)) box.style.display = 'none';
});
</script>

<main>