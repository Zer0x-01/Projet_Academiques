#include "taquin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <graph.h>
#include <X11/keysym.h>

/* Déclaration et Initialisation des Variables Globales */
int tuiles[MAX_DIM][MAX_DIM];
int initial_tuiles[MAX_DIM][MAX_DIM];
int colonnes;
int lignes;
char imageChoisie[80];
int largeurCase, hauteurCase;
int vide_x, vide_y;
int partieLancee = 0;
char messageStatut[200];
int nombreCoups;
int jeuTermine;
int jeuDoitQuitter = 0;
int menuSelection = 0;
int menuMaxItems = 0;

/* Variables externes de la librairie graphique */
extern int _X;
extern int _Y;

/* --- Implémentations des Fonctions --- */

/* Dessine un bouton (commentaire élève) */
void DessinerBouton(int x, int y, int largeur, int hauteur, char *texte, int taillePolice, int selectionne)
{
    /* ... Le corps complet de la fonction DessinerBouton */
    int largeurTexte;
    int hauteurTexte;
    int xTexte;
    int yTexte;
    couleur couleurFond;

    largeurTexte = TailleChaineEcran(texte, taillePolice);
    hauteurTexte = TailleSupPolice(taillePolice) + TailleInfPolice(taillePolice);

    xTexte = x + (largeur - largeurTexte) / 2;
    yTexte = y + (hauteur - hauteurTexte) / 2 + TailleSupPolice(taillePolice);
    
    if (selectionne) {
        couleurFond = CouleurParNom("yellow");
    } else {
        couleurFond = CouleurParNom("gray");
    }

    ChoisirCouleurDessin(couleurFond);
    RemplirRectangle(x, y, largeur, hauteur);

    ChoisirCouleurDessin(CouleurParNom("black"));
    DessinerRectangle(x, y, largeur, hauteur);
    EcrireTexte(xTexte, yTexte, texte, taillePolice);
}

/* Test si la souris est dans le rectangle (utilisé pour boutons) */
int ClicDansBouton(int x, int y, int largeur, int hauteur)
{
    /* ... Le corps complet de la fonction ClicDansBouton */
    SourisPosition();
    if (_X >= x && _X <= x + largeur && _Y >= y && _Y <= y + hauteur) {
        return 1;
    }
    return 0;
}

/* Vérifie si Entrée a été pressée */
int EntreePressee(void)
{
    /* ... Le corps complet de la fonction EntreePressee */
    if (ToucheEnAttente()) {
        KeySym key = Touche();
        if (key == XK_Return || key == XK_KP_Enter) {
            return 1;
        }
        if (ToucheEnAttente()) {
             Touche();
        }
    }
    return 0;
}

/* Vérifie si une touche précise a été pressée */
int TouchePressee(KeySym cle)
{
    /* ... Le corps complet de la fonction TouchePressee */
    if (ToucheEnAttente()) {
        KeySym key = Touche();
        if (key == cle) {
            return 1;
        }
    }
    return 0;
}

/* Initialise le plateau en ordre (0 en haut-gauche) */
void InitialiserTuiles(void)
{
    /* ... Le corps complet de la fonction InitialiserTuiles */
    int i;
    int j;
    int index;

    index = 1;
    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            if (i == 0 && j == 0) {
                tuiles[i][j] = 0;
            } else {
                tuiles[i][j] = index;
                index++;
            }
        }
    }
    vide_x = 0;
    vide_y = 0;
}

/* Sauvegarde l'état courant pour RESET */
void CopierCourantVersInitial(void)
{
    /* ... Le corps complet de la fonction CopierCourantVersInitial */
    int i;
    int j;
    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            initial_tuiles[i][j] = tuiles[i][j];
        }
    }
}

/* Test d'adjacence (distance de Manhattan = 1) */
int Adjacent(int r1, int c1, int r2, int c2)
{
    /* ... Le corps complet de la fonction Adjacent */
    if (r1 == r2 && (c1 == c2 + 1 || c1 + 1 == c2)) return 1;
    if (c1 == c2 && (r1 == r2 + 1 || r1 + 1 == r2)) return 1;
    return 0;
}

/* Déplace le vide vers (ny,nx) (échange) */
void MoveVideTo(int ny, int nx)
{
    /* ... Le corps complet de la fonction MoveVideTo */
    int tmp;
    tmp = tuiles[ny][nx];
    tuiles[ny][nx] = tuiles[vide_y][vide_x];
    tuiles[vide_y][vide_x] = tmp;
    vide_x = nx;
    vide_y = ny;
}

/* Mélange en garantissant solvabilité et en ramenant le vide en (0,0) */
void MelangerTuiles(int iterations)
{
    /* ... Le corps complet de la fonction MelangerTuiles */
    int step;
    int dir;
    int nx;
    int ny;
    int tries;

    srand((unsigned int) time(NULL));
    
    /* Mélange aléatoire par mouvements valides */
    for (step = 0; step < iterations; step++) {
        tries = 0;
        do {
            dir = rand() % 4;
            nx = vide_x;
            ny = vide_y;
            if (dir == 0) nx = vide_x - 1;
            else if (dir == 1) nx = vide_x + 1;
            else if (dir == 2) ny = vide_y - 1;
            else ny = vide_y + 1;
            tries++;
        } while ((nx < 0 || nx >= colonnes || ny < 0 || ny >= lignes) && tries < 10);

        if (nx >= 0 && nx < colonnes && ny >= 0 && ny < lignes) {
            MoveVideTo(ny, nx);
        }
    }

    /* Ramène le vide en (0,0) simplement */
    while (vide_x != 0 || vide_y != 0) {
        if (vide_x > 0) {
            nx = vide_x - 1;
            ny = vide_y;
            MoveVideTo(ny, nx);
        } else if (vide_y > 0) {
            nx = vide_x;
            ny = vide_y - 1;
            MoveVideTo(ny, nx);
        } else {
            break;
        }
    }

    CopierCourantVersInitial();
}

/* Affiche les tuiles (charge morceaux d'image sauf le vide) */
void AfficherTuiles(void)
{
    /* ... Le corps complet de la fonction AfficherTuiles */
    int i;
    int j;
    int val;
    int tuileIndex;
    int x;
    int y;

    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            val = tuiles[i][j];
            x = j * (largeurCase + MARGE) + OFFSET_X;
            y = i * (hauteurCase + MARGE) + OFFSET_Y;
            
            if (val == 0) {
                ChoisirCouleurDessin(CouleurParNom("white"));
                RemplirRectangle(x, y, largeurCase, hauteurCase);
            } else {
                tuileIndex = val;
                ChargerImage(imageChoisie, x, y,
                             (tuileIndex % colonnes) * largeurCase,
                             (tuileIndex / colonnes) * hauteurCase,
                             largeurCase, hauteurCase);
                ChoisirCouleurDessin(CouleurParNom("black"));
                DessinerRectangle(x, y, largeurCase, hauteurCase);
            }
        }
    }
}

/* Dessine le statut (compteur de coups) */
void DessinerStatutJeu(void)
{
    /* ... Le corps complet de la fonction DessinerStatutJeu */
    ChoisirCouleurDessin(CouleurParNom("lightgray"));
    RemplirRectangle(50, 20, 1100, 30);
    ChoisirCouleurDessin(CouleurParNom("black"));
    EcrireTexte(50, 30, messageStatut, 2);
}

/* Vérifie la condition de victoire (0 en (0,0) et séquence) */
int VerifierVictoire(void)
{
    /* ... Le corps complet de la fonction VerifierVictoire */
    int i;
    int j;
    int attendu;

    attendu = 1;
    
    if (tuiles[0][0] != 0) return 0;

    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            if (i == 0 && j == 0) continue;
            if (tuiles[i][j] != attendu) return 0;
            attendu++;
        }
    }
    return 1;
}

/* Gère un clic dans la grille : calcule la tuile cliquée et échange si adjacent */
void GererClicJeu(int click_x, int click_y, int *besoinDeReafficherJeu)
{
    /* ... Le corps complet de la fonction GererClicJeu */
    int tuile_j;
    int tuile_i;
    int x_min;
    int y_min;
    int x_max;
    int y_max;
    int tmp;

    *besoinDeReafficherJeu = 0;

    if (jeuTermine) {
        return;
    }

    tuile_j = (click_x - OFFSET_X) / (largeurCase + MARGE);
    tuile_i = (click_y - OFFSET_Y) / (hauteurCase + MARGE);

    if (tuile_i < 0 || tuile_i >= lignes || tuile_j < 0 || tuile_j >= colonnes) {
        return;
    }

    x_min = OFFSET_X + tuile_j * (largeurCase + MARGE);
    y_min = OFFSET_Y + tuile_i * (hauteurCase + MARGE);
    x_max = x_min + largeurCase;
    y_max = y_min + hauteurCase;

    if (click_x < x_min || click_x > x_max || click_y < y_min || click_y > y_max) {
        return;
    }

    if (Adjacent(tuile_i, tuile_j, vide_y, vide_x)) {
        tmp = tuiles[tuile_i][tuile_j];
        tuiles[tuile_i][tuile_j] = tuiles[vide_y][vide_x];
        tuiles[vide_y][vide_x] = tmp;
        vide_x = tuile_j;
        vide_y = tuile_i;
        nombreCoups++;
        *besoinDeReafficherJeu = 1;
        
        if (!jeuTermine) {
            sprintf(messageStatut, "Game in progress (Moves: %d).", nombreCoups);
        }
    }
}

/* Gère mouvements via les flèches (déplace la tuile adjacente dans le vide) */
void GererMouvementClavier(KeySym key, int *besoinDeReafficherJeu)
{
    /* ... Le corps complet de la fonction GererMouvementClavier */
    int tuile_a_bouger_x = -1;
    int tuile_a_bouger_y = -1;

    *besoinDeReafficherJeu = 0;
    if (jeuTermine) {
        return;
    }

    if (key == XK_Left) {
        tuile_a_bouger_x = vide_x + 1;
        tuile_a_bouger_y = vide_y;
    } else if (key == XK_Right) {
        tuile_a_bouger_x = vide_x - 1;
        tuile_a_bouger_y = vide_y;
    } else if (key == XK_Up) {
        tuile_a_bouger_x = vide_x;
        tuile_a_bouger_y = vide_y + 1;
    } else if (key == XK_Down) {
        tuile_a_bouger_x = vide_x;
        tuile_a_bouger_y = vide_y - 1;
    }

    if (tuile_a_bouger_x >= 0 && tuile_a_bouger_x < colonnes &&
        tuile_a_bouger_y >= 0 && tuile_a_bouger_y < lignes) 
    {
        MoveVideTo(tuile_a_bouger_y, tuile_a_bouger_x);
        nombreCoups++;
        *besoinDeReafficherJeu = 1;
        
        if (!jeuTermine) {
            sprintf(messageStatut, "Game in progress (Moves: %d).", nombreCoups);
        }
    }
}

/* Boucle principale du jeu (affichage, gestion événements) */
void BoucleJeu(void)
{
    /* ... Le corps complet de la fonction BoucleJeu */
    int quitter;
    int x;
    int y;
    int besoinDeReafficherJeu;
    int victoire;
    int i;
    int j;
    KeySym touchePressee = 0;
    int bouton_menu_x;
    int bouton_menu_y[3];
    int bouton_menu_largeur;
    int bouton_menu_hauteur;
    int clicDetecte = 0;
    
    bouton_menu_x = 1200;
    bouton_menu_y[0] = 100;
    bouton_menu_y[1] = 200;
    bouton_menu_y[2] = 300;
    bouton_menu_largeur = 200;
    bouton_menu_hauteur = 50;
    
    menuSelection = 0; 
    menuMaxItems = 3;

    if (!partieLancee) {
        if (strcmp(imageChoisie, "Images/Img1.jpg") == 0) {
            largeurCase = 600 / colonnes;
            hauteurCase = 400 / lignes;
        } else if (strcmp(imageChoisie, "Images/Img2.jpg") == 0) {
            largeurCase = 300 / colonnes;
            hauteurCase = 400 / lignes;
        } else {
            largeurCase = 400 / colonnes;
            hauteurCase = 400 / lignes;
        }

        InitialiserTuiles();
        MelangerTuiles(300);
        CopierCourantVersInitial();

        nombreCoups = 0;
        jeuTermine = 0;
        sprintf(messageStatut, "Game in progress (Moves: %d).", nombreCoups);
        partieLancee = 1;
    }

    quitter = 0;
    besoinDeReafficherJeu = 1;
    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficherJeu) {
            EffacerEcran(CouleurParNom("lightgray"));
            EcrireTexte(700, 50, "Taquin Game", 2);
            AfficherTuiles();
            DessinerStatutJeu();
            
            DessinerBouton(bouton_menu_x, bouton_menu_y[0], bouton_menu_largeur, bouton_menu_hauteur, "MAIN MENU (M)", 2, 0);
            DessinerBouton(bouton_menu_x, bouton_menu_y[1], bouton_menu_largeur, bouton_menu_hauteur, "SHUFFLE (S)", 2, 0);
            DessinerBouton(bouton_menu_x, bouton_menu_y[2], bouton_menu_largeur, bouton_menu_hauteur, "RESET (R)", 2, 0);
            
            ChoisirCouleurDessin(CouleurParNom("darkgray"));
            EcrireTexte(1200, 380, "Grid movement: ARROWS", 2);
            EcrireTexte(1200, 410, "Button action: M/S/R or Click", 2);
            ChoisirCouleurDessin(CouleurParNom("black"));
            
            besoinDeReafficherJeu = 0;
            
            victoire = VerifierVictoire();
            if (victoire && !jeuTermine) {
                jeuTermine = 1;
                quitter = 1;
                continue;
            }
        }
        
        touchePressee = 0;
        clicDetecte = 0;
        
        while (!clicDetecte && !ToucheEnAttente()) {
            if (SourisCliquee()) {
                clicDetecte = 1;
            }
        }
        
        if (ToucheEnAttente()) {
             touchePressee = Touche();
        }
        
        x = _X;
        y = _Y;

        if (clicDetecte) {
            if (ClicDansBouton(bouton_menu_x, bouton_menu_y[0], bouton_menu_largeur, bouton_menu_hauteur)) {
                touchePressee = 'm';
            } else if (ClicDansBouton(bouton_menu_x, bouton_menu_y[1], bouton_menu_largeur, bouton_menu_hauteur)) {
                touchePressee = 's';
            } else if (ClicDansBouton(bouton_menu_x, bouton_menu_y[2], bouton_menu_largeur, bouton_menu_hauteur)) {
                touchePressee = 'r';
            }
        }
        
        if (touchePressee == 'm' || touchePressee == 'M') {
            quitter = 1;
            partieLancee = 0;
        } else if (touchePressee == 's' || touchePressee == 'S') {
            InitialiserTuiles();
            MelangerTuiles(300);
            CopierCourantVersInitial();
            nombreCoups = 0;
            jeuTermine = 0;
            sprintf(messageStatut, "Game shuffled! (Moves: %d).", nombreCoups);
            besoinDeReafficherJeu = 1;
        } else if (touchePressee == 'r' || touchePressee == 'R') {
            for (i = 0; i < lignes; i++) {
                for (j = 0; j < colonnes; j++) {
                    tuiles[i][j] = initial_tuiles[i][j];
                }
            }
            vide_x = 0;
            vide_y = 0;
            nombreCoups = 0;
            jeuTermine = 0;
            sprintf(messageStatut, "Game reset (Moves: %d).", nombreCoups);
            besoinDeReafficherJeu = 1;
        } else if (touchePressee == XK_Left || touchePressee == XK_Right || touchePressee == XK_Up || touchePressee == XK_Down) {
            GererMouvementClavier(touchePressee, &besoinDeReafficherJeu);
        } else if (clicDetecte) {
            GererClicJeu(x, y, &besoinDeReafficherJeu);
        }

        if (ToucheEnAttente()) {
             Touche();
        }
    }
    
    if (jeuTermine) {
        MenuFinJeu();
    }
}

/* Menu affiché après victoire (affiche score et options) */
void MenuFinJeu(void)
{
    /* ... Le corps complet de la fonction MenuFinJeu */
    int quitter = 0;
    int x, y;
    int besoinDeReafficher = 1;
    char scoreTexte[100];
    KeySym touchePressee = 0;
    int bouton_menu_x[2];
    int bouton_menu_y;
    int bouton_menu_largeur;
    int bouton_menu_hauteur;
    
    bouton_menu_x[0] = 450;
    bouton_menu_x[1] = 750;
    bouton_menu_y = 400;
    bouton_menu_largeur = 200;
    bouton_menu_hauteur = 50;
    
    menuSelection = 0;
    menuMaxItems = 2;

    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficher) {
            EffacerEcran(CouleurParNom("white"));
            ChoisirCouleurDessin(CouleurParNom("blue"));
            EcrireTexte(600, 150, " WELL PLAYED! VICTORY! ", 3);
            
            sprintf(scoreTexte, "You solved the puzzle in %d moves.", nombreCoups);
            ChoisirCouleurDessin(CouleurParNom("black"));
            EcrireTexte(600, 250, scoreTexte, 2);
            
            DessinerBouton(bouton_menu_x[0], bouton_menu_y, bouton_menu_largeur, bouton_menu_hauteur, "MAIN MENU (M)", 2, menuSelection == 0);
            DessinerBouton(bouton_menu_x[1], bouton_menu_y, bouton_menu_largeur, bouton_menu_hauteur, "QUIT (Q)", 2, menuSelection == 1);
            
            ChoisirCouleurDessin(CouleurParNom("darkgray"));
            EcrireTexte(600, 500, "Navigate: LEFT/RIGHT (or M/Q)", 2);
            EcrireTexte(600, 530, "Select: ENTER", 2);
            ChoisirCouleurDessin(CouleurParNom("black"));
            
            besoinDeReafficher = 0;
        }

        touchePressee = 0;
        while (!SourisCliquee() && !ToucheEnAttente()) {}
        if (ToucheEnAttente()) {
             touchePressee = Touche();
        }
        
        SourisPosition();
        x = _X;
        y = _Y;
        
        if (touchePressee == XK_Left || touchePressee == 'm' || touchePressee == 'M') {
            menuSelection = 0;
            besoinDeReafficher = 1;
        } else if (touchePressee == XK_Right || touchePressee == 'q' || touchePressee == 'Q') {
            menuSelection = 1;
            besoinDeReafficher = 1;
        } 
        
        if (touchePressee == XK_Return || touchePressee == XK_KP_Enter || (ClicDansBouton(bouton_menu_x[0], bouton_menu_y, bouton_menu_largeur, bouton_menu_hauteur) && menuSelection == 0)) {
            quitter = 1;
        } else if (ClicDansBouton(bouton_menu_x[1], bouton_menu_y, bouton_menu_largeur, bouton_menu_hauteur)) { 
            jeuDoitQuitter = 1;
            quitter = 1;
        }

        if (touchePressee == 'm' || touchePressee == 'M') {
             quitter = 1;
        } else if (touchePressee == 'q' || touchePressee == 'Q') {
             jeuDoitQuitter = 1;
             quitter = 1;
        }
        
        if (ToucheEnAttente()) {
             Touche();
        }
    }
}