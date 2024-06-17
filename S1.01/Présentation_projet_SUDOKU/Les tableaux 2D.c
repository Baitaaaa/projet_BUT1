#include <stdlib.h>
#include <stdio.h>
/************************************************************************/
/* Ce programme montre comment utiliser un tableau à 2 dimensions en C  */
/* Ce document présente à travers un exemple :                          */
/*    1 - la DECLARATION d'un tableau 2D                                */
/*    2 - l'INITIALISATION statique d'un tableau 2D                     */
/*    3 - le PARCOURS COMPLET d'un tableau 2D                           */
/************************************************************************/

/************************************************************************/
/* 1 - DECLARATION d'un tableau 2D                                      */
/*    - Le mieux est de déclarer un nouveau type avec typedef           */
/*    - Les constantes qui servent à dimensionner le tableau            */
/*      sont à déclarer avec #define                                    */
/*    - le premier indice représente les lignes, le deuxième indice     */
/*      représente les colonnes                                         */
/************************************************************************/

// déclaration d'un type tableau de 6 lignes sur 3 colonnes d'entiers
#define NB_LIGNES 6
#define NB_COLONNES 3

typedef int tab2D[NB_LIGNES][NB_COLONNES];

/********************************************************************************/
/*  2 - INITIALISATION d'un tableau 2D                                          */
/*    - il faut déclarer des variables de type tab2D                            */
/*    - on peut initialiser un tableau de manière statique (c'est-à-dire        */
/*      avant exécution) lors de la déclaration de la variable                  */
/********************************************************************************/
void parcours (tab2D t);
int main () {
    // déclaration d'un premier tableau (non initialisé)
    tab2D  monPremierTableau2D;

    // déclaration d'un deuxième tableau, initialisé statiquement
    // remarquez les paires de {} pour délimiter les valeurs des lignes
    tab2D  monDeuxiemeTableau2D =   {
                                        {1, 4, 3},      // première ligne
                                        {5, 8, 7},      // deuxième ligne
                                        {10, 4, 34},    // etc.
                                        {2, 9, 9},
                                        {31, 42, 73},
                                        {1, 34, 93}
                                    };

    parcours(monPremierTableau2D);
    return EXIT_SUCCESS;
}

/****************************************************************************************************/
/*  3 - PARCOURS COMPLET d'un tableau 2D                                                            */
/*    - il faut une double boucle pour parcourir toutes les lignes du tableau, et pour chaque ligne,*/
/*      parcourir toutes les cases de la ligne                                                      */
/*    - rappel : le premier indice est l'indice de ligne, le deuxième l'indice de colonne           */
/****************************************************************************************************/
void parcours (tab2D t){
    int i;  // indice de ligne
    int j;  // indice de colonne

    for (i=0 ; i<NB_LIGNES ; i++){
        for (j=0 ; j<NB_COLONNES ; j++){

            // accès à chaque case t[i][j] du tableau

        }
    }
}
