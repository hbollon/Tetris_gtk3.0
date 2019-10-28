#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define HAUTEUR 10
#define LARGEUR 15
#define NB_PIECES 13
#define HAUTEUR_MAX_DES_PIECES 4

struct SPiece {
  int   id;
  int   hauteur;
  int   largeur;
  char* forme[ HAUTEUR_MAX_DES_PIECES ];
};
typedef struct SPiece Piece;

typedef char Grille[HAUTEUR][LARGEUR];
typedef Piece ListePieces[NB_PIECES];

extern void initialiseGrille(Grille jeu);
extern char lireCase(Grille jeu, int ligne, int colone);
extern void afficheGrille(Grille jeu);
extern void genererPieces(ListePieces tab);
extern void ecrireCase(Grille jeu, int ligne, int colone, char elem);
extern void affichePiece(Piece piece);
extern int hauteurPlat(Grille jeu, int colMin, int colMax);
extern void ecrirePiece(Grille jeu, Piece piece, int col, int hauteur);
extern Piece pieceAleatoire(ListePieces tabPiece);
extern bool pieceDepasse(Grille jeu, Piece piece, int ligne);
extern void supprimerLigne(Grille jeu, int index);
extern int nettoyer(Grille jeu);
extern int hauteurExacte( Grille g, int col_gauche, Piece piece );

#endif