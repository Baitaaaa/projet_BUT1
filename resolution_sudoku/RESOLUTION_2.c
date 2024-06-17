/**
 * @file RESOLUTION-2.c
 * @brief Programme pour résoudre un jeu de Sudoku 16x16.
 * @author LAÎNÉ Baptiste, BRUNEAU-GACHE Léo
 * @version 1.13
 * @date 27 janvier 2024
 * 
 * Ce programme permet de sudoku avec des grilles 16x16 en utilisant la méthode du 
 * backtracking mais améliorée.
 * 
 * Rapide pour les grilles complexes.
 * 
*/


/************
 * INCLUDES *
 ************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/**************************
 * CONSTANTES SYMBOLIQUES *
 **************************/
#define N 4
#define TAILLE (N * N)


/**************
 * CONSTANTES *
**************/

// sépateurs de lignes
const char SEP[60] = "+------------+------------+------------+------------+";

// sépateurs de colonnes
const char COL[2] = "|";

// nombre de cases dans la grille
const int NUMCASE = (TAILLE * TAILLE);

// valeur donnée aux cases qui sont remplies (qui ont 0 candidats) 
const int INDISPONIBLE = 255 ;


/*********
 * TYPES *
*********/

// structure pour une case dans le tableau ordre
typedef struct{
    int i ; // indice en x
    int j ; // indice en y
    int val ; // nombre de candidats
} coordonnee;

// grille de sudoku
typedef int tGrille[TAILLE][TAILLE];

// tableau pour l'ordre de remplissage des cases 
typedef coordonnee ordre[TAILLE * TAILLE] ;

void chargerGrille(tGrille,char[30]);
void afficherGrille(tGrille);
bool backtracking(tGrille,ordre);
bool possible(tGrille, int , int , int );
void ordrePassage(tGrille,ordre) ;
int nbCandidats(tGrille, int, int);
void afficherCand(ordre);
void init(ordre) ;


/************************
 * PROGRAMME PRINCIPALE *
************************/

int main(){
    tGrille g;
    ordre tabOrdre ;
    char nomFichier[30];

    init(tabOrdre) ;
    chargerGrille(g,nomFichier);
    ordrePassage(g, tabOrdre) ;

    printf("-----Grille %s Initiale-----\n",nomFichier);
    afficherGrille(g);

    clock_t begin = clock();

    backtracking(g,tabOrdre);

    clock_t end = clock();

    printf("-----Grille Resolue-----\n");
    afficherGrille(g);

    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;
    printf("-----Temps pour la grille %s-----\n",nomFichier);
    printf( "Temps CPU = %.6f secondes\n",tmpsCPU);
}


/************
 * FONCTION *
************/

/**
 * @fn chargerGrille(tGrille g,char nomFichier[30])
 * @param g : la grille à charger
 * @param nomFichier : le nom du fichier 
 * @brief fonction pour charger la grille a partir d'un fichier binaire
*/
void chargerGrille(tGrille g,char nomFichier[30])
{
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
 * @fn backtracking(tGrille grille,ordre tabOrdre)
 * @param grille : la grille à compléter
 * @param tabOrdre : le tableau pour l'ordre des cases
 * @brief fonction qui résout le sudoku avec la méthode du backtracking
 * mais améliorée : on parcourt la grille dans l'ordre des cases qui ont le moins
 * de candidats à celles qui en ont le plus et en mettant à jour l'ordre à chaque fois
*/
bool backtracking(tGrille grille,ordre tabOrdre){
    // définit l'ordre de remplissage et initialise numeroCase à 0
    ordrePassage(grille,tabOrdre) ;
    int numeroCase = 0 ;

    // si toutes les cases sont pleine
    if (tabOrdre[numeroCase].val == INDISPONIBLE)
        return true;

    // on récupère l'indice de la case
    int i = tabOrdre[numeroCase].i ;
    int j = tabOrdre[numeroCase].j ;

    // pour toutes les valeurs de 1 à 16
    for (int k=1; k <= TAILLE; k++)
    {
        // si la valeur est valide
        if (possible(grille,i,j,k))
        {
            // on met la valeur dans le tableau
            grille[i][j] = k;
            // on regarde pour les autres cases
            if ( backtracking (grille,tabOrdre) )
                return true;
        }
    }
    grille[i][j] = 0;

    return false;
}

/**
 * @fn possible(tGrille grille, int numLigne, int numColonne, int valeur)
 * @param grille : la grille de sudoku
 * @param numLigne : le numero de la ligne
 * @param numColonne : le numero de colonne
 * @brief vérifie si une valeur peut être insérer dans une certaine case du
 * tableau en respectant les règle du sudoku
*/
bool possible(tGrille grille, int numLigne, int numColonne, int valeur) {
    bool estTrouvee = true ;
    int boucle;
    boucle = 0;
    while (estTrouvee == true && boucle < TAILLE)
    {
        if (grille[numLigne][boucle] == valeur || grille[boucle][numColonne] == valeur) {
            estTrouvee = false;
        }
        /* code */
    
        // Vérifier la ligne et la colonne

        // Vérifier le bloc
        int debutLigneBloc = N * (numLigne / N);
        int debutColonneBloc = N * (numColonne / N);

        // Utilisation une seule boucle pour vérifier le bloc
        for (int boucleBloc = 0; boucleBloc < N * N; boucleBloc++) {
            int ligneBloc = debutLigneBloc + boucleBloc / N;
            int colonneBloc = debutColonneBloc + boucleBloc % N;

            if (grille[ligneBloc][colonneBloc] == valeur) {
                estTrouvee = false;
            }
        }
        boucle++;
    }
    return estTrouvee;
}

/**
 * @fn afficherGrille(tGrille grille)
 * @param grille : la grille de sudoku
 * @brief affiche la grille
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


/**
 * @fn compare_num (void const *a, void const *b)
 * @brief fonction necessaire pour utiliser la fonction qsort() permettant de trier le tableau
*/
int compare_num (void const *a, void const *b){
    coordonnee const *pa = a ;
    coordonnee const *pb = b ;
    return pa->val - pb->val ; 
}

/**
 * @fn init(ordre tabOrdre)
 * @param tabOrdre : le tableau pour l'ordre des cases
 * @brief fonction qui initialise le tableau de structure
*/
void init(ordre tabOrdre){
    for (int i = 0; i < NUMCASE; i++)
    {
        tabOrdre[i].i = 0;
        tabOrdre[i].j = 0;
        tabOrdre[i].val = INDISPONIBLE;
    } 
}

/**
 * @fn ordrePassage(tGrille grille, ordre tabOrdre)
 * @param grille : la grille de sudoku
 * @param tabOrdre : le tableau pour l'ordre des cases
 * @brief fonction qui détermine l'ordre dans lequel on va parcourir la grille
 * de sudoku (de la case qui a le moins de candidats a celle qui en a le plus)
*/
void ordrePassage(tGrille grille, ordre tabOrdre){
    int nb = 0;
    for (int i = 0; i < TAILLE; i++) 
    {
        for (int j = 0; j < TAILLE; j++) 
        {
            tabOrdre[nb].i = i;
            tabOrdre[nb].j = j;
            if (grille[i][j] == 0)
            {
                tabOrdre[nb].val = nbCandidats(grille, i, j);
            }
            else
            {
                tabOrdre[nb].val = INDISPONIBLE;
            }
            nb++;
        }
    }
    //fonction utiliser pour trier un tableau
    qsort(tabOrdre, NUMCASE + 1, sizeof(coordonnee), compare_num);
}

/**
 * @fn afficherCand(ordre tabOrdre)
 * @param tabOrdre : le tableau pour l'ordre des cases
 * @brief fonction utile qui afficher le tableau de structure représentant
 * l'ordre de remplissage des cases
*/
void afficherCand(ordre tabOrdre){
    printf("______________________________________\n") ;
    for (int i = 0; i < TAILLE * TAILLE; i++) 
    {
        printf("(%d, %d, %d)\n", tabOrdre[i].i, tabOrdre[i].j, tabOrdre[i].val);
    }
    printf("______________________________________\n") ;
}

/**
 * @fn nbCandidats(tGrille grille, int i, int j , int nb)
 * @param grille : la grille de sudoku
 * @param i : l'indice de ligne
 * @param j : l'indice de colonne
 * @brief fonction qui renvoie le nombre de candidat pour une case donnée
*/
int nbCandidats(tGrille grille, int i, int j){
    int nbCandidats = 0;
    for (int val = 1 ; val <= TAILLE ; val++)
    {
        if (possible(grille,i,j,val))
        {
            nbCandidats++ ;
        }
    }
    return nbCandidats ;
}












