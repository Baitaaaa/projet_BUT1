/**
 * @file main.c
 * @brief Programme pour jouer à un jeu de Sudoku.
 * @author LAÎNÉ Baptiste
 * @version 1.0
 * @date 26 novembre 2023
 * 
 * Ce programme permet de jouer à un jeu de sudoku en important des grilles de fichiers
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TAILLE 9


/**
 * @typedef t_grille
 * @brief Tableau deux dimension de taille TAILLE
 * 
 * Grille pour représenter la grille de sudoku
 */

typedef int t_grille[TAILLE][TAILLE] ;


void chargerGrille(t_grille);
void afficherGrille(t_grille);
void saisir(int*);
bool possible(t_grille,int,int,int);
int grillePleine(t_grille);


/**
 * @fn int main()
 * @brief Programme principal.
 * 
 * Le programme principal permet de faire le liens entre les différentes fonctions pour
 * pouvoir jouer au sudoku
 */
int main(){
    t_grille grille1 ;
    int numLigne,numColonne,valeur;
    chargerGrille(grille1);
    while (! grillePleine(grille1))
    {
        afficherGrille(grille1);
        printf("indice de la case : \n");
        saisir(&numLigne);
        saisir(&numColonne);
        if (grille1[numLigne-1][numColonne-1]!=0)
        {
            printf("IMPOSSIBLE, la case n'est pas libre\n");

        }
        else 
        {
            printf("Valeur à insérer : \n");
            saisir(&valeur);
            if (possible(grille1,numLigne-1,numColonne-1,valeur))
            {
                grille1[numLigne-1][numColonne-1] = valeur;
                
            }
        }
    }
    printf("Grille pleine, fin de partie");
}

/**
 * @fn chargerGrille(t_grille g)
 * @brief Charge une grille de Sudoku à partir d'un fichier.
 * @param g La grille à charger.
 * 
 * Charge la grille à partir d'un fichier .sud dans un tableau de 2 dimensions, les cases vides
 * seront représenté par un 0.
 */
void chargerGrille(t_grille g){
    char nomFichier[30];
    FILE * f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        fread(g, sizeof(int), TAILLE*TAILLE, f);
    }
    fclose(f);
}

/**
 * @fn afficherGrille(t_grille grille) 
 * @brief Affiche une grille de Sudoku.
 * @param grille La grille à afficher.
 * 
 * Affiche la grille en parcourant chaques valeurs de la grille et en la remplaçant par un "."
 * si elle est égale à 0.
 */
void afficherGrille(t_grille grille) {
    int i ;
    int j ;
    /** Afficher les numéros de colonne */
    printf("     1  2  3   4  5  6   7  8  9\n");

    /** Afficher le cadre supérieur de la grille */
    printf("   +---------+---------+---------+\n");

    /** Afficher la grille avec les numéros de ligne */
    for (i = 0; i < TAILLE; i++) {
        printf("%d  |", i + 1);
        for (j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 0) {
                printf(" . ");
            } 
            else 
            {
                printf(" %d ", grille[i][j]);
            }
            if (j % 3 == 2) {
                printf("|");
            }
        }
        printf("\n");

        /** Afficher le séparateur horizontal après chaque 3 lignes */
        if (i % 3 == 2) {
            printf("   +---------+---------+---------+\n");
        }
    }
}


/**
 * @fn saisir(int *val)
 * @brief Saisit un entier entre 1 et 9.
 * @param val Pointeur vers la variable où stocker la valeur saisie.
 * 
 * Permet de saisir un entier sous forme de chaine de caractere entre 1 et 9 puis met le nombre
 * dans la variable val en entrée/sortie si il est convertible en entier
 */
void saisir(int *val) {
    char input[100];
    int value;
    int stop ;
    stop = 1 ;
    while (stop)
    {
    
        scanf("%s", input);

        if (sscanf(input, "%d", &value) != 0) 
        {
            if ((value >= 1) && (value <= 9)) 
            {
                *val = value;
                stop = 0;
            } 
            else 
            {
                printf("Erreur : La valeur doit être entre 1 et 9.\n");
            }
        } 
        else 
        {
            printf("Erreur : Veuillez saisir un entier valide.\n");
        }

        /** Nettoyer le tampon d'entrée en cas d'erreur */
        while (getchar() != '\n');

    } 
}

/**
 * @fn possible(t_grille grille, int numLigne, int numColonne, int valeur)
 * @brief Vérifie si une valeur peut être insérée dans une position donnée de la grille.
 * @param grille La grille de Sudoku.
 * @param numLigne Numéro de ligne.
 * @param numColonne Numéro de colonne.
 * @param valeur La valeur à vérifier.
 * @return true si la valeur est possible, false sinon.
 * 
 * Vérifie si la valeur peut aller dans une case donnée avec numLigne et numColonne en vérifiant la colonne,
 * la ligne et le bloc et affiche un message d'erreur qui précise où est le problème puis renvoie true ou false
 * si la valeur rentre ou pas.
 */
bool possible(t_grille grille, int numLigne, int numColonne, int valeur) {
    bool booleen ;
    booleen = true ;
    /** Vérifier la ligne */
    for (int i = 0; i < TAILLE; i++) {
        if (grille[numLigne][i] == valeur) {
            printf("Erreur : La valeur %d est déjà présente sur la même ligne.\n", valeur);
            booleen = false;
        }
    }

    /** Vérifier la colonne */       
    for (int i = 0; i < TAILLE; i++) {
        if (grille[i][numColonne] == valeur) {
            printf("Erreur : La valeur %d est déjà présente sur la même colonne.\n", valeur);
            booleen = false;
        }
    }

    /** Vérifier le bloc */ 
    int debutLigneBloc = 3 * (numLigne / 3);
    int debutColonneBloc = 3 * (numColonne / 3);

    for (int i = debutLigneBloc; i < debutLigneBloc + 3; i++) {
        for (int j = debutColonneBloc; j < debutColonneBloc + 3; j++) {
            if (grille[i][j] == valeur) {
                printf("Erreur : La valeur %d est déjà présente dans le même bloc.\n", valeur);
                booleen = false;
            }
        }
    }

    return booleen;
}


/**
 * @fn grillePleine(t_grille grille)
 * @brief Vérifie si la grille est pleine.
 * @param grille La grille de Sudoku.
 * @return 1 si la grille est pleine, 0 sinon.
 * 
 * Vérifie si la grille est pleine pour pouvoir entrer dans la boucle while dans le main
 * en parcourant chaques valeurs de la grille pour voir si il y en a une à 0 (=case vide)
 */
int grillePleine(t_grille grille){
    int lig,col;
    int retour ;
    retour = 1;
    for (lig = 0 ; lig < TAILLE ; lig++)
    {
        for (col = 0 ; col < TAILLE ; col++)
        {
            if (grille[lig][col] == 0)
            {
                retour = 0;
            }
        }
    }
    return retour ;
}
