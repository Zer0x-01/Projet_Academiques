#ifndef TAQUIN_H
#define TAQUIN_H

/* Inclut <graph.h> et les types nécessaires comme KeySym */
#include <graph.h>

/* --- Constantes --- */
#define MIN_DIM 3
#define MAX_DIM 8
#define MARGE 3
#define OFFSET_X 50
#define OFFSET_Y 100

/* --- Variables Globales (Déclarées dans taquin.c) --- */
extern int tuiles[MAX_DIM][MAX_DIM];
extern int initial_tuiles[MAX_DIM][MAX_DIM];
extern int colonnes;
extern int lignes;
extern char imageChoisie[80];
extern int largeurCase, hauteurCase;
extern int vide_x, vide_y;
extern int partieLancee;
extern char messageStatut[200];
extern int nombreCoups;
extern int jeuTermine;
extern int jeuDoitQuitter;
extern int menuSelection;
extern int menuMaxItems;

/* --- Prototypes des Fonctions --- */

/* Fonctions utilitaires/graphiques */
void DessinerBouton(int x, int y, int largeur, int hauteur, char *texte, int taillePolice, int selectionne);
int ClicDansBouton(int x, int y, int largeur, int hauteur);
int EntreePressee(void);
int TouchePressee(KeySym cle);

/* Fonctions de logique du jeu (game.c) */
void InitialiserTuiles(void);
void CopierCourantVersInitial(void);
void MelangerTuiles(int iterations);
void AfficherTuiles(void);
void DessinerStatutJeu(void);
int VerifierVictoire(void);
void GererClicJeu(int click_x, int click_y, int *besoinDeReafficherJeu);
void GererMouvementClavier(KeySym key, int *besoinDeReafficherJeu);
void BoucleJeu(void); /* La boucle interne de jeu */

/* Fonctions de Menu et d'Orchestration */
void MenuFinJeu(void);

#endif /* TAQUIN_H */