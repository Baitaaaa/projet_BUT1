/**
* \page Général 
* 
* \author CHESNEL
* \version 1.0
* \date 23 Novembre 2023 
*
* Ce programme permet de jouer a un jeu de sudoku, Il lit un document avec une grille
et lit les placements de valeur par des commandes. Il y une vérification pour empêcher
de rentrer n'importe quel valeur a n'importe quel endroit, et enfin le jeu se fini
quand la grille est entièrement rempli.
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * \def NB_LIGNE 
 * \brief correspond au nombre maximum de ligne que le tableau peut nous présenter.
*/
#define NB_LIGNE 9

/**
 * \def NB_COLONNE
 * \brief correspond au nombre maximum de colonne que le tableau peut nous présenter.
*/
#define NB_COLONNES 9

/**
 * \def NB_SQUARE 
 * \brief correspond au nombre de ligne & colonne d'un carré sur le tableau du sudoku.
*/
#define NB_SQUARE 3

/**
* \typedef tGrille
* \brief type tableau de taille NB_LIGNE ( \see NB_LIGNE ), par NB_COLONNE ( \see NB_COLONNE ).
* C'est ce qui permet de définir la taille de la grille du sudoku.
*/


typedef int tGrille[NB_LIGNE][NB_COLONNES];

/**
 * \fn void saisir(int* valeur)
 * \brief Elle permet de vérifier que la valeur est correcte
 * \param valeur il s'agit de la valeur qui sera vérifié dans la procèdure
 * Quand on l'utiliser elle va servir de scanf
 * Puis elle vérifiera que la valeur donnée respecte les contraintes
 * ne dépasse pas le tableau, est un entier.
 *  
 */

void saisir(int* valeur)
{
    char ch[10] ;
    bool bonne_valeur = false;
    while (bonne_valeur == false)
    {
        scanf("%s", ch);
        if (sscanf(ch, "%d", valeur) !=0) // sscanf qui retourne 0 si la conversion n’est pas possible.
        {
            if ((*valeur >0) && (*valeur <NB_COLONNES+1))
            {
                bonne_valeur = true ;
                printf("La conversion a réussi.\n");
            }
            else if (*valeur <= 0)
            {
                printf("votre valeur est trop basse.\n");
            }
            else if (*valeur > NB_COLONNES)
            {
                printf("votre valeur est trop grande\n");
            }
        }
        else
        {
            printf("votre valeur n'est pas un entier\n");
        }
    }
}


/**
 * \fn Procédure chargerGrille
 * \param g il s'agit de la grille qu'on utilise tout au long du jeu. Et donc celle qu'on charge.
 * \brief Elle permet de récupérer des grilles données de Sudoku
 *
 * Cette procèdure permet de rentrer des grilles préchoisies, part des fichiers.
*/

void chargerGrille(tGrille g)
{
    char nomFichier[30];
    FILE * f;

    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(g, sizeof(int), NB_LIGNE * NB_COLONNES, f);
    }
    fclose(f);
}

/**
 * \fn void afficherGrille(tGrille grille)
 * \param grille il s'agit de la grille qu'on utilise tout au long du jeu. ici elle permet de remplir les cases.
 * \brief Cette procèdure permet d'afficher la grille.

 * C'est elle qui fais les colonnes ( \see NB_COLONNE ).
 * C'est elle qui fais les lignes. ( \see NB_LIGNE)
 * C'est elle qui affiche les bornes de la grille.
 * C'est elle qui remplis les cases avec des nombres au 1er tour. ( \see void chargerGrille(tGrille g) )
 * C'est aussi elle qui affiche les points donc.
 * Sans elle, le programme n'apparaîtrai pas comme un jeu.
*/

void afficherGrille(tGrille grille)
{
    int ligne, colonne;
    printf("       1  2  3   4  5  6   7  8  9 \n");
    printf("     +-----------------------------+");
    for (ligne = 0; ligne < NB_LIGNE; ligne++)
    {
        if ((ligne == 3) || (ligne == 6))
        {
            printf("\n     ");
        }
        else
        {
            printf("\n%d    ",ligne+1);
        }


        for (colonne = 0; colonne < NB_COLONNES; colonne++)
        {
            if ((ligne == 3 && colonne == 0) || (ligne == 6 && colonne == 0))
            {
                printf("+---------+---------+---------+\n");
                printf("%d    |",ligne+1);
            }
            else if (colonne == 3 || colonne == 6 || colonne == 0)
            {
                printf("|");
            }
            if (grille[ligne][colonne] == 0)
            {
                printf(" . ");
            }
            else
            {
                printf(" %d ", grille[ligne][colonne]);
            }
        }
        printf("|");
    }
    printf("\n     +---------+---------+---------+\n");
}

/**
 * \brief fin de l'initialisation des procèdures.
*/


/**
 * @fn bool possible(tGrille grille, int lig, int col, int val)
 * @brief Vérifie si la valeur et la position rentré en paramètre respectent les règles du sudoku.
 * 
 * @param grille il s'agit de la grille qu'on utilise tout au long du jeu. Ici elle ne change pas
 * @param lig La ligne choisie.
 * @param col La colonne choisie.
 * @param val La valeur choisie.
 * @return Renvoie true si le placement ne pose pas problème, sinon renvoie false.
 * 
 */


bool possible(tGrille grille, int lig, int col, int val)
{
    bool end = true ;
    int test_line = 0;
    int test_col = 0;
    int square_c = NB_SQUARE * (col / NB_SQUARE);
    int square_l = NB_SQUARE * (lig / NB_SQUARE);
    int parcours_l ;
    int parcours_c ;
    for (test_col = 0; test_col < NB_LIGNE; test_col++)
    {
        if (grille[lig][test_col] == val)
        {
            printf("la même valeur se trouve sur la même ligne\n");
            end = false ;
        }
    }
    if (end != false)
    {
       for (test_line = 0; test_line < NB_COLONNES; test_line++)
        {
            if (grille[test_line][col] == val)
            {
                printf("la même valeur se trouve sur la même colonne\n");
                end = false;
            }
        }
        for (parcours_l = square_l ; parcours_l <= NB_SQUARE; parcours_l++)
        {
            for (parcours_c = square_c ; parcours_c <= NB_SQUARE; parcours_c++)
            {
                if (parcours_c != col || parcours_l != lig)
                {
                    if (grille[parcours_l][parcours_c] == val )
                    {
                        printf("la même valeur se trouve dans le même carré\n");
                        end = false ;
                    }
                }
                
            }
        }
    }
    return end;
}

/**
 * @fn bool remplie(tGrille grille1)
 * @brief Vérifie si la grille du sudoku est remplie ou non.
 * 
 * @param grille1 il s'agit de la grille qu'on utilise tout au long du jeu. Ici elle ne change pas
 * @return Renvoie true si la grille est remplie, sinon renvoie false.
 * 
 */

bool remplie(tGrille grille1)
{
    int nb_case_vide = 0;
    int ligne_check ;
    int colonne_check ;
    bool check = false ;
    for (ligne_check = 0; ligne_check < NB_LIGNE; ligne_check++)
    {
        for (colonne_check = 0; colonne_check < NB_COLONNES; colonne_check++)
        {
            if (grille1[ligne_check][colonne_check] == 0)
            {
                nb_case_vide ++;
            }
        }
    }
    if (nb_case_vide == 0 )
    {
        check = true;
    }
    return check;
}

/**
 * @fn int main()
 * @brief Il s'agit du programme principal, sur lequel le jeux se déroule.
 * 
 * Le programme principal appelle chaque fonction et procèdures. Elle possède une boucle
 * qui se répéte jusqu'à ce que la procèdure remplie renvoie false
 * Elle demandera la ligne, la colonne puis la valeur, vérifie si c'est possible puis
 * met ou non la valeur dans la grille. Quand la grille est remplie, le jeu est fini, et
 * un message s'envoie.
 */

int main()
{

    tGrille grille1;
    int numLigne;
    int numColonne;
    int valeur ;
    bool fini ;
    bool poss ;
    chargerGrille(grille1);
    fini = false ;
    while (fini == false)
    {
        afficherGrille(grille1);
        printf("Indices de la case (ligne) ? \n");
        saisir(&numLigne);
        printf("Indices de la case (colonne) ? \n");
        saisir(&numColonne);
        printf("Valeur ? \n");
        saisir(&valeur);
        if (grille1[numLigne-1][numColonne-1] != 0)
        {
            printf("la case est déjà occupé %d \n", grille1[numLigne][numColonne]);
        }
        else
        {
            poss = possible(grille1,numLigne-1,numColonne-1,valeur);
            if (poss == true)
            {
                grille1[numLigne-1][numColonne-1] = valeur;
            }
        }
        fini = remplie(grille1);
    }
    printf("le jeu est fini.");
    return EXIT_SUCCESS;
}