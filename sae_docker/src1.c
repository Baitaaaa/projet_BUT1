/**
 * @BRIEF
 * @AUTHOR = Mateo Morvan
 * @CLASSE 1D1
 * DATE = 23 11 2023
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const char ETOILE = '*';
#define TAILLE_MAX 15
typedef char tMot[TAILLE_MAX + 1];  // un octet en plus pour '*'

void init(tMot m){
    int i;  // i va être notre compteur pour connaitre la case
    for (i = 0; i < (TAILLE_MAX + 1); i++) // toujours un octet en plus pour '*'
    {
        m[i] = ETOILE; // fait que la case soit égal à ETOILE
    }
}

void  lireMot(tMot m){
    char car;
    char entree;
    int i; 
    i = 0;// i est encore notre compteur pour connaitre la case
    while ((car != ETOILE) && (i < TAILLE_MAX)) // tant que le caractère dans la case n'est pas égale à '*' le programme va continuer à s'executer 
    {
        printf("rentrez un caractère : ");
        scanf("%c%c", &car, &entree);  // 2 %c pour "absober" la touche entrée
        m[i] = car;
        i=i+1; // on rajoute 1 à i pour pourvoir avancer dans les cases et sortir de la boucle
    }
}

void affiche(tMot m){
    int i;
    i=0; // i va nous servir de compteur
    while (m[i] != ETOILE) // de même que précedemment, que le caractère dans la case n'est pas égale à '*' le programme va continuer à s'executer 
    {
        printf("%c", m[i]); // on écrit le caractère de la case
        i=i+1; // on rajoute 1 à i pour pourvoir avancer dans les cases et sortir de la boucle
    }
    printf("\n");
}

int longueur(tMot m){
    int taille, i; // i va nous servir de compteur et taille va nous indiquer la taille du mot
    i = 0;
    taille = 0;
    while (m[i] != ETOILE)
    {
        taille = taille + 1; // on  ajoute 1 à la taille du mot
        i = i + 1;
    }
    return taille; // on renvoie la taille du mot
}

void testelongueur(){ // va nous permettre de tester la fonction longueur
    int taille;
    tMot m1 = {'*'};
    tMot m2 = {'C', 'H', 'I', 'E', 'N', '*'};
    tMot m3 = {'B', 'O', 'A', '*'};
    tMot m4 = {'B', 'O', 'A', ' ', 'C', 'O', 'N', 'S', 'T', 'R', 'T', 'C', 'T', 'O', 'R', '*'}; // on crée différent tableau avec déjà des caractères à l'intérieur
    taille = longueur(m1);
    printf("%d \n", taille);
    taille = longueur(m2);
    printf("%d\n", taille);
    taille = longueur(m3);
    printf("%d\n", taille);
    taille = longueur(m4);
    printf("%d\n", taille); // on test notre fonction longueur et on écrit son résultat
}

bool estDans(tMot m, char c){
    bool res; // va enregistrer true ou false et le renvoyer 
    int stop; // va nous permettre de s'arrêter quand on aura res = true / m[i] == c
    int i; // i est notre compteur
    stop = 1;
    for (i = 0; i < TAILLE_MAX; i++)
    {
        if(stop != 0) // arrête tout si stop = 0
        {
            if(m[i] != c)
            {
                res = false;
            }
            if(m[i] == c)
            {
                res = true;
                stop = 0;
            }
        }
    }
    return res;
}

int compare(tMot m1, tMot m2){
    bool res;
    int i; // i est notre compteur
    int stop;
    stop = 1;
    for (i = 0; i < TAILLE_MAX; i++)
    {
        if(stop != 0) // arrête tout si stop = 0
        {
            if(m1[i] == m2[i])
            {
                res = true;
            }else if(m1[i] != m2[i])
            {
                res = false;
                stop = 0;
            }
        }
    }
    return res;
}

int main(){
    char car = 'F'; // caractère 'F' pour la fonction estDans afin de la tester
    bool res; 
    tMot m; // on définie un tableau m dans le main
    tMot m1 = {'C', 'H', 'I', 'E', 'N', '*'};
    tMot m2 = {'C', 'H', 'I', 'E', 'N', '*'};
    init(m);
    lireMot(m);
    affiche(m);
    testelongueur(); // fait un test pour la longueur
    res = estDans(m, car);
    if (res == true) // va nous permettre d'afficher true ou false en fonction de la fonction estDAns
    {
        printf("true\n");
    }else{
        printf("false\n");
    }
    res = compare(m1, m2);
    if (res == true) // va nous permettre d'afficher true ou false en fonction de la fonction estDAns
    {
        printf("true\n");
    }else{
        printf("false\n");
    }
    return 0;
}