// BOLLON Hugo
#include <math.h>
#include <gtk/gtk.h>
#include "tetris.h"

#define TAILLE_CARRE 16

typedef struct {
  Grille g;
  Piece tab[NB_PIECES];
  Piece piece; // la piece "en cours" que l'on cherche à placer.
  int col; // la colonne actuelle pour la pièce que l'on cherche à caser.
  int score; // le score
  int delay; // le delay
  bool etat;
} Jeu;

/**
 * Structure servant à rassembler un pointeur vers le jeu et un vers le canvas
 * Cela pour pouvoir passer les 2 à nos signaux gauche, droite, bas, newGame 
 */
typedef struct {
    GtkWidget *drawArea;
    Jeu *jeu;
} DrawSignal;

// Labels score et delai
GtkWidget* labelDelay;
GtkWidget* labelScore;
// Variable globale contenant l'id du timer pour pouvoir le supprimer à tout moment
int idTimeOut;

gint delayUpdate(gpointer data);
void scoreUpdate(Jeu* ptrJeu);

/**
 * Tire une pièce aléatoire dans la liste de pièces 
 * Et assigne au membre de la structure de jeu 'col' la colonne ou la placer pour l'aperçu
 * 
 * @param jeu : Pointeur vers la structure de jeu 
 */
void nouvellePiece(Jeu *jeu)
{
    jeu->piece = pieceAleatoire(jeu->tab);
    jeu->col = (LARGEUR-jeu->piece.largeur)/2;
}

/**
 * Action pour le bouton gauche 
 * Déplace la pièce vers la gauche en enlevant 1 à col si col est supérieur à 0
 * Redessine ensuite le canvas
 * 
 * @param widget
 * @param data 
 * @return gboolean 
 */
gboolean Gauche(GtkWidget *widget, gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;
    if(drawStr->jeu->col > 0)
    {
        drawStr->jeu->col -= 1;
        gtk_widget_queue_draw(drawStr->drawArea); 
    }
    return TRUE; // Tout s'est bien passé
}

/**
 * Action pour le bouton droite 
 * Déplace la pièce vers la droite en ajoutant 1 à col si la pièce ne dépasse pas (en fonction de sa taille et de col)
 * Redessine ensuite le canvas
 * 
 * @param widget
 * @param data 
 * @return gboolean 
 */
gboolean Droite(GtkWidget *widget, gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;
    if((drawStr->jeu->col < LARGEUR - 1 && drawStr->jeu->piece.largeur == 1) || 
    (drawStr->jeu->col < LARGEUR - 2 && drawStr->jeu->piece.largeur == 2) || 
    (drawStr->jeu->col < LARGEUR - 3 && drawStr->jeu->piece.largeur == 3) || 
    (drawStr->jeu->col < LARGEUR - 4 && drawStr->jeu->piece.largeur == 4))
    {
        drawStr->jeu->col += 1;
        gtk_widget_queue_draw(drawStr->drawArea);
    }
    return TRUE; // Tout s'est bien passé
}

/**
 * Action pour le bouton bas
 * Si le jeu est en marche et que la pièce ne dépassera pas de la grille:
 * - Fait tomber la pièce dans la colone correspondante (en se basant sur l'attribut col de la structure de jeu)
 * - Nettoie les lignes complètes et incrémente le score 
 * - Tire une nouvelle pièce 
 * - Met à jour le timer et redessine tout
 * 
 * Sinon passe l'état du jeu à false
 * 
 * @param widget 
 * @param data 
 * @return gboolean 
 */
gboolean Bas(GtkWidget *widget, gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;

    if (drawStr->jeu->etat == true)
    {
        if(!pieceDepasse(drawStr->jeu->g, drawStr->jeu->piece, hauteurExacte(drawStr->jeu->g, drawStr->jeu->col, drawStr->jeu->piece)))
        {
            ecrirePiece(drawStr->jeu->g, drawStr->jeu->piece, drawStr->jeu->col, hauteurExacte(drawStr->jeu->g, drawStr->jeu->col, drawStr->jeu->piece));
            drawStr->jeu->score += nettoyer(drawStr->jeu->g) * 100 + 10;
            nouvellePiece(drawStr->jeu);

            scoreUpdate(drawStr->jeu);

            drawStr->jeu->delay = 10;
            delayUpdate((gpointer)drawStr);

            gtk_widget_queue_draw(drawStr->drawArea);
        }
        else
        {
            drawStr->jeu->etat = false;
        }
    }
    else
    {
        g_source_remove(idTimeOut);
    }
    
    return TRUE; // Tout s'est bien passé
}

/**
 * Action pour le bouton rotation à gauche
 * Change la pièce courante par son homologue touné à gauche et redessine le canvas
 * Se sert d'un tableau à 2 dimensions comportant les différtentes combinaisons ainsi que de l'id de la pièce
 * 
 * @param widget 
 * @param data 
 * @return gboolean 
 */
gboolean rotationG(GtkWidget *widget, gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;
    int rotG[NB_PIECES][2] = { {0,3},{1,0},{2,1},{3,2} , {4,5},{5,4} , {6,7},{7,6} , {8,8} , {9,12},{10,9},{11,10},{12,11} };

    for (int i = 0; i < NB_PIECES; ++i)
    {
        if(drawStr->jeu->piece.id == rotG[i][0])
        {
            drawStr->jeu->piece = drawStr->jeu->tab[rotG[i][1]];
            break;
        }
    }
    
    gtk_widget_queue_draw(drawStr->drawArea);
    return TRUE; // Tout s'est bien passé
}

/**
 * Action pour le bouton rotation à droite
 * Change la pièce courante par son homologue touné à droite et redessine le canvas
 * Se sert d'un tableau à 2 dimensions comportant les différtentes combinaisons ainsi que de l'id de la pièce
 * 
 * @param widget 
 * @param data 
 * @return gboolean 
 */
gboolean rotationD(GtkWidget *widget, gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;
    int rotD[NB_PIECES][2] = { {0,1},{1,2},{2,3},{3,0} , {4,5},{5,4} , {6,7},{7,6} , {8,8} , {9,10},{10,11},{11,12},{12,9} };

    for (int i = 0; i < NB_PIECES; ++i)
    {
        if(drawStr->jeu->piece.id == rotD[i][0])
        {
            drawStr->jeu->piece = drawStr->jeu->tab[rotD[i][1]];
            break;
        }
    }
    
    gtk_widget_queue_draw(drawStr->drawArea);
    return TRUE; // Tout s'est bien passé
}

/**
 * Action pour le bouton NewGame
 * Réinitialise le jeu (Grille, tableau de pièce, piece courante, score, delai)
 * 
 * @param widget 
 * @param data 
 * @return gboolean 
 */
gboolean NewGame(GtkWidget *widget, gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;
    initialiseGrille(drawStr->jeu->g);
    genererPieces(drawStr->jeu->tab);
    nouvellePiece(drawStr->jeu);

    drawStr->jeu->delay = 10;
    drawStr->jeu->score = 0;
    scoreUpdate(drawStr->jeu);
    delayUpdate((gpointer)drawStr);

    gtk_widget_queue_draw(drawStr->drawArea);

    return TRUE; // Tout s'est bien passé
}

/**
 * @brief Create a arrow button object
 * 
 * @param arrow_type 
 * @return GtkWidget* 
 */
GtkWidget *create_arrow_button(GtkArrowType arrow_type)
{
  GtkWidget* button = gtk_button_new();
  GtkWidget* arrow = gtk_arrow_new(arrow_type, GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(button), arrow);
  gtk_widget_show_all(button);
  return button;
}

/**
 * Dessine un carré aux coordonnées passées en paramètres dans le canvas
 * Defini la couleur en fonction du caractère
 * 
 * @param cr 
 * @param ligne 
 * @param colonne 
 * @param c 
 */
void dessineCarre(cairo_t* cr, int ligne, int colonne, char c)
{
    double r, g, b;
    switch(c){
        case ' ':
            r = 1; g = 1; b = 1;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            break;
        case 'l':
            r = 1; g = 0; b = 0;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, r-0.5, g, b);
            break;
        case '|':
            r = 0; g = 0; b = 1;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, r, g, b-0.5);
            break;
        case '#':
            r = 0; g = 1; b = 0;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, r, g-0.5, b);
            break;
        case 'O':
            r = 1; g = 1; b = 0;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, r, g-0.5, b);
            break;
        case '@':
            r = 0; g = 1; b = 1;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, r, g-0.5, b-0.5);
            break;
        default:
            r = 1; g = 1; b = 1;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (colonne+2)*TAILLE_CARRE, (HAUTEUR+5-ligne)*TAILLE_CARRE, TAILLE_CARRE, TAILLE_CARRE);
            cairo_fill_preserve(cr);
            break;
    }
    
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);
}

/**
 * Dessine toute la grille de jeu dans le canvas à l'aide de la fonction dessineCarre
 * 
 * @param cr 
 * @param g 
 */
void dessineGrille(cairo_t* cr, Grille g)
{
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    for(int i = 0; i < HAUTEUR; i++)
    {
        for(int j = 0; j < LARGEUR; j++)
        {
            dessineCarre(cr, i, j, lireCase(g, i, j));
        }
    }
    afficheGrille(g); // DEBUG : Affiche dans la console un aperçu du jeu
}

/**
 * Dessine un aperçu de la pièce courante au dessus de la grille
 * 
 * @param cr 
 * @param pJeu 
 */
void dessinePiece(cairo_t* cr, Jeu* pJeu)
{
    int x = HAUTEUR+3;
    int y = pJeu->col;
    for(int i = pJeu->piece.hauteur; --i >= 0;)
    {
        for(int j = pJeu->piece.largeur; --j >= 0;)
        {
            dessineCarre(cr, x, y+pJeu->piece.largeur-1, pJeu->piece.forme[i][j]);
            y--;
        }
        x--;
        y = pJeu->col;
    }
}

gboolean realize_evt_reaction( GtkWidget *widget, gpointer data )
 { // force un événement "expose" juste derrière.
   gtk_widget_queue_draw(widget); 
   return TRUE;
 }

 // c'est la réaction principale qui va redessiner tout.
gboolean expose_evt_reaction( GtkWidget *widget, cairo_t *event, gpointer data )
{
    // c'est la structure qui permet d'afficher dans une zone de dessin
    // via Cairo 
    cairo_t* cr = gdk_cairo_create(gtk_widget_get_window(widget));
    Jeu* pJeu = (Jeu*) data;
    dessineGrille(cr, pJeu->g);
    dessinePiece(cr, pJeu);
    cairo_destroy(cr);

    return TRUE;
}

/**
 * Décrémente ou reinitilise l'attribut delay de la structure de jeu et met à jour le timer
 * Met également à jour le label delai
 * 
 * @param data 
 * @return gint 
 */
gint delayUpdate(gpointer data)
{
    DrawSignal* drawStr = (DrawSignal*)data;
    if(drawStr->jeu->delay > 0)
        drawStr->jeu->delay--;
    else
    {
        drawStr->jeu->delay = 10;
        Bas(NULL, drawStr);
    }
    char temp[30] = {""};
    sprintf(temp, "Delay :  %d", drawStr->jeu->delay);
    gtk_label_set_text(GTK_LABEL(labelDelay), temp);
    g_source_remove(idTimeOut);
    idTimeOut = g_timeout_add(1000, delayUpdate, (gpointer)drawStr); // réenclenche le timer.
    return 0;
}

/**
 * Met à jour le label score
 * 
 * @param ptrJeu 
 */
void scoreUpdate(Jeu* ptrJeu)
{
    char temp[30] = {""};
    sprintf(temp, "Score :  %d", ptrJeu->score);
    gtk_label_set_text(GTK_LABEL(labelScore), temp);
}

/**
 * Fonction créeant toute la vue de notre fenètre, et s'occupe de lié les widgets à leurs signaux/slots
 * 
 * @param ptrJeu 
 * @return GtkWidget* 
 */
GtkWidget *createIHM(Jeu* ptrJeu)
{
    // Layouts
    GtkWidget* boxArrow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    GtkWidget* boxBtn = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
    GtkWidget* boxLeftPanel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* boxRightPanel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* boxWindow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Labels
    labelScore = gtk_label_new( "Score : " );
    labelDelay = gtk_label_new( "Delay :    10" );

    // Boutons
    GtkWidget* left = create_arrow_button( GTK_ARROW_LEFT );
    GtkWidget* right = create_arrow_button( GTK_ARROW_RIGHT );
    GtkWidget* down = create_arrow_button( GTK_ARROW_DOWN );
    GtkWidget* btnRotationD = gtk_button_new_with_label("RD");
    GtkWidget* btnRotationG = gtk_button_new_with_label("RG");
    GtkWidget* new = gtk_button_new_with_label ( "New" );
    GtkWidget* button_quit = gtk_button_new_with_label ( "Quit" );

    gtk_box_set_homogeneous(GTK_BOX(boxArrow), TRUE);
    gtk_box_set_spacing(GTK_BOX(boxArrow), 10);
    gtk_box_set_homogeneous(GTK_BOX(boxRightPanel), TRUE);

    // Canvas
    GtkWidget* drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request(drawing_area, TAILLE_CARRE*(LARGEUR+4), TAILLE_CARRE*(HAUTEUR+7));

    // Structure pour pouvoir passer aux signaux le canvas ainsi que le Jeu
    DrawSignal *drawStr = g_slice_new0(DrawSignal);
    drawStr->drawArea = drawing_area;
    drawStr->jeu = ptrJeu;

    // Connecte la réaction gtk_main_quit à l'événement "clic" sur ce bouton.
    g_signal_connect( button_quit, "clicked",
                    G_CALLBACK( gtk_main_quit ),
            NULL);

    g_signal_connect( new, "clicked",
                    G_CALLBACK( NewGame ),
            drawStr);

    g_signal_connect( left, "clicked",
                    G_CALLBACK( Gauche ),
            drawStr);

    g_signal_connect( right, "clicked",
                    G_CALLBACK( Droite ),
            drawStr);

    g_signal_connect( down, "clicked",
                    G_CALLBACK( Bas ),
            drawStr);

    g_signal_connect( btnRotationG, "clicked",
                    G_CALLBACK(rotationG),
            drawStr);

    g_signal_connect( btnRotationD, "clicked",
                    G_CALLBACK(rotationD),
            drawStr);

    g_signal_connect( G_OBJECT(drawing_area), "realize", 
            G_CALLBACK(realize_evt_reaction), ptrJeu );
    g_signal_connect( G_OBJECT (drawing_area), "draw",
            G_CALLBACK (expose_evt_reaction), ptrJeu );

    gtk_container_add(GTK_CONTAINER(boxArrow), left);
    gtk_container_add(GTK_CONTAINER(boxArrow), btnRotationG);
    gtk_container_add(GTK_CONTAINER(boxArrow), down);
    gtk_container_add(GTK_CONTAINER(boxArrow), btnRotationD);
    gtk_container_add(GTK_CONTAINER(boxArrow), right);

    gtk_container_add(GTK_CONTAINER(boxBtn), new);
    gtk_container_add(GTK_CONTAINER(boxBtn), button_quit);

    gtk_container_add(GTK_CONTAINER(boxLeftPanel), drawing_area);
    gtk_container_add(GTK_CONTAINER(boxLeftPanel), boxArrow);

    gtk_container_add(GTK_CONTAINER(boxRightPanel), boxBtn);
    gtk_container_add(GTK_CONTAINER(boxRightPanel), labelScore);
    gtk_container_add(GTK_CONTAINER(boxRightPanel), labelDelay);

    gtk_container_add(GTK_CONTAINER(boxWindow), boxLeftPanel);
    gtk_container_add(GTK_CONTAINER(boxWindow), boxRightPanel);

    ptrJeu->delay = 10;
    ptrJeu->score = 0;
    idTimeOut = g_timeout_add(1000, delayUpdate, (gpointer)drawStr);

    return boxWindow;
}

int main( int   argc,
          char *argv[] )
{
    srand(time(NULL));
    GtkWidget *window;
    Jeu jeu;
    jeu.etat = true;

    initialiseGrille(jeu.g);
    genererPieces(jeu.tab);
    nouvellePiece(&jeu);
    /* Passe les arguments à GTK, pour qu'il extrait ceux qui le concernent. */
    gtk_init(&argc, &argv);
    
    /* Crée une fenêtre. */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkWidget* boxWindow = createIHM(&jeu);

    gtk_container_add(GTK_CONTAINER(window), boxWindow);

    gtk_widget_show_all(boxWindow);
    /* La rend visible. */
    gtk_widget_show (window);
    
    /* Rentre dans la boucle d'événements. */
    /* Tapez Ctrl-C pour sortir du programme ! */
    gtk_main();
    return 0;
}