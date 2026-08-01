#include "taquin.h"
#include <graph.h>
#include <stdio.h>
#include <string.h>
#include <X11/keysym.h>

/* Variables globales déclarées dans taquin.c, utilisées ici. */
extern int colonnes;
extern int lignes;
extern char imageChoisie[80];
extern int partieLancee;
extern int jeuDoitQuitter;
extern int menuSelection;
extern int menuMaxItems;

/* MAIN : menu principal et navigation */
int main(void)
{
    /* ... Le corps complet de la fonction main */
    int menuActif;
    int x;
    int y;
    int besoinDeReafficher;
    int ancienMenuActif;
    int ancien_colonnes;
    int ancien_lignes;
    char choixTexte[120];
    int clicTraite;
    int entreeValidee;
    KeySym touchePressee;
    int selectionFermeture; 

    InitialiserGraphique();
    CreerFenetre(100, 100, 1500, 800);

    colonnes = 0;
    lignes = 0;
    strcpy(imageChoisie, "");
    menuActif = 1;
    besoinDeReafficher = 1;
    menuSelection = 0;

    while (1) {
        ancienMenuActif = menuActif;
        ancien_colonnes = colonnes;
        ancien_lignes = lignes;
        clicTraite = 0;
        selectionFermeture = -1; 
        
        if (jeuDoitQuitter) {
            break;
        }
        
        touchePressee = 0;
        if (!SourisCliquee() && ToucheEnAttente()) {
            touchePressee = Touche();
        }

        if (besoinDeReafficher) {
            EffacerEcran(CouleurParNom("lightgray"));
            if (menuActif == 1) {
                menuMaxItems = 1;
                
                EcrireTexte(700, 50, "Main Menu", 2);
                DessinerBouton(650, 300, 200, 50, "PLAY", 2, menuSelection == 0);
                
                ChoisirCouleurDessin(CouleurParNom("darkgray"));
                EcrireTexte(870, 315, "(or press ENTER)", 2);
                ChoisirCouleurDessin(CouleurParNom("black"));
                
            } else if (menuActif == 2) {
                menuMaxItems = 4;
                
                EcrireTexte(700, 50, "Select an Image", 2);
                DessinerBouton(650, 200, 200, 50, "Raton laveur - 600x400", 2, menuSelection == 0);
                DessinerBouton(650, 300, 200, 50, "Chien - 300x400", 2, menuSelection == 1);
                DessinerBouton(650, 400, 200, 50, "Dromadaire - 400x400", 2, menuSelection == 2);
                DessinerBouton(650, 500, 200, 50, "BACK", 2, menuSelection == 3);
                
                ChoisirCouleurDessin(CouleurParNom("darkgray"));
                EcrireTexte(870, 215, "(Key 1)", 2);
                EcrireTexte(870, 315, "(Key 2)", 2);
                EcrireTexte(870, 415, "(Key 3)", 2);
                EcrireTexte(870, 515, "(Key ESCAPE)", 2);
                EcrireTexte(100, 500, "Navigate: UP/DOWN", 2);
                EcrireTexte(100, 530, "Select: ENTER", 2);
                ChoisirCouleurDessin(CouleurParNom("black"));

            } else if (menuActif == 3) {
                menuMaxItems = 2;
                
                EcrireTexte(700, 50, "Choose Dimensions (3-8)", 2);
                
                ChoisirCouleurDessin(CouleurParNom("darkgray"));
                sprintf(choixTexte, "Image selection: %s", imageChoisie);
                EcrireTexte(500, 150, choixTexte, 2);
                
                ChoisirCouleurDessin(CouleurParNom("black"));
                EcrireTexte(500, 250, "COLONNES:", 2);
                DessinerBouton(690, 230, 30, 30, "v", 2, 0); 
                ChoisirCouleurDessin(CouleurParNom("darkgreen"));
                sprintf(choixTexte, "%d", colonnes);
                EcrireTexte(650, 250, choixTexte, 2);
                DessinerBouton(730, 230, 30, 30, "^", 2, 0); 
                
                ChoisirCouleurDessin(CouleurParNom("black"));
                EcrireTexte(500, 350, "LIGNES:", 2);
                DessinerBouton(690, 330, 30, 30, "v", 2, 0); 
                ChoisirCouleurDessin(CouleurParNom("darkgreen"));
                sprintf(choixTexte, "%d", lignes);
                EcrireTexte(650, 350, choixTexte, 2);
                DessinerBouton(730, 330, 30, 30, "^", 2, 0); 

                DessinerBouton(500, 750, 200, 50, "BACK (ESC)", 2, menuSelection == 0);
                DessinerBouton(800, 750, 200, 50, "START GAME (ENTER)", 2, menuSelection == 1);
                
                ChoisirCouleurDessin(CouleurParNom("darkgray"));
                EcrireTexte(770, 250, "(Key C: Change Col)", 2);
                EcrireTexte(770, 350, "(Key L: Change Lin)", 2);
                EcrireTexte(100, 750, "Navigate: LEFT/RIGHT", 2);
                ChoisirCouleurDessin(CouleurParNom("black"));
            }
            besoinDeReafficher = 0;
        }
        
        if (touchePressee == 0) {
            while (!SourisCliquee() && !ToucheEnAttente()) {}
            if (ToucheEnAttente()) {
                 touchePressee = Touche();
            }
        }
        
        SourisPosition();
        x = _X;
        y = _Y;

        entreeValidee = (touchePressee == XK_Return || touchePressee == XK_KP_Enter);

        if (menuActif == 1) {
            if (ClicDansBouton(650, 300, 200, 50) || entreeValidee) { 
                colonnes = MIN_DIM;
                lignes = MIN_DIM;
                menuActif = 2; 
                menuSelection = 0;
                clicTraite = 1; 
            }
        } else if (menuActif == 2) {
            if (touchePressee == XK_Up) {
                menuSelection = (menuSelection - 1 + menuMaxItems) % menuMaxItems;
                clicTraite = 1;
            } else if (touchePressee == XK_Down) {
                menuSelection = (menuSelection + 1) % menuMaxItems;
                clicTraite = 1;
            }
            
            if (ClicDansBouton(650, 200, 200, 50) || touchePressee == XK_1 || (entreeValidee && menuSelection == 0)) { strcpy(imageChoisie, "Images/Img1.jpg"); selectionFermeture = 0; }
            else if (ClicDansBouton(650, 300, 200, 50) || touchePressee == XK_2 || (entreeValidee && menuSelection == 1)) { strcpy(imageChoisie, "Images/Img2.jpg"); selectionFermeture = 1; }
            else if (ClicDansBouton(650, 400, 200, 50) || touchePressee == XK_3 || (entreeValidee && menuSelection == 2)) { strcpy(imageChoisie, "Images/Img3.jpg"); selectionFermeture = 2; }
            else if (ClicDansBouton(650, 500, 200, 50) || touchePressee == XK_Escape || (entreeValidee && menuSelection == 3)) { selectionFermeture = 3; }
            
            if (selectionFermeture != -1) {
                if (selectionFermeture < 3) {
                    menuActif = 3; 
                    menuSelection = 1;
                } else {
                    menuActif = 1;
                }
                clicTraite = 1;
            }
            
        } else if (menuActif == 3) {
            if (touchePressee == XK_Left) {
                menuSelection = 0;
                clicTraite = 1;
            } else if (touchePressee == XK_Right) {
                menuSelection = 1;
                clicTraite = 1;
            }
            
            if (ClicDansBouton(730, 230, 30, 30) || touchePressee == XK_c) {
                colonnes = (colonnes % MAX_DIM) + 1;
                if (colonnes < MIN_DIM) colonnes = MIN_DIM;
                clicTraite = 1;
            }
            if (ClicDansBouton(690, 230, 30, 30)) {
                colonnes--;
                if (colonnes < MIN_DIM) colonnes = MAX_DIM;
                clicTraite = 1;
            }
            
            if (ClicDansBouton(730, 330, 30, 30) || touchePressee == XK_l) {
                lignes = (lignes % MAX_DIM) + 1;
                if (lignes < MIN_DIM) lignes = MIN_DIM;
                clicTraite = 1;
            }
            if (ClicDansBouton(690, 330, 30, 30)) {
                lignes--;
                if (lignes < MIN_DIM) lignes = MAX_DIM;
                clicTraite = 1;
            }
            
            if (ClicDansBouton(500, 750, 200, 50) || touchePressee == XK_Escape || (entreeValidee && menuSelection == 0)) {
                menuActif = 2;
                menuSelection = 0;
                clicTraite = 1;
            }

            if (ClicDansBouton(800, 750, 200, 50) || (entreeValidee && menuSelection == 1)) { 
                if (colonnes >= MIN_DIM && lignes >= MIN_DIM) {
                    partieLancee = 0;
                    BoucleJeu();
                    
                    if (jeuDoitQuitter) {
                        break;
                    }
                    
                    menuActif = 1;
                    menuSelection = 0;
                    besoinDeReafficher = 1;
                } else {
                    besoinDeReafficher = 1;
                }
                clicTraite = 1;
            }
        }

        if (ancienMenuActif != menuActif || (menuActif == 3 && (ancien_colonnes != colonnes || ancien_lignes != lignes)) || clicTraite) {
            besoinDeReafficher = 1;
        }

        while (SourisCliquee()) {}
        if (ToucheEnAttente()) {
             Touche();
        }
    }

    FermerGraphique();
    return 0;
}