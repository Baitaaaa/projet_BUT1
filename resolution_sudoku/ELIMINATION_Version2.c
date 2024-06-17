#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define N 3
#define TAILLE (N*N)

const char SEPA[8] = "*******";

typedef struct {
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
} tCase2;

typedef tCase2 tGrille[TAILLE][TAILLE]; // grille du sudoku avec toutes les possibilités
typedef int t_grille[TAILLE][TAILLE]; // grille du sudoku

int chargerGrille(t_grille,char[30]);
void afficherGrille(tGrille);
void init(t_grille, tGrille);
void initialiserCandidats(tGrille*);
void ajouterCandidat(tCase2*,int) ;
void retirerCandidat(tCase2*,int);
void retirerCandidatGrille(tGrille*,int,int,int);
bool estCandidat(tCase2*,int) ;
int possible(tGrille, int, int, int);
void afficherStats(int, int,char[30]);
void singletonNu(tGrille*,bool*,int*,int*);
// void singletonCache(tGrille,bool*,int*,int*);



            //######################//
            //#     PROGRAMME      #//
            //######################//


int main(){
    t_grille grilleInt;
    tGrille grilleSudoku;
    bool progression;
    int nbCasesVides;
    int nbCasesRemplies;
    char nomFichier[30];

    nbCasesVides = chargerGrille(grilleInt,nomFichier);
    printf("Nombres de cases vides : %d\n",nbCasesVides) ;
    init(grilleInt, grilleSudoku);
    initialiserCandidats(grilleSudoku);
    afficherGrille(grilleSudoku);
    progression = true;
    while ((nbCasesVides != 0) && progression)
    {
        singletonNu(&grilleSudoku,&progression,&nbCasesVides,&nbCasesRemplies);
        // singletonCache(grilleSudoku,&progression,&nbCasesVides,&nbCasesRemplies);
        afficherStats(nbCasesRemplies,nbCasesVides,nomFichier);
        
        progression = false;
    }
    //afficherGrille(grilleSudoku);
}



            //######################//
            //#     FONCTIONS      #//
            //######################//



void singletonNu(tGrille *grilleSudoku, bool *progression, int *nbCasesVides, int *nbCasesRemplies) {
    int nb;
    for (int boucleL = 0; boucleL < TAILLE; boucleL++) {
        for (int boucleC = 0; boucleC < TAILLE; boucleC++) {
            if ((*grilleSudoku)[boucleL][boucleC].valeur == 0 && (*grilleSudoku)[boucleL][boucleC].nbCandidats == 1) {
                for (int i = 1; i <= TAILLE; i++) {
                    if ((*grilleSudoku)[boucleL][boucleC].candidats[i]) {
                        (*grilleSudoku)[boucleL][boucleC].valeur = i;
                        nb = i;
                    }
                }
                (*nbCasesVides) -= 1;
                retirerCandidat(&((*grilleSudoku)[boucleL][boucleC]), nb);
                retirerCandidatGrille(&(*grilleSudoku),boucleL,boucleC,nb);
                (*nbCasesRemplies) += 1;
                *progression = true;
            }
        }
    }

}


// void singletonCache(tGrille grilleSudoku,bool *progression,int *nbCaseVides, int *nbRemplies){
//     int tempCorrLig;
//     int tempCorrCol;
//     int nbOccurence;
//     for (int ligne = 0; ligne < TAILLE; ligne = ligne + N){
//         for (int col = 0; col < TAILLE; col = col + N){
//             for (int i = 0; i < TAILLE; i++){
//                 tempCorrCol = -1;
//                 nbOccurence = 0;
//                 tempCorrLig = -1;
//                 for (int iBlocLig = ligne; iBlocLig < ligne + N; iBlocLig++){
//                     for (int iBlocCol = col; iBlocCol < col + N; iBlocCol++){
//                         if (grilleSudoku[iBlocLig][iBlocCol].valeur == 0 && estCandidat(&grilleSudoku[ligne][col],i)){
//                             nbOccurence++;
//                             tempCorrCol = iBlocCol;
//                             tempCorrLig = iBlocLig;
//                         } 
//                     }
//                 }
//                 if (nbOccurence == 1){
//                     grilleSudoku[tempCorrLig][tempCorrCol].valeur = i;
//                     *nbCaseVides --;
//                     *nbRemplies ++;
//                     retirerCandidat(&grilleSudoku[tempCorrLig][col],i);
//                     retirerCandidat(&grilleSudoku[ligne][tempCorrCol],i);
//                     *progression = true;
//                 } 
//             }  
//         }   
//     }
// }


int chargerGrille(t_grille g, char nomFichier[30]){
    int nb = 0 ;
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
    for (int i = 0 ; i < TAILLE ; i++){
        for (int j = 0 ; j < TAILLE ; j++){
            if(g[i][j] == 0)
            {
                nb += 1 ;
            }
        }
    }
    return nb ;
}


void afficherGrille(tGrille g){
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
    printf("%s\n", "+----------+----------+----------+");
    for (boucleC = 0; boucleC < TAILLE; boucleC++)
    {
        printf("%d", boucleC + 1);
        printf(" %s", "|");
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
                printf(" %s", "|");
            }
        }
        printf("\n");
        // Puis une autre ligne de separateur
        if ((boucleC + 1) % 3 == 0)
        {
            printf("  %s\n", "+----------+----------+----------+");
        }
    }

}


void initialiserCandidats(tGrille *g){
   for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            for (int v = 1; v <= TAILLE; v++) {
                (*g)[i][j].candidats[v] = false;
            }
            if ((*g)[i][j].valeur == 0) {
                for (int v = 1; v <= TAILLE; v++) {
                    if (possible(*g, i, j, v) == 1) {
                        (*g)[i][j].candidats[v] = true;
                        (*g)[i][j].nbCandidats ++ ;
                    }
                }
            }
        }
   }
}


int possible(tGrille grille, int numLigne, int numColonne, int valeur) {
    int booleen ;
    booleen = 1 ;
    /** Vérifier la ligne */
    for (int i = 0; i < TAILLE; i++) {
        if (grille[numLigne][i].valeur == valeur) {
            //printf("Erreur : La valeur %d est déjà présente sur la même ligne.\n", valeur);
            booleen = 0;
        }
    }

    /** Vérifier la colonne */       
    for (int i = 0; i < TAILLE; i++) {
        if (grille[i][numColonne].valeur == valeur) {
            //printf("Erreur : La valeur %d est déjà présente sur la même colonne.\n", valeur);
            booleen = 0;
        }
    }

    /** Vérifier le bloc */ 
    int debutLigneBloc = 3 * (numLigne / 3);
    int debutColonneBloc = 3 * (numColonne / 3);

    for (int i = debutLigneBloc; i < debutLigneBloc + 3; i++) {
        for (int j = debutColonneBloc; j < debutColonneBloc + 3; j++) {
            if (grille[i][j].valeur == valeur) {
                //printf("Erreur : La valeur %d est déjà présente dans le même bloc.\n", valeur);
                booleen = 0;
            }
        }
    }

    return booleen;
}


void init(t_grille grilleIn, tGrille grilleOut){

    for ( int boucleL = 0; boucleL < TAILLE; boucleL++)
    {
        for ( int boucleC  = 0; boucleC < TAILLE; boucleC++)
        {
            grilleOut[boucleL][boucleC].valeur = grilleIn[boucleL][boucleC];
            for (int iTabCandidats = 1; iTabCandidats < TAILLE + 1; iTabCandidats++)
            {
                grilleOut[boucleL][boucleC].candidats[iTabCandidats] = false;
            }
            grilleOut[boucleL][boucleC].nbCandidats = 0;
            
        }
        
    }

}


void ajouterCandidat(tCase2 *tcase, int nb){
    (*tcase).candidats[nb] = true ;
    (*tcase).nbCandidats ++ ;
}


void retirerCandidat(tCase2 *tcase,int nb){
    (*tcase).candidats[nb] = false ;
    (*tcase).nbCandidats -- ;
}


bool estCandidat(tCase2 *tcase,int nb){
    bool retour = false;
    if ((*tcase).candidats[nb])
    {
        retour = true ;
    }
    return retour ;
}


void afficherStats(int remplies, int casesVides,char nomFichier[30]){
    int totalGrille = TAILLE * TAILLE;
    int nbCasesRemplies = totalGrille - casesVides;
    float tauxRemplissage = (nbCasesRemplies / (float)totalGrille) * 100;
    int nbElimine = totalGrille - casesVides;  // Le total des candidats éliminés est le nombre total de cases - nombre de cases vides
    float tauxElimination = (nbElimine / (float)(totalGrille * (TAILLE + 1))) * 100;  // Le total des candidats possibles est TAILLE * TAILLE * (TAILLE + 1)

    printf("\n%s%s%s\n", SEPA, nomFichier, SEPA);
    printf("Nombre de cases remplies: %d sur %d\tTaux de remplissage: %.2f%%\n", nbCasesRemplies,totalGrille, tauxRemplissage);
    printf("Nombre de candidats éliminés: %d\tPourcentage d'élimination: %.2f%%\n", nbElimine, tauxElimination);
}


void retirerCandidatGrille(tGrille *grille, int numLigne, int numColonne, int valeur){
    /** Vérifier la ligne */
    for (int i = 0; i < TAILLE; i++) {
        if ((*grille)[numLigne][i].candidats[valeur]) {
            (*grille)[numLigne][i].candidats[valeur] = false ;
        }
    }

    /** Vérifier la colonne */       
    for (int i = 0; i < TAILLE; i++) {
        if ((*grille)[i][numColonne].candidats[valeur]) {
           (*grille)[i][numColonne].candidats[valeur] = false ;
           
        }
    }

    /** Vérifier le bloc */ 
    int debutLigneBloc = 3 * (numLigne / 3);
    int debutColonneBloc = 3 * (numColonne / 3);

    for (int i = debutLigneBloc; i < debutLigneBloc + 3; i++) {
        for (int j = debutColonneBloc; j < debutColonneBloc + 3; j++) {
            if ((*grille)[i][j].candidats[valeur]) {
               (*grille)[i][j].candidats[valeur] = false ;
                
            }
        }
    }
}
