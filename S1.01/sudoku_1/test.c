#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define TAILLE 9

typedef int t_grille[TAILLE][TAILLE] ;


void afficherGrille(t_grille);
void chargerGrille(t_grille);
void saisir(int*);
int grillePleine(t_grille);


int main(){
    t_grille grille1 ;
    int val;
    chargerGrille(grille1);
    afficherGrille(grille1);
    saisir(&val) ;
}

void afficherGrille(t_grille grille) {
    // Afficher les numéros de colonne
    printf("     1  2  3   4  5  6   7  8  9\n");

    // Afficher le cadre supérieur de la grille
    printf("   +---------+---------+---------+\n");

    // Afficher la grille avec les numéros de ligne
    for (int i = 0; i < TAILLE; i++) {
        printf("%d  |", i + 1);
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 0) {
                printf(" . ");
            } else {
                printf(" %d ", grille[i][j]);
            }
            if (j % 3 == 2) {
                printf("|");
            }
        }
        printf("\n");

        // Afficher le séparateur horizontal après chaque 3 lignes
        if (i % 3 == 2) {
            printf("   +---------+---------+---------+\n");
        }
    }
}


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


void saisir(int *S, int n) {
    char input[100];
    int value;

    do {
        printf("Veuillez saisir un entier entre 1 et %d : ", n * n);
        scanf("%s", input);

        if (sscanf(input, "%d", &value) != 0) {
            if (value >= 1 && value <= n * n) {
                *S = value;
                break;
            } else {
                printf("Erreur : La valeur doit être entre 1 et %d.\n", n * n);
            }
        } else {
            printf("Erreur : Veuillez saisir un entier valide.\n");
        }

        // Nettoyer le tampon d'entrée en cas d'erreur
        while (getchar() != '\n');

    } while (1);
}

/*

     1  2  3   4  5  6   7  8  9
    +---------+---------+---------+
 1  | .  .  . | .  .  . | .  .  . |
 2  | .  .  . | .  .  . | .  .  . |
 3  | .  .  . | .  .  . | .  .  . |
    +---------+---------+---------+
 4  | .  .  . | .  .  . | .  .  . |
 5  | .  .  . | .  .  . | .  .  . |
 6  | .  .  . | .  .  . | .  .  . |
    +---------+---------+---------+
 7  | .  .  . | .  .  . | .  .  . |
 8  | .  .  . | .  .  . | .  .  . |
 9  | .  .  . | .  .  . | .  .  . |
    +---------+---------+---------+

*/