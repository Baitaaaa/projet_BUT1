/***************************
 * INCLUDES
****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**************************
 * CONSTANTES SYMBOLIQUES
***************************/
#define N 3
#define TAILLE N * N

/*************
 * CONSTANTES
**************/
/**
 * @def on definit les sépateurs de lignes
 */
const char SEP[35] = "+----------+----------+----------+";
/**
 * @def on definit les sépateurs de colonnes
 */
const char COL[2] = "|";
const char SEPA[8] = "*******";

/**************
 * STRUCTURES
**************/
/**
 * @brief
 *
 */
typedef struct
{
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

/********************************
 * DEFINITIONS DES NOUVEAUX TYPES
*********************************/
typedef tCase1 tGrille[TAILLE][TAILLE];   /**Grille possibilités*/
typedef int t_grille_int[TAILLE][TAILLE]; /**Grille sudoku*/

/****************************************
 * PROTOTYPES DES FONCTIONS ET PROCEDURES 
*****************************************/

int chargerGrille(t_grille_int,char[30]);
void ajouterCandidat(tCase1*, int);
void initialiseCandidats(tGrille);
void init(t_grille_int, tGrille);
int possible(tGrille, int, int, int);
void afficherTableau(tGrille, int, int);
void retirerCandidat(tCase1*, int,float*);
void afficherStats(float, float,char[30]);
bool estCandidat(tCase1,int);
void singletonNu(tGrille,bool*,float[2]);
void singletonCache(tGrille,bool*,float[2]);
void afficherGrilleSudo(tGrille);

/*********************
 * PROGRAMME PRINCIPAL
**********************/
/**
 * @brief
 *
 * @return int
 */
int main()
{
    t_grille_int grilleInt;
    tGrille grilleSudoku;
    bool progression;
    float nbCasesVides;
    float nbCasesRemplies;
    char nomFichier[30];
    float tableauSingletonNu[2];
    float tableauSingletonCache[2];

    nbCasesVides = chargerGrille(grilleInt,nomFichier);
    init(grilleInt, grilleSudoku);
    initialiseCandidats(grilleSudoku);
    //afficherGrilleSudo(grilleSudoku);
    progression = true;
    while ((nbCasesVides != 0) && progression)
    {
        singletonNu(grilleSudoku,&progression,tableauSingletonNu);
        singletonCache(grilleSudoku,&progression,tableauSingletonCache);
        
        progression = false;
    }
    nbCasesVides = tableauSingletonCache[0] + tableauSingletonNu[0];
    nbCasesRemplies = tableauSingletonCache[1] + tableauSingletonNu[1];
    afficherStats(nbCasesVides,nbCasesRemplies,nomFichier);
    //afficherGrilleSudo(grilleSudoku);
}

/*************************
 * PROCEDURES ET FONCTIONS
**************************/
/**
 * @brief la procédure permet de rechercher les cases ou il n'y a qu'un seul candidat
 * 
 * @param grilleSudoku la grille des possibilitées
 * @param progression la progression dans la resolution de la case
 * @param nbCandEli le nombre de cases vides qui doit diminuer 
 * @param nbCasesRemplies le nombre de cases remplies au fur et à mesure
 */
void singletonNu(tGrille grilleSudoku,bool *progression, float tableauResult[2]){
    float sommeEli;
    sommeEli = 0;
    for (int boucleL = 0; boucleL < TAILLE; boucleL++){
        for (int boucleC = 0; boucleC < TAILLE; boucleC++){
            if (grilleSudoku[boucleL][boucleC].valeur == 0){
                if (grilleSudoku[boucleL][boucleC].nbCandidats == 1){
                    grilleSudoku[boucleL][boucleC].valeur = grilleSudoku[boucleL][boucleC].candidats[0];
                    retirerCandidat(&(grilleSudoku[boucleL][boucleC]), grilleSudoku[boucleL][boucleC].candidats[0],&sommeEli);
                    tableauResult[1]--;//caseVides
                    tableauResult[0]++;//case Remplies
                    *progression = true;
                }
            }
        }
    }
}

/**
 * @brief la procédure singleton caché permet de voir si une valeur apparait une autre fois dans un bloc, ligne, colonne 
 * 
 * @param grilleSudoku la grille de recherche 
 * @param progression la progression de la resolution 
 * @param nbCaseVides le nombre de cases vides 
 * @param nbRemplies le nombre de cases remplies
 */
void singletonCache(tGrille grilleSudoku,bool *progression,float tableauResult[2]){
    int tempCorrLig;
    int tempCorrCol;
    int nbOccurence;
    float sommeEli;
    
    sommeEli = 0;
    for (int ligne = 0; ligne < TAILLE; ligne = ligne + N){
        for (int col = 0; col < TAILLE; col = col + N){
            for (int i = 0; i < TAILLE; i++){
                tempCorrCol = -1;
                nbOccurence = 0;
                tempCorrLig = -1;
                for (int iBlocLig = ligne; iBlocLig < ligne + N; iBlocLig++){
                    for (int iBlocCol = col; iBlocCol < col + N; iBlocCol++){
                        if (grilleSudoku[iBlocLig][iBlocCol].valeur == 0 && estCandidat(grilleSudoku[ligne][col],i)){
                            nbOccurence++;
                            tempCorrCol = iBlocCol;
                            tempCorrLig = iBlocLig;
                        } 
                    }
                }
                if (nbOccurence == 1){
                    grilleSudoku[tempCorrLig][tempCorrCol].valeur = i;
                    retirerCandidat(&grilleSudoku[tempCorrLig][col],i,&sommeEli);
                    retirerCandidat(&grilleSudoku[ligne][tempCorrCol],i,&sommeEli);
                    tableauResult[1]--;//nbCases vides
                    tableauResult[0]++;
                    *progression = true;
                } 
            }  
        }   
    }
}
/**
 * @brief fonction permattant de charger la grille et de donner les casesVides et le nombre de cases de la grille
 *
 * @param g la grille chargée 
 * @param nbCase le nombre de cases que comporte la grille 
 * @return int
 */
int chargerGrille(t_grille_int g, char nomFichier[30])
{
    int nbCasesVide = 0;
    int boucleL;
    int boucleC;
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
    for (boucleC = 0; boucleC < TAILLE; boucleC++)
    {
        for (boucleL = 0; boucleL < TAILLE; boucleL++)
        {
            if (g[boucleC][boucleL] == 0)
            {
                nbCasesVide++;      
            }
        }
    }
    return nbCasesVide;
}

/**
 * @brief Procedure permettant l'affichage la grille de sudoku 
 * 
 * @param g la grille que nous voulons afficher
 */
void afficherGrilleSudo(tGrille g){
    int boucleNb;
    int boucleC;
    int boucleL;
    // espace entre le bord
    printf("     ");
    // on affiche la ligne du haut
    for (boucleNb = 0; boucleNb < TAILLE; boucleNb++)
    {
        printf("%d  ", boucleNb + 1);
        // on met un espace toutes les 3 valeurs avec le modulo
        if (((boucleNb + 1) % 3) == 0)
        {
            printf("  ");
        }
    }
    printf("\n");
    printf("  ");
    // la première ligne de séparateur
    printf("%s\n", SEP);
    for (boucleC = 0; boucleC < TAILLE; boucleC++)
    {
        printf("%d", boucleC + 1);
        printf(" %s", COL);
        for (boucleL = 0; boucleL < TAILLE; boucleL++)
        {
            // On regarde si la valeur vaut 0 si c'est le cas c'est un .
            if (g[boucleC][boucleL].valeur == 0)
            {
                printf("  .");
                // Dans le cas contraire on affiche la valeur
            }
            else
            {
                printf("  %d", g[boucleC][boucleL].valeur);
            }
            // On regarde aussi avec le modulo pour afficher les colonnes
            // Boucle + 1 car on veut commencer à 1
            if ((boucleL + 1) % 3 == 0)
            {
                printf(" %s", COL);
            }
        }
        printf("\n");
        // Puis une autre ligne de separateur
        if ((boucleC + 1) % 3 == 0)
        {
            printf("  %s\n", SEP);
        }
    }

}
/**
 * @brief procédure permetant d'initialiser toutes les cases avec leur tableau à 0 
 *
 * @param grilleIn la grille d'entrée avec les valeurs
 * @param grilleOut la grille de sortie avec les valeurs mise à jour
 */
void init(t_grille_int grilleIn, tGrille grilleOut)
{
    for (int boucleL = 0; boucleL < TAILLE; boucleL++){
        for (int boucleC = 0; boucleC < TAILLE; boucleC++){
            grilleOut[boucleL][boucleC].valeur = grilleIn[boucleL][boucleC];
            for (int iTabCandidats = 0; iTabCandidats < TAILLE; iTabCandidats++){
                grilleOut[boucleL][boucleC].candidats[iTabCandidats] = 0;
            }
            grilleOut[boucleL][boucleC].nbCandidats = 0;
        }
    }
}
/**
 * @brief procédure permetant d'initialiser les valeurs candidates ainsi que leur valeur
 *
 * @param grille la grille de sudoku
 * Cette procedure permet de mettre en candidat toutes les valeurs possibles grace à l'utilisation de la fonction possible
*/
void initialiseCandidats(tGrille grille){
    int boucleL;
    int boucleC;
    int candidat;
    for (boucleL = 0; boucleL < TAILLE; boucleL++) /**Parcours de la grille avec toutes les cases*/
    {
        for (boucleC = 0; boucleC < TAILLE; boucleC++)
        {
            if (grille[boucleL][boucleC].valeur == 0)
            {
                for (candidat = 1; candidat < TAILLE + 1; candidat++)
                {
                    if ((possible(grille, boucleL, boucleC, candidat)) == 1)
                    {
                        ajouterCandidat(&grille[boucleL][boucleC], candidat);
                    }
                }
            }
        }
    }
}
/**
 * @brief fonction possible permet de voir toutes les valeurs libres dans une case 
 *
 * @param grille la grille des possibilitées
 * @param numLigne le numéro de ligne 
 * @param numCol la numero de colonne
 * @param val la valeur 
 * @return 1 si la valeur peut être placée
*/
int possible(tGrille grille, int numLigne, int numCol, int val)
{
    // Initialisation des variables de boucles pour les colonnes et lignes
    int boucleC;
    int boucleL;
    // Initialisation de la valeur boolénne de retour
    int correct;
    // Initialisation des variables de boucles pour les blocs
    int blocCol;
    int blocLigne;
    int boucleBlocLig;
    int boucleBlocCol;
    correct = 1;
    boucleC = 0;
    // On regarde tant que la valeur est correcte
    while (boucleC < TAILLE && correct == 1)
    {
        boucleL = 0;
        // On regarde tant que la valeur est correcte
        while (boucleL < TAILLE && correct == 1)
        {
            // Affichage du premier message d'erreur si la valeur est présente sur la ligne
            if (grille[numLigne][boucleC].valeur == val)
            {
                correct = 0;
            }
            // Affichage du deuxième message d'erreur si la valeur est présente sur la colonne
            if (grille[boucleL][numCol].valeur == val)
            {
                correct = 0;
            }
            boucleL++;
        }
        boucleC++;
    }
    // Puis pour les blocs
    // Mise en place d'une relation pour récupérer les coordonnées de la première valeur du bloc
    blocLigne = (numLigne / 3) * 3;
    blocCol = (numCol / 3) * 3;
    boucleBlocLig = 0;
    boucleBlocCol = 0;
    // On parcourt les colonnes entre 0 et 3
    while (boucleBlocLig < 3 && correct == 1)
    {
        // On parcourt les lignes entre 0 et 3
        while (boucleBlocCol < 3 && correct == 1)
        {
            // Affichage du troisième message d'erreur si la valeur est présente dans le bloc
            if (grille[blocLigne + boucleBlocLig][blocCol + boucleBlocCol].valeur == val)
            {
                correct = 0;
            }
            boucleBlocCol++;
        }
        // On réinitialise la valeur pour passer à la colonne suivante
        boucleBlocCol = 0;
        boucleBlocLig++;
    }
    return correct;
}



/**
 * @brief la procedure retirer candidat permet de supprimer tous les candidats d'une case 
 * 
 * @param laCase la case en question sur lequel nous voulons supprimer les candidats
 * @param valeur la valeur que nous voulons supprimer
*/
void retirerCandidat(tCase1 *laCase, int valeur,float *somme)
{
    int boucle;
    bool estTrouvee;
    estTrouvee = false;
    for (boucle = 0; boucle < TAILLE; boucle++){
        if ((*laCase).candidats[boucle] == valeur){
            (*laCase).candidats[boucle] = 0;
            laCase->nbCandidats--;
            *somme ++;
            //estTrouvee = true;
        }
        if (estTrouvee && (boucle < TAILLE -1))
        {
            (*laCase).candidats[boucle] = laCase->candidats[boucle+1];
            (*laCase).candidats[boucle+1] = 0;
        }
        
    }
}

/**
 * @brief procédure permetant d'ajouter des candidats à la case associée
 *
 * @param laCase la case sur lequel nous voulons ajouter des candidats 
 * @param valeur la valeur que nous voulons lui ajouter 
 */
void ajouterCandidat(tCase1 *laCase, int valeur)
{
    int boucle;
    bool estPlace;
    estPlace = false;
    boucle = 0;
    while (boucle < TAILLE && estPlace == false)
    {
        if (laCase->candidats[boucle] == valeur) /**deja présente dans le tab */
        {
            estPlace = true;
        }
        else if ((*laCase).candidats[boucle] == 0) /**case vide*/
        {
            (*laCase).candidats[boucle] = valeur;
            (*laCase).nbCandidats++;
            estPlace = true;
        }
        else
        { /**passe autre case*/
            boucle++;
        }
    }
}
/**
 * @brief fonction pour savoir si une valeur est déjà présente
 *
 * @param laCase la case de recharcher de la valeur
 * @param val la valeur en question
 * @return false/true
 */
bool estCandidat(tCase1 laCase, int val)
{
    int boucle;
    bool candidat;
    candidat = false;
    for (boucle = 0; boucle < TAILLE; boucle++)
    {
        if (laCase.candidats[boucle] == val)
        {
            candidat = true;
        }
    }
    return candidat;
}
/**
 * @brief fonction permetant de nous donner le nombre de candidat d'une case
 *
 * @param laCase la case que nous recherchons
 * @return somme le nombre de candidats de la case
 */
int nbCandidats(tCase1 laCase)
{
    int boucle;
    int somme;
    somme = 0;
    for (boucle = 0; boucle < TAILLE; boucle++)
    {
        if (laCase.candidats[boucle] != 0)
        {
            somme++;
        }
    }
    return somme;
}


/**
 * @brief Procédure permetant de vérifier la bonne initialisation des candidats
 *
 * @param grille la grille des possibilité 
 * @param valeur la première borne 
 * @param valeur2 la deuxième borne 
 */
void afficherTableau(tGrille grille, int valeur, int valeur2)
{
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if ((i == valeur) && (j == valeur2))
            {
                for (int boucleTab = 0; boucleTab < TAILLE; boucleTab++)
                {
                    printf("%d\t", grille[valeur][valeur2].candidats[boucleTab]);
                }
            }
        }
    }
}

/**
 * @brief procedure afficherStat permet d'affciher les information sur la resolution de la grille
 * 
 * @param remplies le nombre de cases remplies
 * @param casesVides le nombre de case vides
 */
void afficherStats(float remplies, float elimine,char nomFichier[30])
{
    float tauxRemplissage;
    float tauxElimination;
    tauxRemplissage = 0;
    remplies = 0;
    //tauxRemplissage = (remplies/(TAILLE*TAILLE))*100.0;
    tauxElimination = elimine/100.0;
    printf("%s%s%s\n",SEPA,nomFichier,SEPA);
    printf("Nombre de cases remplies:%.0f sur %d\tTaux de remplissage:%.2f%%\n",remplies,(TAILLE*TAILLE),tauxRemplissage);
    printf("Nombre de candidats elimines:%f\tPourcentage d'elimination:%.2f%%\n",elimine*-1.0,tauxElimination*(-1.0));
}