/* taquin_visual_enhanced.c - Version avec rendu visuel amélioré (C89 compatible) */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h> 

/* Ajout explicite de stddef.h pour garantir la définition de size_t en C89 */
#include <stddef.h> 

#include <graph.h> 
#include <X11/keysym.h> /* Nécessaire pour les codes de touches comme XK_Left, etc. */

/* Les variables globales _X et _Y pour la position de la souris
 * doivent être déclarées externes.
 */
extern int _X;
extern int _Y;

/* ---------- Constantes ---------- */
#define MIN_DIM 3
#define MAX_DIM 8
#define MARGE 5 /* Espace entre les tuiles */
#define OFFSET_X 80
#define OFFSET_Y 120
#define SHADOW_OFFSET 3 /* Ombre pour effet 3D */
#define FENETRE_LARGEUR 1500
#define FENETRE_HAUTEUR 800
#define MAX_PREVIEW_SIZE 200 /* Taille maximale pour la prévisualisation dans MenuOptions */

/* ---------- Structure pour les images ---------- */
typedef struct {
    char path[80];
    char name[20];
    int width;
    int height;
} ImageInfo;

/* ---------- Variables globales ---------- */
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
int imageSelection = 0; /* Index de l'image sélectionnée */

/* Images disponibles (NOMS MIS À JOUR) */
ImageInfo images[3] = {
    {"Images/Img1.jpg", "Raton Laveur", 600, 400},
    {"Images/Img2.jpg", "Chien", 300, 400},
    {"Images/Img3.jpg", "Dromadaire", 400, 400}
};

/* ---------- Prototypes (Déclarations de fonctions pour C89) ---------- */
void DessinerBoutonGradient(int x, int y, int largeur, int hauteur, char *texte, int taillePolice, int selectionne);
void DessinerCadreDecore(int x, int y, int largeur, int hauteur, int epaisseur);
int ClicDansBouton(int x, int y, int largeur, int hauteur);
void InitialiserTuiles(void);
void CopierCourantVersInitial(void);
void MelangerTuiles(int iterations);
void AfficherTuile(int i, int j);
void AfficherTuiles(void);
void DessinerStatutJeu(void);
int VerifierVictoire(void);
void GererClicJeu(int click_x, int click_y, int *besoinDeReafficherJeu);
void GererMouvementClavier(KeySym key, int *besoinDeReafficherJeu);
void BoucleJeu(void);
void MenuFinJeu(void);
void RedessinerSpinners(void);
int Adjacent(int r1, int c1, int r2, int c2);
void MoveVideTo(int ny, int nx);
void DessinerTitre(int x, int y, char *texte);
void DessinerFondDegrade(void);
void MenuOptions(void);
void MenuPrincipal(void);
void MenuAide(void);

/* ---------- Fonctions de dessin et utilitaires (C89) ---------- */

void DessinerFondDegrade(void)
{
    int i;
    int hauteur = FENETRE_HAUTEUR;
    int largeur = FENETRE_LARGEUR;
    int steps = 200;
    
    for (i = 0; i < steps; i++) {
        int gray_level = 200 + (55 * i / steps); 
        ChoisirCouleurDessin(CouleurParNom("lavender"));
        RemplirRectangle(0, (i * hauteur) / steps, largeur, hauteur / steps + 1);
    }
}

void DessinerTitre(int x, int y, char *texte)
{
    ChoisirCouleurDessin(CouleurParNom("darkslateblue"));
    EcrireTexte(x + 3, y + 3, texte, 3);
    
    ChoisirCouleurDessin(CouleurParNom("gold"));
    EcrireTexte(x, y, texte, 3);
}

void DessinerBoutonGradient(int x, int y, int largeur, int hauteur, char *texte, int taillePolice, int selectionne)
{
    int largeurTexte, hauteurTexte, xTexte, yTexte;
    couleur couleurFond, couleurBordure, couleurTexte;
    
    largeurTexte = TailleChaineEcran(texte, taillePolice);
    hauteurTexte = TailleSupPolice(taillePolice) + TailleInfPolice(taillePolice);
    xTexte = x + (largeur - largeurTexte) / 2;
    yTexte = y + (hauteur - hauteurTexte) / 2 + TailleSupPolice(taillePolice);
    
    if (selectionne) {
        couleurFond = CouleurParNom("gold");
        couleurBordure = CouleurParNom("darkorange");
        couleurTexte = CouleurParNom("darkblue");
    } else {
        couleurFond = CouleurParNom("cornflowerblue");
        couleurBordure = CouleurParNom("darkblue");
        couleurTexte = CouleurParNom("white");
    }
    
    ChoisirCouleurDessin(CouleurParNom("gray50"));
    RemplirRectangle(x + SHADOW_OFFSET, y + SHADOW_OFFSET, largeur, hauteur);
    
    ChoisirCouleurDessin(couleurFond);
    RemplirRectangle(x, y, largeur, hauteur);
    
    ChoisirCouleurDessin(couleurBordure);
    DessinerRectangle(x, y, largeur, hauteur);
    DessinerRectangle(x + 1, y + 1, largeur - 2, hauteur - 2);
    
    ChoisirCouleurDessin(CouleurParNom("white"));
    DessinerSegment(x + 2, y + 2, x + largeur - 2, y + 2);
    DessinerSegment(x + 2, y + 2, x + 2, y + hauteur - 2);
    
    ChoisirCouleurDessin(CouleurParNom("gray20"));
    DessinerSegment(x + largeur - 2, y + 2, x + largeur - 2, y + hauteur - 2);
    DessinerSegment(x + 2, y + hauteur - 2, x + largeur - 2, y + hauteur - 2);
    
    ChoisirCouleurDessin(couleurTexte);
    EcrireTexte(xTexte, yTexte, texte, taillePolice);
}

void DessinerCadreDecore(int x, int y, int largeur, int hauteur, int epaisseur)
{
    int i;
    
    ChoisirCouleurDessin(CouleurParNom("gray60"));
    RemplirRectangle(x + epaisseur, y + epaisseur, largeur, hauteur);
    
    ChoisirCouleurDessin(CouleurParNom("white"));
    RemplirRectangle(x, y, largeur, hauteur);
    
    for (i = 0; i < epaisseur; i++) {
        if (i % 2 == 0) {
            ChoisirCouleurDessin(CouleurParNom("darkblue"));
        } else {
            ChoisirCouleurDessin(CouleurParNom("gold"));
        }
        DessinerRectangle(x + i, y + i, largeur - 2*i, hauteur - 2*i);
    }
}

int ClicDansBouton(int x, int y, int largeur, int hauteur)
{
    SourisPosition();
    return (_X >= x && _X <= x + largeur && _Y >= y && _Y <= y + hauteur);
}

void InitialiserTuiles(void)
{
    int i, j, index = 1;
    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            /* Case vide (0) = index 1 de l'image (haut-gauche) */
            /* POINT 4: tuile[0][0] est la case vide dans l'état résolu */
            tuiles[i][j] = (i == 0 && j == 0) ? 0 : index++;
        }
    }
    vide_x = 0;
    vide_y = 0;
}

void CopierCourantVersInitial(void)
{
    int i, j;
    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            initial_tuiles[i][j] = tuiles[i][j];
        }
    }
}

int Adjacent(int r1, int c1, int r2, int c2)
{
    /* La fonction abs() est dans math.h */
    return (r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1);
}

void MoveVideTo(int ny, int nx)
{
    int tmp = tuiles[ny][nx];
    tuiles[ny][nx] = tuiles[vide_y][vide_x];
    tuiles[vide_y][vide_x] = tmp;
    vide_x = nx;
    vide_y = ny;
}

void MelangerTuiles(int iterations)
{
    int step, dir, nx, ny, tries;
    
    /* srand doit être appelé une seule fois dans main */
    
    for (step = 0; step < iterations; step++) {
        tries = 0;
        do {
            dir = rand() % 4;
            nx = vide_x + (dir == 1) - (dir == 0); /* 0: Gauche, 1: Droite */
            ny = vide_y + (dir == 3) - (dir == 2); /* 2: Haut, 3: Bas */
            tries++;
        } while ((nx < 0 || nx >= colonnes || ny < 0 || ny >= lignes) && tries < 10);
        
        if (nx >= 0 && nx < colonnes && ny >= 0 && ny < lignes) {
            MoveVideTo(ny, nx);
        }
    }
    
    /* Ramène la case vide en haut à gauche (0,0) après le mélange pour garantir la jouabilité */
    /* POINT 4: S'assurer que le vide est à (0,0) pour l'état initial après mélange */
    while (vide_x != 0 || vide_y != 0) {
        if (vide_x > 0) MoveVideTo(vide_y, vide_x - 1);
        else if (vide_y > 0) MoveVideTo(vide_y - 1, vide_x);
        else break;
    }
    CopierCourantVersInitial();
}

void AfficherTuile(int i, int j)
{
    int val = tuiles[i][j];
    int x = j * (largeurCase + MARGE) + OFFSET_X;
    int y = i * (hauteurCase + MARGE) + OFFSET_Y;
    
    if (val == 0) {
        ChoisirCouleurDessin(CouleurParNom("lightgray"));
        RemplirRectangle(x, y, largeurCase, hauteurCase);
        
        ChoisirCouleurDessin(CouleurParNom("gray"));
        DessinerRectangle(x + 2, y + 2, largeurCase - 4, hauteurCase - 4);
    } else {
        /* tuile_idx = 0 correspond au morceau haut-gauche de l'image (val = 1) */
        int tuile_idx = val - 1; 
        
        ChoisirCouleurDessin(CouleurParNom("gray30"));
        RemplirRectangle(x + SHADOW_OFFSET, y + SHADOW_OFFSET, largeurCase, hauteurCase);
        
        ChargerImage(imageChoisie, x, y,
                     (tuile_idx % colonnes) * largeurCase,
                     (tuile_idx / colonnes) * hauteurCase,
                     largeurCase, hauteurCase);
        
        /* Effet 3D et bordure */
        ChoisirCouleurDessin(CouleurParNom("white"));
        DessinerSegment(x, y, x + largeurCase, y);
        DessinerSegment(x, y, x, y + hauteurCase);
        
        ChoisirCouleurDessin(CouleurParNom("gray20"));
        DessinerSegment(x + largeurCase, y, x + largeurCase, y + hauteurCase);
        DessinerSegment(x, y + hauteurCase, x + largeurCase, y + hauteurCase);
        
        ChoisirCouleurDessin(CouleurParNom("navy"));
        DessinerRectangle(x, y, largeurCase, hauteurCase);
    }
}

void AfficherTuiles(void)
{
    int i, j;
    
    DessinerCadreDecore(OFFSET_X - 10, OFFSET_Y - 10, 
                        colonnes * (largeurCase + MARGE) + 20,
                        lignes * (hauteurCase + MARGE) + 20, 3);
    
    for (i = 0; i < lignes; i++) {
        for (j = 0; j < colonnes; j++) {
            AfficherTuile(i, j);
        }
    }
}

void DessinerStatutJeu(void)
{
    DessinerCadreDecore(40, 10, FENETRE_LARGEUR - 80, 50, 2);
    
    ChoisirCouleurDessin(CouleurParNom("navy"));
    EcrireTexte(60, 40, messageStatut, 2);
}

int VerifierVictoire(void)
{
    int i, j, attendu = 1;
    /* POINT 4: Condition de victoire: case (0,0) est la case vide (0) */
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

void GererClicJeu(int click_x, int click_y, int *besoinDeReafficherJeu)
{
    int tuile_j, tuile_i, x_min, y_min, x_max, y_max, tmp;
    int old_vide_x, old_vide_y;

    *besoinDeReafficherJeu = 0;
    if (jeuTermine) return;

    tuile_j = (click_x - OFFSET_X) / (largeurCase + MARGE);
    tuile_i = (click_y - OFFSET_Y) / (hauteurCase + MARGE);

    if (tuile_i < 0 || tuile_i >= lignes || tuile_j < 0 || tuile_j >= colonnes) return;

    x_min = OFFSET_X + tuile_j * (largeurCase + MARGE);
    y_min = OFFSET_Y + tuile_i * (hauteurCase + MARGE);
    x_max = x_min + largeurCase;
    y_max = y_min + hauteurCase;

    if (click_x < x_min || click_x > x_max || click_y < y_min || click_y > y_max) return;

    if (Adjacent(tuile_i, tuile_j, vide_y, vide_x)) {
        old_vide_x = vide_x;
        old_vide_y = vide_y;
        
        tmp = tuiles[tuile_i][tuile_j];
        tuiles[tuile_i][tuile_j] = 0;
        tuiles[vide_y][vide_x] = tmp;
        vide_x = tuile_j;
        vide_y = tuile_i;
        
        /* Réafficher seulement les tuiles déplacées */
        AfficherTuile(old_vide_y, old_vide_x);
        AfficherTuile(vide_y, vide_x);
        
        nombreCoups++;
        if (!jeuTermine) {
            sprintf(messageStatut, "Moves: %d", nombreCoups);
            DessinerStatutJeu();
        }
    }
}

void GererMouvementClavier(KeySym key, int *besoinDeReafficherJeu)
{
    int tuile_x = -1, tuile_y = -1;
    int old_vide_x, old_vide_y;
    
    *besoinDeReafficherJeu = 0;
    if (jeuTermine) return;

    /* Détermine la tuile à déplacer (adjacente à la case vide) */
    if (key == XK_Left) { tuile_x = vide_x + 1; tuile_y = vide_y; }
    else if (key == XK_Right) { tuile_x = vide_x - 1; tuile_y = vide_y; }
    else if (key == XK_Up) { tuile_x = vide_x; tuile_y = vide_y + 1; }
    else if (key == XK_Down) { tuile_x = vide_x; tuile_y = vide_y - 1; }

    if (tuile_x >= 0 && tuile_x < colonnes && tuile_y >= 0 && tuile_y < lignes) {
        old_vide_x = vide_x;
        old_vide_y = vide_y;
        
        MoveVideTo(tuile_y, tuile_x);
        
        /* Réafficher seulement les tuiles déplacées */
        AfficherTuile(old_vide_y, old_vide_x);
        AfficherTuile(vide_y, vide_x);
        
        nombreCoups++;
        if (!jeuTermine) {
            sprintf(messageStatut, "Moves: %d", nombreCoups);
            DessinerStatutJeu();
        }
    }
}

void RedessinerSpinners(void)
{
    char choixTexte[20];
    int cadre_x = 480;
    int cadre_largeur = 300;
    int bouton_larg = 30, bouton_haut = 30;
    
    ChoisirCouleurDessin(CouleurParNom("lavender"));
    RemplirRectangle(cadre_x, 220, cadre_largeur, 50);
    
    ChoisirCouleurDessin(CouleurParNom("darkblue"));
    EcrireTexte(500, 250, "COLUMNS (C):", 2);
    DessinerBoutonGradient(690, 230, bouton_larg, bouton_haut, "-", 2, menuSelection == 0);
    ChoisirCouleurDessin(CouleurParNom("crimson"));
    sprintf(choixTexte, "%d", colonnes);
    EcrireTexte(650, 250, choixTexte, 2);
    DessinerBoutonGradient(730, 230, bouton_larg, bouton_haut, "+", 2, menuSelection == 1);
    
    ChoisirCouleurDessin(CouleurParNom("lavender"));
    RemplirRectangle(cadre_x, 320, cadre_largeur, 50);
    
    ChoisirCouleurDessin(CouleurParNom("darkblue"));
    EcrireTexte(500, 350, "ROWS (L):", 2);
    DessinerBoutonGradient(690, 330, bouton_larg, bouton_haut, "-", 2, menuSelection == 2);
    ChoisirCouleurDessin(CouleurParNom("crimson"));
    sprintf(choixTexte, "%d", lignes);
    EcrireTexte(650, 350, choixTexte, 2);
    DessinerBoutonGradient(730, 330, bouton_larg, bouton_haut, "+", 2, menuSelection == 3);
}

/* ---------- Boucle de jeu et fonctions de menu ---------- */

void BoucleJeu(void)
{
    int quitter = 0, besoinDeReafficherJeu;
    int victoire, i, j;
    KeySym touchePressee = 0;
    int bouton_menu_x = 1200;
    int bouton_menu_y[3] = {100, 200, 300};
    int bouton_menu_largeur = 200, bouton_menu_hauteur = 50;
    int clicDetecte = 0;
    int image_idx = 0;

    if (!partieLancee) {
        for (i = 0; i < 3; i++) {
            if (strcmp(imageChoisie, images[i].path) == 0) {
                image_idx = i;
                break;
            }
        }
        /* Recalculer la taille des cases en fonction de l'image et de la grille */
        largeurCase = images[image_idx].width / colonnes;
        hauteurCase = images[image_idx].height / lignes;

        InitialiserTuiles();
        MelangerTuiles(300);
        CopierCourantVersInitial();
        nombreCoups = 0;
        jeuTermine = 0;
        sprintf(messageStatut, "Moves: %d", nombreCoups);
        partieLancee = 1;
    }

    besoinDeReafficherJeu = 1;
    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficherJeu) {
            DessinerFondDegrade();
            
            DessinerTitre(630, 60, "TAQUIN PUZZLE");
            
            AfficherTuiles();
            DessinerStatutJeu();
            
            DessinerBoutonGradient(bouton_menu_x, bouton_menu_y[0], bouton_menu_largeur, bouton_menu_hauteur, "MENU (M)", 2, 0);
            DessinerBoutonGradient(bouton_menu_x, bouton_menu_y[1], bouton_menu_largeur, bouton_menu_hauteur, "SHUFFLE (S)", 2, 0);
            DessinerBoutonGradient(bouton_menu_x, bouton_menu_y[2], bouton_menu_largeur, bouton_menu_hauteur, "RESET (R)", 2, 0);
            
            ChoisirCouleurDessin(CouleurParNom("darkslategray"));
            EcrireTexte(1200, 380, "Arrows: Move tiles", 2);
            EcrireTexte(1200, 410, "M/S/R: Menu actions", 2);
            
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
        
        /* Attente passive d'une interaction */
        while (!clicDetecte && !ToucheEnAttente()) {
            if (SourisCliquee()) clicDetecte = 1;
        }
        if (ToucheEnAttente()) touchePressee = Touche();

        if (clicDetecte) {
            SourisPosition();
            if (ClicDansBouton(bouton_menu_x, bouton_menu_y[0], bouton_menu_largeur, bouton_menu_hauteur)) touchePressee = 'm';
            else if (ClicDansBouton(bouton_menu_x, bouton_menu_y[1], bouton_menu_largeur, bouton_menu_hauteur)) touchePressee = 's';
            else if (ClicDansBouton(bouton_menu_x, bouton_menu_y[2], bouton_menu_largeur, bouton_menu_hauteur)) touchePressee = 'r';
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
            sprintf(messageStatut, "Shuffled! (Moves: %d)", nombreCoups);
            besoinDeReafficherJeu = 1;
        } else if (touchePressee == 'r' || touchePressee == 'R') {
            for (i = 0; i < lignes; i++) {
                for (j = 0; j < colonnes; j++) {
                    tuiles[i][j] = initial_tuiles[i][j];
                    if (initial_tuiles[i][j] == 0) {
                        vide_x = j;
                        vide_y = i;
                    }
                }
            }
            nombreCoups = 0;
            jeuTermine = 0;
            sprintf(messageStatut, "Reset (Moves: %d)", nombreCoups);
            besoinDeReafficherJeu = 1;
        } else if (touchePressee == XK_Left || touchePressee == XK_Right || 
                   touchePressee == XK_Up || touchePressee == XK_Down) {
            GererMouvementClavier(touchePressee, &besoinDeReafficherJeu);
        } else if (clicDetecte) {
            GererClicJeu(_X, _Y, &besoinDeReafficherJeu);
        }

        if (ToucheEnAttente()) Touche(); /* Vider le buffer clavier */
    }
    
    if (jeuTermine) MenuFinJeu();
}

void MenuFinJeu(void)
{
    int quitter = 0, besoinDeReafficher = 1;
    char scoreTexte[100];
    KeySym touchePressee = 0;
    int bouton_x[2] = {450, 750};
    int bouton_y = 450;
    int select_btn = -1;
    
    menuSelection = 0;
    menuMaxItems = 2;

    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficher) {
            EffacerEcran(CouleurParNom("honeydew"));
            
            ChoisirCouleurDessin(CouleurParNom("forestgreen"));
            EcrireTexte(453, 153, "VICTORY!", 3);
            ChoisirCouleurDessin(CouleurParNom("gold"));
            EcrireTexte(450, 150, "VICTORY!", 3);
            
            sprintf(scoreTexte, "Solved in %d moves!", nombreCoups);
            ChoisirCouleurDessin(CouleurParNom("darkgreen"));
            EcrireTexte(600, 270, scoreTexte, 2);
            
            DessinerBoutonGradient(bouton_x[0], bouton_y, 200, 50, "MENU (M)", 2, menuSelection == 0);
            DessinerBoutonGradient(bouton_x[1], bouton_y, 200, 50, "QUIT (Q)", 2, menuSelection == 1);
            
            ChoisirCouleurDessin(CouleurParNom("darkgray"));
            EcrireTexte(600, 550, "LEFT/RIGHT or M/Q", 2);
            EcrireTexte(600, 580, "ENTER to select", 2);
            
            besoinDeReafficher = 0;
        }

        touchePressee = 0;
        select_btn = -1;
        while (!SourisCliquee() && !ToucheEnAttente()) {}
        if (ToucheEnAttente()) touchePressee = Touche();
        
        SourisPosition();

        if (touchePressee == XK_Left || touchePressee == 'm' || touchePressee == 'M') {
            menuSelection = 0;
            besoinDeReafficher = 1;
        } else if (touchePressee == XK_Right || touchePressee == 'q' || touchePressee == 'Q') {
            menuSelection = 1;
            besoinDeReafficher = 1;
        }
        
        if (ClicDansBouton(bouton_x[0], bouton_y, 200, 50)) select_btn = 0;
        else if (ClicDansBouton(bouton_x[1], bouton_y, 200, 50)) select_btn = 1;
        else if (touchePressee == XK_Return || touchePressee == XK_KP_Enter) select_btn = menuSelection;

        if (select_btn == 0) {
            quitter = 1;
        } else if (select_btn == 1) {
            jeuDoitQuitter = 1;
            quitter = 1;
        }

        if (ToucheEnAttente()) Touche();
    }
}

void MenuOptions(void)
{
    /* Déclarations déplacées au début du bloc pour respecter C90 */
    int quitter = 0, besoinDeReafficher = 1;
    KeySym touchePressee = 0;
    int btn_hauteur = 40;
    int btn_largeur_fleche = 50; 
    int image_x = 250, image_y = 520; 
    int select_btn = -1;
    int img_w, img_h;
    int preview_width = 0, preview_height = 0;
    int preview_x, preview_y;
    float ratio_w;
    float ratio_h;
    float scale_factor;
    
    menuSelection = 0;
    menuMaxItems = 4;

    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficher) {
            EffacerEcran(CouleurParNom("lavender"));
            DessinerTitre(550, 80, "OPTIONS");
            DessinerTitre(550, 480, "IMAGE SELECTION");

            RedessinerSpinners();

            /* --- LOGIQUE D'AFFICHAGE DE PRÉVISUALISATION ENTIÈRE --- */
            img_w = images[imageSelection].width;
            img_h = images[imageSelection].height;
            preview_width = img_w;
            preview_height = img_h;

            /* Calculer la nouvelle taille pour maintenir le ratio (max MAX_PREVIEW_SIZE) */
            if (img_w > MAX_PREVIEW_SIZE || img_h > MAX_PREVIEW_SIZE) {
                ratio_w = (float)MAX_PREVIEW_SIZE / img_w;
                ratio_h = (float)MAX_PREVIEW_SIZE / img_h;
                scale_factor = (ratio_w < ratio_h) ? ratio_w : ratio_h;
                
                preview_width = (int)(img_w * scale_factor);
                preview_height = (int)(img_h * scale_factor);
            }

            /* Centrage de l'image dans le cadre 200x200 (virtuel) à gauche */
            preview_x = image_x + (MAX_PREVIEW_SIZE - preview_width) / 2;
            preview_y = image_y + (MAX_PREVIEW_SIZE - preview_height) / 2;

            /* Affichage du titre au-dessus de l'image */
            ChoisirCouleurDessin(CouleurParNom("darkblue"));
            EcrireTexte(image_x + MAX_PREVIEW_SIZE / 2 - TailleChaineEcran(images[imageSelection].name, 2) / 2, image_y - 30, images[imageSelection].name, 2);

            /* Dessin du cadre fixe 200x200 */
            DessinerCadreDecore(image_x - 5, image_y - 5, MAX_PREVIEW_SIZE + 10, MAX_PREVIEW_SIZE + 10, 2);
            
            /* Charger l'image entière redimensionnée */
            ChargerImage(images[imageSelection].path, preview_x, preview_y, 
                         0, 0, preview_width, preview_height);

            /* Repositionnement des boutons < et > à côté du cadre 200x200 */
            DessinerBoutonGradient(image_x - 50, image_y + MAX_PREVIEW_SIZE / 2 - btn_hauteur/2, btn_largeur_fleche, btn_hauteur, "<", 2, menuSelection == menuMaxItems);
            DessinerBoutonGradient(image_x + MAX_PREVIEW_SIZE + 5, image_y + MAX_PREVIEW_SIZE / 2 - btn_hauteur/2, btn_largeur_fleche, btn_hauteur, ">", 2, menuSelection == menuMaxItems + 1);

            /* POINT 1: Bouton BACK (E) */
            DessinerBoutonGradient(600, 680, 200, 50, "BACK (E)", 2, menuSelection == menuMaxItems + 2);

            /* Instructions clavier mises à jour */
            ChoisirCouleurDessin(CouleurParNom("darkslategray"));
            EcrireTexte(800, 250, "C: Columns (+) - Cyclage 8->3", 2);
            EcrireTexte(800, 350, "L: Rows (+) - Cyclage 8->3", 2);
            EcrireTexte(800, 570, "Arrows: Navigate", 2);
            EcrireTexte(800, 600, "ENTER/E: Select/Back", 2);


            besoinDeReafficher = 0;
        }

        touchePressee = 0;
        select_btn = -1;
        while (!SourisCliquee() && !ToucheEnAttente()) {}
        if (ToucheEnAttente()) touchePressee = Touche();

        SourisPosition();
        
        /* Clic de la souris */
        if (ClicDansBouton(730, 230, 30, 30)) select_btn = 1; /* Colonnes + */
        else if (ClicDansBouton(690, 230, 30, 30)) select_btn = 0; /* Colonnes - */
        else if (ClicDansBouton(730, 330, 30, 30)) select_btn = 3; /* Lignes + */
        else if (ClicDansBouton(690, 330, 30, 30)) select_btn = 2; /* Lignes - */
        else if (ClicDansBouton(image_x - 50, image_y + MAX_PREVIEW_SIZE / 2 - btn_hauteur/2, btn_largeur_fleche, btn_hauteur)) select_btn = menuMaxItems; /* Image < */
        else if (ClicDansBouton(image_x + MAX_PREVIEW_SIZE + 5, image_y + MAX_PREVIEW_SIZE / 2 - btn_hauteur/2, btn_largeur_fleche, btn_hauteur)) select_btn = menuMaxItems + 1; /* Image > */
        else if (ClicDansBouton(600, 680, 200, 50)) select_btn = menuMaxItems + 2; /* BACK (E) */

        /* Clavier étendu */
        if (touchePressee == XK_Up) {
            if (menuSelection == 0 || menuSelection == 1) menuSelection = menuMaxItems + 2; 
            else if (menuSelection == 2 || menuSelection == 3) menuSelection = 0; 
            else if (menuSelection == menuMaxItems || menuSelection == menuMaxItems + 1) menuSelection = 2; 
            else if (menuSelection == menuMaxItems + 2) menuSelection = menuMaxItems + 1; 
            besoinDeReafficher = 1;
        } else if (touchePressee == XK_Down) {
            if (menuSelection == 0 || menuSelection == 1) menuSelection = 2; 
            else if (menuSelection == 2 || menuSelection == 3) menuSelection = menuMaxItems; 
            else if (menuSelection == menuMaxItems || menuSelection == menuMaxItems + 1) menuSelection = menuMaxItems + 2; 
            else if (menuSelection == menuMaxItems + 2) menuSelection = 0; 
            besoinDeReafficher = 1;
        } else if (touchePressee == XK_Left) {
            if (menuSelection == 1) menuSelection = 0;
            else if (menuSelection == 3) menuSelection = 2;
            else if (menuSelection == menuMaxItems + 1) menuSelection = menuMaxItems;
            besoinDeReafficher = 1;
        } else if (touchePressee == XK_Right) {
            if (menuSelection == 0) menuSelection = 1;
            else if (menuSelection == 2) menuSelection = 3;
            else if (menuSelection == menuMaxItems) menuSelection = menuMaxItems + 1;
            besoinDeReafficher = 1;
        } 
        /* POINT 3: C/L pour Colonnes/Lignes et cyclage 8->3 */
        else if (touchePressee == 'c' || touchePressee == 'C') { select_btn = 1; } /* C pour Colonnes+ */
        else if (touchePressee == 'l' || touchePressee == 'L') { select_btn = 3; } /* L pour Lignes+ */
        else if (touchePressee == 'e' || touchePressee == 'E') { select_btn = menuMaxItems + 2; } /* E pour Entrée/BACK */
        else if (touchePressee == XK_Return || touchePressee == XK_KP_Enter) {
            select_btn = menuSelection;
        }

        /* Action (avec cyclage MIN_DIM <-> MAX_DIM) */
        if (select_btn == 1) { 
            colonnes = (colonnes == MAX_DIM) ? MIN_DIM : colonnes + 1; /* Cyclage: 8 -> 3 */
            besoinDeReafficher = 1; 
        }
        else if (select_btn == 0 && colonnes > MIN_DIM) { colonnes--; besoinDeReafficher = 1; }
        else if (select_btn == 3) { 
            lignes = (lignes == MAX_DIM) ? MIN_DIM : lignes + 1; /* Cyclage: 8 -> 3 */
            besoinDeReafficher = 1; 
        }
        else if (select_btn == 2 && lignes > MIN_DIM) { lignes--; besoinDeReafficher = 1; }
        else if (select_btn == menuMaxItems) {
            imageSelection = (imageSelection - 1 + 3) % 3;
            strcpy(imageChoisie, images[imageSelection].path);
            besoinDeReafficher = 1;
        } else if (select_btn == menuMaxItems + 1) {
            imageSelection = (imageSelection + 1) % 3;
            strcpy(imageChoisie, images[imageSelection].path);
            besoinDeReafficher = 1;
        } else if (select_btn == menuMaxItems + 2) {
            quitter = 1;
        }

        if (ToucheEnAttente()) Touche();
    }
}


void MenuAide(void)
{
    int quitter = 0, besoinDeReafficher = 1;
    KeySym touchePressee = 0;
    int bouton_x = 650;
    int bouton_y = 650;
    
    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficher) {
            EffacerEcran(CouleurParNom("azure"));
            DessinerTitre(550, 80, "HELP / AIDE");
            
            ChoisirCouleurDessin(CouleurParNom("darkblue"));
            EcrireTexte(100, 180, "OBJECTIF DU JEU:", 2);
            ChoisirCouleurDessin(CouleurParNom("gray30"));
            EcrireTexte(100, 220, "Faites glisser les tuiles (pieces de l'image) pour reconstituer l'image originale.", 2);
            /* POINT 4: Clarification sur la case vide */
            EcrireTexte(100, 250, "Le coin haut-gauche (index 0,0) est la case vide (en gris) dans l'etat resolu.", 2);
            EcrireTexte(100, 280, "La victoire est atteinte lorsque l'image est complete et la case vide est en haut a gauche.", 2);

            ChoisirCouleurDessin(CouleurParNom("darkblue"));
            EcrireTexte(100, 380, "COMMANDES CLAVIER:", 2);
            ChoisirCouleurDessin(CouleurParNom("gray30"));
            EcrireTexte(100, 420, "Fleches Directionnelles: Deplacer la tuile adjacente a la case vide.", 2);
            EcrireTexte(100, 450, "M: Menu principal (Pendant le jeu).", 2);
            EcrireTexte(100, 480, "S: Melanger les tuiles (Shuffle) (Pendant le jeu).", 2);
            EcrireTexte(100, 510, "R: Reinitialiser la partie (Reset) (Pendant le jeu).", 2);

            ChoisirCouleurDessin(CouleurParNom("darkblue"));
            EcrireTexte(100, 580, "COMMANDES MENU OPTIONS:", 2);
            /* POINT 3: Instructions C/L mises à jour */
            EcrireTexte(100, 620, "C: Augmenter le nombre de Colonnes (Cyclique 3->8->3).", 2);
            EcrireTexte(100, 650, "L: Augmenter le nombre de Lignes (Cyclique 3->8->3).", 2);
            EcrireTexte(100, 680, "E / ENTER: Selectionner un bouton ou revenir au menu principal.", 2);
            
            /* POINT 1: Bouton BACK (E) */
            DessinerBoutonGradient(bouton_x, bouton_y, 200, 50, "BACK (E)", 2, 1);
            
            besoinDeReafficher = 0;
        }

        touchePressee = 0;
        while (!SourisCliquee() && !ToucheEnAttente()) {}
        if (ToucheEnAttente()) touchePressee = Touche();

        if (ClicDansBouton(bouton_x, bouton_y, 200, 50) || touchePressee == XK_Return || touchePressee == XK_KP_Enter || touchePressee == 'e' || touchePressee == 'E') {
            quitter = 1;
        }

        if (ToucheEnAttente()) Touche();
    }
}

void MenuPrincipal(void)
{
    int quitter = 0, besoinDeReafficher = 1;
    KeySym touchePressee = 0;
    int bouton_x = 650;
    int bouton_y[4] = {250, 350, 450, 550};
    int bouton_largeur = 200, bouton_hauteur = 60;
    int select_btn = -1;
    
    menuSelection = 0;
    menuMaxItems = 3;

    while (SourisCliquee()) {}
    while (ToucheEnAttente()) { Touche(); }

    while (!quitter) {
        if (besoinDeReafficher) {
            DessinerFondDegrade();
            DessinerTitre(550, 80, "TAQUIN PUZZLE");
            
            DessinerBoutonGradient(bouton_x, bouton_y[0], bouton_largeur, bouton_hauteur, "PLAY", 2, menuSelection == 0);
            DessinerBoutonGradient(bouton_x, bouton_y[1], bouton_largeur, bouton_hauteur, "OPTIONS", 2, menuSelection == 1);
            DessinerBoutonGradient(bouton_x, bouton_y[2], bouton_largeur, bouton_hauteur, "HELP", 2, menuSelection == 2);
            DessinerBoutonGradient(bouton_x, bouton_y[3], bouton_largeur, bouton_hauteur, "QUIT (Q)", 2, menuSelection == 3);

            ChoisirCouleurDessin(CouleurParNom("darkgray"));
            EcrireTexte(600, 650, "UP/DOWN, ENTER to select, Q to Quit", 2);
            
            besoinDeReafficher = 0;
        }

        touchePressee = 0;
        select_btn = -1;
        while (!SourisCliquee() && !ToucheEnAttente()) {}
        if (ToucheEnAttente()) touchePressee = Touche();

        SourisPosition();

        /* Clavier */
        if (touchePressee == XK_Up) {
            menuSelection = (menuSelection - 1 + menuMaxItems + 1) % (menuMaxItems + 1);
            besoinDeReafficher = 1;
        } else if (touchePressee == XK_Down) {
            menuSelection = (menuSelection + 1) % (menuMaxItems + 1);
            besoinDeReafficher = 1;
        } else if (touchePressee == 'q' || touchePressee == 'Q') {
             select_btn = 3; /* Quitter */
        } else if (touchePressee == XK_Return || touchePressee == XK_KP_Enter) {
            select_btn = menuSelection;
        }

        /* Clic de la souris */
        if (ClicDansBouton(bouton_x, bouton_y[0], bouton_largeur, bouton_hauteur)) select_btn = 0;
        else if (ClicDansBouton(bouton_x, bouton_y[1], bouton_largeur, bouton_hauteur)) select_btn = 1;
        else if (ClicDansBouton(bouton_x, bouton_y[2], bouton_largeur, bouton_hauteur)) select_btn = 2;
        else if (ClicDansBouton(bouton_x, bouton_y[3], bouton_largeur, bouton_hauteur)) select_btn = 3;

        /* Action */
        if (select_btn == 0) { 
            quitter = 1;
        } else if (select_btn == 1) { 
            MenuOptions();
            besoinDeReafficher = 1;
        } else if (select_btn == 2) { 
            MenuAide(); 
            besoinDeReafficher = 1;
        } else if (select_btn == 3) { 
            jeuDoitQuitter = 1;
            quitter = 1;
        }

        if (ToucheEnAttente()) Touche();
    }
}


int main(void)
{
    /* srand doit être appelé une seule fois dans main pour une bonne initialisation */
    srand((unsigned int)time(NULL)); 

    InitialiserGraphique();
    CreerFenetre(100, 100, FENETRE_LARGEUR, FENETRE_HAUTEUR);

    colonnes = MIN_DIM;
    lignes = MIN_DIM;
    strcpy(imageChoisie, images[0].path);
    imageSelection = 0;
    partieLancee = 0;
    jeuDoitQuitter = 0;
    
    while (SourisCliquee()) {} /* Vider le buffer de la souris */

    while (!jeuDoitQuitter) {
        
        MenuPrincipal();

        if (jeuDoitQuitter) break;

        BoucleJeu();

        /* BoucleJeu revient ici si l'utilisateur a gagné ou est revenu au menu */
    }
    
    FermerGraphique();
    return 0;
}