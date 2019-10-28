//BOLLON Hugo

// Header contenant nos types, includes, prototypes ect...
#include "tetris.h"

/**
 * Initialise la grille de jeu
 * 
 * @param jeu Grille de jeu
 * @return void
 */
void initialiseGrille(Grille jeu)
{
    for(int i = 0; i < HAUTEUR; ++i)
    {
        for(int j = 0; j < LARGEUR; ++j)
        {
            jeu[i][j] = ' ';
        }
    }
}

/**
 * Retourne le caractère présent dans une case de la grille 
 * 
 * @param jeu Grille de jeu
 * @param ligne Index de ligne
 * @param colone Index de colone
 * 
 * @return Le caractère contenu dans la case
 */
char lireCase(Grille jeu, int ligne, int colone)
{
    if((ligne <= HAUTEUR && ligne >= 0) && (colone <= LARGEUR && colone >= 0))
        return jeu[ligne][colone];
    else
    {
        printf("Erreur : Emplacement invalide pour lecture");
        return ' ';
    }
}

/**
 * Affiche une représentation graphique de la grille de jeu
 * 
 * @param jeu Grille de jeu
 * @return void
 */
void afficheGrille(Grille jeu)
{
    printf("\n\n");
    for(int i = HAUTEUR; --i >= 0;)
    {
        printf("%d ||", i);
        for(int j = 0; j < LARGEUR; ++j)
        {
            printf("%c", lireCase(jeu, i, j));
        }
        printf("||\n");
    }
    printf("  |||||||||||||||||||  \n");
    printf("    0         1        \n");
    printf("    012345678901234    \n\n");
}

/**
 * Crée les pièces coposant le jeu et les ajoute à un tableau
 * 
 * @param tabPiece Tableau contenant nos pièces
 * @return void
 */
void genererPieces(ListePieces tabPiece)
{
    tabPiece[0].id = 0;
    tabPiece[0].hauteur = 3;
    tabPiece[0].largeur = 2;
    tabPiece[0].forme[2] = "ll";
    tabPiece[0].forme[1] = " l";
    tabPiece[0].forme[0] = " l";

    tabPiece[1].id = 1;
    tabPiece[1].hauteur = 2;
    tabPiece[1].largeur = 3;
    tabPiece[1].forme[1] = "  l";
    tabPiece[1].forme[0] = "lll";

    tabPiece[2].id = 2;
    tabPiece[2].hauteur = 3;
    tabPiece[2].largeur = 2;
    tabPiece[2].forme[2] = "l ";
    tabPiece[2].forme[1] = "l ";
    tabPiece[2].forme[0] = "ll";

    tabPiece[3].id = 3;
    tabPiece[3].hauteur = 2;
    tabPiece[3].largeur = 3;
    tabPiece[3].forme[1] = "lll";
    tabPiece[3].forme[0] = "l  ";

    tabPiece[4].id = 4;
    tabPiece[4].hauteur = 4;
    tabPiece[4].largeur = 1;
    tabPiece[4].forme[3] = "|";
    tabPiece[4].forme[2] = "|";
    tabPiece[4].forme[1] = "|";
    tabPiece[4].forme[0] = "|";

    tabPiece[5].id = 5;
    tabPiece[5].hauteur = 1;
    tabPiece[5].largeur = 4;
    tabPiece[5].forme[0] = "||||";

    tabPiece[6].id = 6;
    tabPiece[6].hauteur = 3;
    tabPiece[6].largeur = 2;
    tabPiece[6].forme[2] = "# ";
    tabPiece[6].forme[1] = "##";
    tabPiece[6].forme[0] = " #";

    tabPiece[7].id = 7;
    tabPiece[7].hauteur = 2;
    tabPiece[7].largeur = 3;
    tabPiece[7].forme[1] = " ##";
    tabPiece[7].forme[0] = "## ";

    tabPiece[8].id = 8;
    tabPiece[8].hauteur = 2;
    tabPiece[8].largeur = 2;
    tabPiece[8].forme[1] = "OO";
    tabPiece[8].forme[0] = "OO";

    tabPiece[9].id = 9;
    tabPiece[9].hauteur = 2;
    tabPiece[9].largeur = 3;
    tabPiece[9].forme[1] = " @ ";
    tabPiece[9].forme[0] = "@@@";

    tabPiece[10].id = 10;
    tabPiece[10].hauteur = 3;
    tabPiece[10].largeur = 2;
    tabPiece[10].forme[2] = "@ ";
    tabPiece[10].forme[1] = "@@";
    tabPiece[10].forme[0] = "@ ";

    tabPiece[11].id = 11;
    tabPiece[11].hauteur = 2;
    tabPiece[11].largeur = 3;
    tabPiece[11].forme[1] = "@@@";
    tabPiece[11].forme[0] = " @ ";

    tabPiece[12].id = 12;
    tabPiece[12].hauteur = 3;
    tabPiece[12].largeur = 2;
    tabPiece[12].forme[2] = " @";
    tabPiece[12].forme[1] = "@@";
    tabPiece[12].forme[0] = " @";

    
}

/**
 * Ecrit dans une case de la grille de jeu un caractère
 * 
 * @param jeu Grille de jeu
 * @param ligne Index de ligne
 * @param col Index de colone 
 * @param elem Caractère à insérer
 * 
 * @return void
 */
void ecrireCase(Grille jeu, int ligne, int colone, char elem)
{
    if((ligne <= HAUTEUR && ligne >= 0) && (colone <= LARGEUR && colone >= 0))
    {
        printf("x : %d | y : %d\n", ligne, colone);
        jeu[ligne][colone] = elem;
    }
    else
    {
        printf("Erreur : Emplacement invalide pour écriture : x : %d | y : %d\n", ligne, colone);
    }
}

/**
 * Affiche une représentation de la pièce 
 * 
 * @param piece Une pièce
 * @return void
 */
void affichePiece(Piece piece)
{
    for(int i = piece.hauteur; --i >= 0;)
    {
        for (int j = 0; j < piece.largeur; ++j)
        {
                printf("%c", piece.forme[i][j]);
        }
        printf("\n");
    }
    printf("↑");
    for(int i = 1; i < piece.largeur; ++i)
    {
        printf(" ");
    }
}

/**
 * Fonction calculant la ligne minimale ou placer la pièce
 * On affine ensuite son résulat dans la fonction hauteurExacte
 * 
 * @param jeu Grille de jeu
 * @param colMin La colone ou l'utilisateur veut placer la pièce
 * @param colMax colMin + la largeur de la pièce 
 * 
 * @return Ligne minimale pour placer la pièce
 */
int hauteurPlat(Grille jeu, int colMin, int colMax)
{
    bool find = false;
    int ligne = 9;
    while(find == false)
    {
        bool empty = true;
        for(int i = colMin; i <= colMax; ++i)
        {
            if(jeu[ligne][i] != ' ')
                empty = false;
        }
        if(empty == false)
            find = true;
        else
            ligne--;
    }
    return ++ligne;
}

/**
 * Cette procédure écrit dans la grille la pièce à l'emplacement demandé
 * Elle utilse la fonction ecrireCase pour chaque caractère
 * 
 * @param jeu Grille de jeu
 * @param piece La pièce
 * @param col La colone où commencer le placement 
 * @param ligne La ligne où commencer le placement
 * 
 * @return void
 */ 
void ecrirePiece(Grille jeu, Piece piece, int col, int hauteur)
{
    int pX = 0;
    int pY = 0;
    for (int i = hauteur; i < hauteur + piece.hauteur; ++i)
    {
        for (int j = col; j < col + piece.largeur; ++j)
        {
            if(piece.forme[pX][pY] != ' ')
                ecrireCase(jeu, i, j, piece.forme[pX][pY]);
            pY++;
        }
        pY = 0;
        pX++;
    }
    
}

/**
 * Retourne un pièce aléatoire
 * 
 * @param tabPiece La liste es pièces disponibles
 * @return Une pièce
 */
Piece pieceAleatoire(ListePieces tabPiece)
{
    int alea = (rand() % NB_PIECES);
    return tabPiece[alea];
}

/**
 * Test si la pièce va dépassé du tableau 
 * 
 * @param jeu Grille de jeu
 * @param piece La piece
 * @param ligne La ligne où on veut la mettre
 * 
 * @return Un booléen 
 */
bool pieceDepasse(Grille jeu, Piece piece, int ligne)
{
    if(ligne + piece.hauteur > HAUTEUR)
        return true;
    else
        return false;
}

/**
 *  Supprime la ligne correspondant à l'index en paramètre et décalle le reste
 * 
 * @param jeu Grille de jeu
 * @return void
 */
void supprimerLigne(Grille jeu, int index)
{
    for(int i = index+1; i<HAUTEUR; ++i)
    {
        for(int j = 0; j<LARGEUR; ++j)
        {
           jeu[i-1][j] = jeu[i][j]; 
        }
    }
    for(int j = 0; j<LARGEUR; ++j)
    {
        jeu[HAUTEUR-1][j] = ' ';
    }
}

/**
 * Cette fonction est appelée à chaque fin de la boucle de jeu
 * Elle vérifie si il y a des lignes pleines a supprimer 
 * Elle les supprimes en appelant la fonction supprimerLigne et incrémente nbLignes
 * 
 * @param jeu Grille de jeu
 * @return Le nombre de lignes supprimées
 */
int nettoyer(Grille jeu)
{
    bool full;
    int nbLignes = 0;
    for(int i = 0; i<HAUTEUR; ++i)
    {
        full = true;
        for (int j = 0; j<LARGEUR; ++j)
        {
            if(jeu[i][j] == ' ')
                full = false;
        }
        if(full == true)
        {
            supprimerLigne(jeu, i);
            ++nbLignes;
        }
    }

    return nbLignes;
}

/**
 * Cette fonction permet d'affiner la ligne ou placer la pièce
 * Elle utilise le résultat de hauteurPlat
 * 
 * @param jeu Grille de jeu
 * @param col Input de l'utilisateur
 * @param Piece La piece
 * 
 * @return La hauteur exacte ou peut etre placé la pièce
 */
int hauteurExacte(Grille jeu, int col, Piece piece)
{
    int ligneMin = hauteurPlat(jeu, col, col + (piece.largeur -1));
    int ligne = ligneMin;
    for(int i = 0; i < piece.hauteur; ++i)
    {
        for (int j = 0; j < piece.largeur; ++j)
        {
            if(piece.forme[i][j] != ' ' && jeu[ligneMin-(i+1)][col+j] != ' ')
                return ligne;
        }
        if(ligne > 0)
            ligne--;
        else
            return ligne;
    }
    return ligne;
}

void main_console()
{
    // Cette fonction génère un seed pour la fonction rand()
    srand(time(NULL));

    Grille grilleJeu;
    ListePieces tabPieces;
    
    // Tableaux associatifs pour pouvoir tourner les pièces
    int rotD[NB_PIECES][2] = { {0,1},{1,2},{2,3},{3,0} , {4,5},{5,4} , {6,7},{7,6} , {8,8} , {9,10},{10,11},{11,12},{12,9} };
    int rotG[NB_PIECES][2] = { {0,3},{1,0},{2,1},{3,2} , {4,5},{5,4} , {6,7},{7,6} , {8,8} , {9,12},{10,9},{11,10},{12,11} };

    int etatJeu = 1;
    Piece randPiece;
    char userInput[8];
    bool inputValidation = false;

    initialiseGrille(grilleJeu);
    genererPieces(tabPieces);
    
    // Boucle de jeu
    while(etatJeu != 0)
    {
        printf("\n\n");
        randPiece = pieceAleatoire(tabPieces);
        affichePiece(randPiece);
        afficheGrille(grilleJeu);
        inputValidation = false;
        
        // Validation de l'input 
        while(inputValidation == false)
        {
            printf("\nVeuillez saisir le numéro de colone où vous voulez placer la pièce ou 'g'/'d' pour tourner la pièce : ");
            scanf("%7s", userInput);
            // Cas où le joueur veut tourner la pièce à droite 
            if(userInput[0] == 'd')
            {
                for (int i = 0; i < NB_PIECES; ++i)
                {
                    if(randPiece.id == rotD[i][0])
                    {
                        randPiece = tabPieces[rotD[i][1]];
                        break;
                    }
                }
                
                affichePiece(randPiece);
                afficheGrille(grilleJeu);
            }
            // Cas où le joueur veut tourner la pièce à gauche
            else if(userInput[0] == 'g')
            {
                for (int i = 0; i < NB_PIECES; ++i)
                {
                    if(randPiece.id == rotG[i][0])
                    {
                        randPiece = tabPieces[rotG[i][1]];
                        break;
                    }
                }

                affichePiece(randPiece);
                afficheGrille(grilleJeu);
            }
            // Si l'utilisateur à rentrer un valeur invalide on recommence la saisie avec un message d'erreur
            else if (atoi(userInput) < -1 || atoi(userInput) >= LARGEUR)
            {
                printf("\nErreur : Index de colone invalide\n");
            }
            // Si la saisie est valide on sort
            else
            {
                inputValidation = true;
            }
        }

        int colone;
        colone = atoi(userInput);

        // On arrète le jeu si la pièce dépasse ou si le joueur à entrer -1
        if(colone == -1 || pieceDepasse(grilleJeu, randPiece, hauteurExacte(grilleJeu, colone, randPiece)) == true)
        {
            etatJeu = 0;
        }

        ecrirePiece(grilleJeu, randPiece, colone, hauteurExacte(grilleJeu, colone, randPiece));
        nettoyer(grilleJeu);
    }

}