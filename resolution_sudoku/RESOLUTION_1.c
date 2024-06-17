/************
 * INCLUDES *
*************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/**************************
 * CONSTANTES SYMBOLIQUES
***************************/
#define N 4
#define TAILLE (N * N)

/**
 * @def on definit les sépateurs de lignes
 */
const char SEP[60] = "+------------+------------+------------+------------+";
/**
 * @def on definit les sépateurs de colonnes
 */
const char COL[2] = "|";

/***************
 * TYPE GRILLE *
****************/
typedef int tGrille[TAILLE][TAILLE];

/**********************************
 * ENTETES PROCEDURE ET FONCTIONS *
***********************************/
void chargerGrille(tGrille,char[30]);
void afficherGrille(tGrille);
bool backtracking(tGrille, int);
bool possible(tGrille, int , int , int );
void aleatoire(tGrille);


/**
 * @brief Fonction permettant de relier toutes les procédures/fonctions pour résoudre la grille
 * 
 * @return 0 si le programme c'est déroulé sans encombres et 1 dans le cas contraire
 */
int main(){
    tGrille g;
    char nomFichier[30];
    chargerGrille(g,nomFichier);
    printf("-----Grille %s Initiale-----\n",nomFichier);
    afficherGrille(g);
    clock_t begin = clock();
    backtracking(g,0);
    clock_t end = clock();
    printf("-----Grille Resolue-----\n");
    afficherGrille(g);
    aleatoire(g);
    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;
    printf("-----Temps pour la grille %s-----\n",nomFichier);
    printf( "Resolution de la grille en %.6f secondes\n",tmpsCPU);
}


void aleatoire(tGrille grille){
    int boucleC;
    int boucleL;
    for ( boucleC = 0; boucleC < TAILLE; boucleC++)
    {
        for (boucleL = 0;boucleL < TAILLE;boucleL++)
        {
            srand(time(NULL));
            int integer = (rand()%16)+1;
            printf("%d",integer);
            grille[boucleC][boucleL] = integer;
        }
        
    }
    
}

/**
 * @brief Procédure permettant de charger la grille de jeu
 * 
 * @param g la grille de jeu
 * @param nomFichier le nom du fichier qui sera récurpérer
 */
void chargerGrille(tGrille g,char nomFichier[30])
{
    // defintion d'une valeur pour rentrer le nom du fichier
    // association du nom logique et du nom externe
    FILE *f;
    printf("Nom du fichier ?\n");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    // on teste si on peut ouvrir le fichier*
    if (f == NULL)
    {
        // Affichage d'un message d'erreur
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);

}

/**
 * @brief Fonction permettant de résoudre une grille de Sudoku
 * 
 * @param grille la grille de jeu
 * @param numeroCase la case départ qui sera incrémenter à chaque passage (fonction récursive)
 * @return true/false
 */

bool backtracking(tGrille grille, int numeroCase){
    int ligne;
    int colonne;
    bool resultat;
    resultat = false;
    // printf("%d\n", numeroCase);
    if (numeroCase == (TAILLE * TAILLE)) {
        //on a traité toutes les cases, la grille est résolue
        resultat = true;
    }
    else
    {
        // On récupère les "coordonnées" de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;

        // printf("%d %d\n", ligne, colonne);

        if (grille[ligne][colonne]!=0)
        {
            // Si la case n’est pas vide, on passe à la suivante
            // (appel récursif)

            resultat = backtracking(grille, numeroCase+1);
            // printf("%d", resultat);
        
        }
        else
        {
            for (int valeur = 1 ; valeur <= TAILLE ; valeur++){
                //printf("toto\n");
                if ((possible(grille,ligne,colonne,valeur))){
                    // Si la valeur est autorisée on l'inscrit
                    //dans la case...
                    
                    grille[ligne][colonne] = valeur;
                    
                    // ... et on passe à la suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if ((backtracking(grille,numeroCase+1)) == true){
                        resultat = true;
                    }
                    else{
                        grille[ligne][colonne] = 0;
                    }
                }
            }

        }
    }
    return resultat;
}

/**
 * @brief Fonction permettant de savoir si une valeur est présente sur ligne, colonne ou bloc de la grille
 * 
 * @param grille la grille de jeu 
 * @param numLigne le numéro de ligne
 * @param numColonne le numéro de colonne
 * @param valeur la valeur recherchée
 * @return la fonction nous retourne un booléen et nous permet de savoir si la valeur peut être placée à l'endroit indiqué
 */

bool possible(tGrille grille, int numLigne, int numColonne, int valeur) {
    bool booleen ;
    booleen = true ;
    /** Vérifier la ligne */
    for (int i = 0; i < TAILLE; i++) {
        if (grille[numLigne][i] == valeur) {
            booleen = false;
        }
    }

    /** Vérifier la colonne */       
    for (int i = 0; i < TAILLE; i++) {
        if (grille[i][numColonne] == valeur) {
            booleen = false;
        }
    }

    /** Vérifier le bloc */ 
    int debutLigneBloc = N * (numLigne / N);
    int debutColonneBloc = N * (numColonne / N);

    for (int i = debutLigneBloc; i < debutLigneBloc + N; i++) {
        for (int j = debutColonneBloc; j < debutColonneBloc + N; j++) {
            if (grille[i][j] == valeur) {
                booleen = false;
            }
        }
    }
    return booleen;
}

/**
 * @brief Procédure permettant d'afficher la grille de jeu
 * 
 * @param grille la grille à afficher
 */

void afficherGrille(tGrille grille) {
    int i ;
    int j ;
    /** Afficher les numéros de colonne */
    printf("     ");
    for (int i = 1; i < TAILLE+1; i++){
        if (i < 9){
            printf("%d  ",i);
        }else{
            printf("%d ",i);
        }
        if ((i % N)== 0 ){
            printf(" ");
        }
        
    }
    printf("\n");
    /** Afficher le cadre supérieur de la grille */
    printf("   %s\n",SEP);
    /** Afficher la grille avec les numéros de ligne */
    for (i = 0; i < TAILLE; i++) {
        if (i +1 > 9)
        {
            printf("%d |", i + 1);
        }
        else
        {
            printf("%d  |", i + 1);
        }
        for (j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 0) {
                printf(" . ");
            } 
            else 
            {
                if (grille[i][j] > 9)
                {
                    printf("%d ", grille[i][j]);
                }
                else
                {
                    printf(" %d ", grille[i][j]);
                }
            }
            if (j % 4 == 3) {
                printf("|");
            }
        }
        printf("\n");

        /** Afficher le séparateur horizontal après chaque 3 lignes */
        if (i % N == 3) {
            printf("   %s\n",SEP);
        }
    }
}
