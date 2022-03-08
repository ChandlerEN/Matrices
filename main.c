#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULLE       0
#define IDENTITE    1
#define HASARD      2

#define ENTIER      0
#define DECIMAL     1

FILE* Matrice_save;

//----------JALON_001----------//

typedef struct
{
    unsigned int    dimX;
    unsigned int    dimY;
    double**        tab2D;
    char*           nom;
}Matrice;

//----------JALON_016----------//

double calculerDeterminantMatrice (Matrice*);       // Declaration "prototype" de la fonction "calculerDeterminantMatrice"

//----------JALON_002----------//

Matrice* creerMatrice (char* nom_c, unsigned int X, unsigned int Y, int type)
{
    Matrice* c_m = (Matrice*) malloc (sizeof(Matrice));

    if (c_m == NULL)
    {
        printf ("Erreur de malloc \n");
        exit(1);
    }

    c_m->dimX     = X;
    c_m->dimY     = Y;
    c_m->nom      = nom_c;
    c_m->tab2D    = (double*) calloc (Y, sizeof (double));

    for (int i = 0; i < Y; i++){
        c_m->tab2D[i] = (double*) calloc (X, sizeof (double));
    }

    switch (type){
        case IDENTITE   : for (int i = 0; i < Y; i++){
                            for (int y = 0; y < X; y++){
                                if (i == y){
                                    c_m->tab2D[i][y] = 1;
                                }
                            }
                          } break;

        case HASARD     : for (int i = 0; i < Y; i++){
                            for (int y = 0; y < X; y++){
                                c_m->tab2D[i][y] = rand () % 101;
                            }
                          } break;
    }

    return c_m;
}

//----------JALON_003----------//

void detruireMatrice (Matrice* d_m)
{
    if (d_m){
        if (d_m->tab2D){
            for (int i = 0; i < d_m->dimY; i++){
                if (d_m->tab2D[i]){
                    free(d_m->tab2D[i]);
                }
            }
            free(d_m->tab2D);
        }
        free(d_m);
    }
}

//----------JALON_004----------//

void afficherMatrice (Matrice* a_m, int format)
{
    printf ("\nMatrice : %s \n", a_m->nom);

    printf ("_________");                       //
                                                //
    for (int i = 0; i < a_m->dimX; i++){        //
        printf("    %d   ",i);                  //  Cosmetique
    }                                           //
                                                //
    printf("\n");                               //

    for (int i = 0; i < a_m->dimY; i++){
        printf(" %d  |   ",i);
        for (int y = 0; y < a_m->dimX; y++){
            if (format == ENTIER){
                printf (" [%5.0lf]", a_m->tab2D[i][y]);
            }
            else if (format == DECIMAL){
                printf (" [%5.2lf]", a_m->tab2D[i][y]);
            }
        }
        printf ("\n");
    }
}

//----------JALON_005----------//

char* getNom (Matrice* gn_m)
{
    return gn_m->nom;
}

//----------JALON_006----------//

void setNom (Matrice* sn_m, char* nom)
{
    sn_m->nom = nom;
}

//----------JALON_007----------//

unsigned int getDimX (Matrice* gdx_m)
{
    return gdx_m->dimX;
}

unsigned int getDimY (Matrice* gdy_m)
{
    return gdy_m->dimY;
}

//----------JALON_008----------//

unsigned int getElement (Matrice* ge_m, unsigned int X, unsigned int Y, double* valeur)
{
    if (Y <= ge_m->dimY && X <= ge_m->dimX)
    {
        *valeur = ge_m->tab2D[Y][X];

        return 1;
    }
    else{
        return 0;
    }
}

//----------JALON_009----------//

unsigned int setElement (Matrice* ge_m, unsigned int X, unsigned int Y, double valeur)
{
    if (Y >= 0 && Y <= ge_m->dimY && X >= 0 && X <= ge_m->dimX)
    {
        ge_m->tab2D[Y][X] = valeur;

        return 1;
    }
    else{
        return 0;
    }
}

//----------JALON_010----------//

void setDimX (Matrice* sdx_m, unsigned int X)
{
    double** temp = (double*) calloc (sdx_m->dimY, sizeof(double));

    for (int i = 0; i < sdx_m->dimY; i++){
        temp[i] = (double*) calloc (X, sizeof (double));
    }

    for (int i = 0; i < sdx_m->dimY; i++){
        for (int j = 0; j < sdx_m->dimX; j++){
            if (j < X){
                temp[i][j] = sdx_m->tab2D[i][j];
            }
        }

        if (sdx_m->tab2D[i]){
            free(sdx_m->tab2D[i]);
            sdx_m->tab2D[i] = temp[i];
        }
    }

    sdx_m->dimX = X;
}

void setDimY (Matrice* sdy_m, unsigned int Y)
{
    double** temp = (double*) calloc (Y, sizeof(double));

    for (int i = 0; i < Y; i++){
        temp[i] = (double*) calloc (sdy_m->dimX, sizeof (double));
    }

    for (int i = 0; i < sdy_m->dimY; i++){
        if (i < Y){
            for (int j = 0; j < sdy_m->dimX; j++){
                temp[i][j] = sdy_m->tab2D[i][j];
            }
        }
        if (sdy_m->tab2D[i]){
            free(sdy_m->tab2D[i]);
        }
    }

    if (sdy_m->tab2D){
        free(sdy_m->tab2D);
        sdy_m->tab2D = temp;
    }

    sdy_m->dimY = Y;
}

//----------JALON_011----------//

Matrice* transposerMatrice (Matrice* t_m, char* nom)
{
    Matrice* m = creerMatrice(nom, t_m->dimY, t_m->dimX, NULLE);

    for (int i = 0; i < m->dimY; i++){
        for (int y = 0; y < m->dimX; y++){
            m->tab2D[i][y] = t_m->tab2D[y][i];
        }
    }

    return m;
}

//----------JALON_012----------//

Matrice* additionnerMatrice (Matrice* a1_m, Matrice* a2_m, char* nom)
{
    if (a1_m->dimX == a2_m->dimX && a1_m->dimY == a2_m->dimY){

        Matrice* r_m = creerMatrice(nom, a1_m->dimX, a1_m->dimY, NULLE);

        for (int i = 0; i < r_m->dimY; i++){
            for (int j = 0; j < r_m->dimX; j++){
                r_m->tab2D[i][j] = (a1_m->tab2D[i][j] + a2_m->tab2D[i][j]);
            }
        }

        return r_m;
    }
    else{
        printf("\nErreur fonction addition.\nLe format des matrices n'est pas identique.\nVeuillez modifier la taille d'une de ces dernieres.\n");

        return 0;
    }
}

//----------JALON_013----------//

Matrice* soustraireMatrice (Matrice* s1_m, Matrice* s2_m, char* nom)
{
    if (s1_m->dimX == s2_m->dimX && s1_m->dimY == s2_m->dimY){

        Matrice* r_m = creerMatrice(nom, s1_m->dimX, s1_m->dimY, NULLE);

        for (int i = 0; i < r_m->dimY; i++){
            for (int j = 0; j < r_m->dimX; j++){
                r_m->tab2D[i][j] = (s1_m->tab2D[i][j] - s2_m->tab2D[i][j]);
            }
        }

        return r_m;
    }
    else{
        printf("\nErreur fonction soustraction.\nLe format des matrices n'est pas identique.\nVeuillez modifier la taille d'une de ces dernieres.\n");

        return 0;
    }

}

//----------JALON_014----------//

Matrice* multiplierMatriceParUnReel (Matrice* mr_m, double valeur, char* nom)
{
    Matrice* r_m = creerMatrice(nom, mr_m->dimX, mr_m->dimY, NULLE);

    for (int i = 0; i < r_m->dimY; i++){
        for (int j = 0; j < r_m->dimX; j++){
            r_m->tab2D[i][j] = (mr_m->tab2D[i][j] * valeur);
        }
    }

    return r_m;
}

//----------JALON_015----------//

Matrice* multiplierMatrice (Matrice* m1_m, Matrice* m2_m, char* nom)
{
    if (m1_m->dimX == m2_m->dimY){

        Matrice* r_m = creerMatrice(nom, m2_m->dimX, m1_m->dimY, NULLE);

        for (int i = 0; i < m1_m->dimY; i++){
            for (int j = 0; j < m2_m->dimX; j++){
                for (int v = 0; v < m1_m->dimX; v++){
                    r_m->tab2D[i][j] = r_m->tab2D[i][j] + (m1_m->tab2D[i][v] * m2_m->tab2D[v][j]);
                }
            }
        }
        return r_m;
    }
    else{
        printf("\nErreur fonction multiplication.\nLe format des matrices n'est pas de (m,n) x (n,p) = (m,p).\nVeuillez modifier la taille d'une de ces dernieres.\n");

        return 0;
    }
}

//----------JALON_016----------//

double calculerCofacteur (Matrice* cc_m, unsigned int x, unsigned int y)
{
    double cofacteur = 0;
    int indice_y = 0;

    Matrice* c_m1 = creerMatrice("c_m1", cc_m->dimX-1, cc_m->dimY-1, NULLE);

    for (int i = 0; i < cc_m->dimY; i++){
        int indice_x = 0;
        for (int j = 0; j < cc_m->dimX; j++){
            if (i != y && j != x){
                c_m1->tab2D[indice_y][indice_x] = cc_m->tab2D[i][j];
                indice_x++;
                if (indice_x == c_m1->dimX){    // ne pas confondre une valeur avec le nombre de case dans un tableau
                    indice_y++;
                }
            }
        }
    }

    if (cc_m->dimY > 2){                                 // Une d�claration d'un "prototype" de la fonction "calculerDeterminantMatrice" se trouve en haut de la page.
        cofacteur = calculerDeterminantMatrice(c_m1);    // Pas de "return" pour que la matrice temporaire "c_m1" puisse �tre d�truite et donc �viter la fuite de m�moire.
    }
    else{
        cofacteur = (c_m1->tab2D[0][0] * c_m1->tab2D[1][1]) - (c_m1->tab2D[0][1] * c_m1->tab2D[1][0]);
    }

    detruireMatrice(c_m1);

    return cofacteur;
}

double calculerDeterminantMatrice (Matrice* cd_m)
{
    if (cd_m->dimY != cd_m->dimX || cd_m->dimX < 2){

        printf("\nErreur fonction Determinant.\nLa matrice n'est pas carree ou sa taille est trop petite.\nVeuillez modifier la taille de cette derniere.\n");

        return 0;
    }

    double tab_determinant [cd_m->dimX];
    double determinant = 0;

    if (cd_m->dimY == 2){
        determinant = (cd_m->tab2D[0][0] * cd_m->tab2D[1][1]) - (cd_m->tab2D[0][1] * cd_m->tab2D[1][0]);
    }
    else{
        for (int i = 0; i < cd_m->dimX; i++){
            tab_determinant[i] = cd_m->tab2D[0][i] * calculerCofacteur(cd_m, i, 0);
        }

        for (int i = 0; i < cd_m->dimX; i++){
            if (i % 2){
                determinant = determinant - (tab_determinant[i]);
            }
            else{
                    determinant = determinant + (tab_determinant[i]);
            }
        }
    }

    return determinant;
}

//----------JALON_017----------//

Matrice* inverserMatrice (Matrice* i_m, char* nom)
{
    if (calculerDeterminantMatrice(i_m) == 0 || i_m->dimX != i_m->dimY){

        printf("\nErreur fonction Inverser.\nLe determinant de la matrice est egal a 0 et n'est donc pas inversible.\nOu la matrice n'est pas carree\n");

        return 0;
    }

    Matrice* inv_m = creerMatrice("inverse", i_m->dimX, i_m->dimY, NULLE);

    if (i_m->dimY == 2 && i_m->dimX == 2){
        inv_m->tab2D[0][0] =  i_m->tab2D[1][1];
        inv_m->tab2D[0][1] = -i_m->tab2D[0][1];
        inv_m->tab2D[1][0] = -i_m->tab2D[1][0];
        inv_m->tab2D[1][1] =  i_m->tab2D[0][0];

        for (int i = 0; i < inv_m->dimY; i++){
            for (int j = 0; j < inv_m->dimX; j++){
                inv_m->tab2D[i][j] = ( 1 / calculerDeterminantMatrice(i_m) ) * inv_m->tab2D[i][j];
            }
        }
    }
    else{
        for (int i = 0; i < i_m->dimY; i++){
            for (int j = 0; j < i_m->dimX; j++){
                if (j % 2){
                    if (i % 2){
                        inv_m->tab2D[i][j] = +calculerCofacteur(i_m, j, i);
                    }
                    else{
                        inv_m->tab2D[i][j] = -calculerCofacteur(i_m, j, i);
                    }
                }
                else{
                    if (i % 2){
                        inv_m->tab2D[i][j] = -calculerCofacteur(i_m, j, i);
                    }
                    else{
                        inv_m->tab2D[i][j] = +calculerCofacteur(i_m, j, i);
                    }
                }
            }
        }
        inv_m = transposerMatrice(inv_m, nom);
        for (int i = 0; i < inv_m->dimY; i++){
            for (int j = 0; j < inv_m->dimX; j++){
                inv_m->tab2D[i][j] = ( 1 / calculerDeterminantMatrice(i_m) ) * inv_m->tab2D[i][j];
            }
        }
    }

    return inv_m;
}

//----------JALON_018----------//

Matrice* diviserMatrice (Matrice* d1_m, Matrice* d2_m, char* nom)
{
    if (d1_m->dimX == d2_m->dimY && d2_m->dimX == d2_m->dimY){

        Matrice* inv_m2 = inverserMatrice(d2_m, nom);

        Matrice* r_m = multiplierMatrice(d1_m, inv_m2, nom);

        detruireMatrice(inv_m2);

        return r_m;
    }
    else{
        printf("\nErreur fonction diviser.\nLe format des matrices n'est pas de (m,n) x (n,p) = (m,p) ou la deuxieme matrice n'est pas carree.\nVeuillez modifier la taille d'une de ces dernieres.\n");

        return 0;
    }
}

//----------JALON_019----------//

void ecrireDansFichierMatrice (Matrice* m, FILE* fichier)
{
    fichier = fopen("Matrice.txt", "a");    // Utiliser le mode "w" pour effacer le contenu du fichier � chaque appel de la fonction et �crire les nouvelles matrices � la place.

    if (fichier == NULL)
    {
        printf("Erreur, impossible de creer le fichier 'Matrice.txt' ou d'ecrire dans ce dernier.\n");
        exit(1);
    }

    fprintf(fichier, "[\n");
    fprintf(fichier, "%s\n", m->nom);
    fprintf(fichier, "%d\n", m->dimX);
    fprintf(fichier, "%d\n", m->dimY);

    for (int i = 0; i < m->dimY; i++){
        for (int j = 0; j < m->dimX; j++){
            fprintf(fichier, "%.2lf ", m->tab2D[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fprintf(fichier, "]\n\n");

    fclose(fichier);

    printf("La matrice a ete sauvegardee dans le fichier 'Matrice.txt' du projet. \n");
}

//----------JALON_020----------//

Matrice* lireDepuisFichierMatrice (FILE* fichier)
{
    fichier = fopen("Matrice.txt", "r");

    if (fichier == NULL)
    {
        printf("Erreur, impossible de lire le fichier 'Matrice.txt'.\nVerifie que le fichier existe et soit present dans la racine du projet.");
        exit(1);
    }

    const char numbers [] = "0123456789";
    const char letters [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char start[100];
    char* d_end;
    int i = 0;
    int j = 0;

    char*           nom = (char*) malloc (sizeof(void*));
    unsigned int    dim_X = 0;
    unsigned int    dim_Y = 0;

    while (fgets(start, 100, fichier) != NULL){
        if (strstr(start, "[") != NULL){
            printf("%c \n", start[0]);

            while (fgets(start, 100, fichier) != NULL && strstr(start, "]") == NULL){
                if (strpbrk(start, letters) != NULL){
                    strcpy(nom, start);
                    printf("%s", nom);

                    while (fgets(start, 100, fichier) != NULL && strstr(start, "]") == NULL){
                        if (strpbrk(start, numbers) != NULL && strstr(start, ".") == NULL){
                            dim_X = atoi(start);
                            printf("dim_X = %d \n", dim_X);

                            while (fgets(start, 100, fichier) != NULL && strstr(start, "]") == NULL){
                                if (strpbrk(start, numbers) != NULL && strpbrk(start, ".") == NULL){
                                    dim_Y = atoi(start);
                                    printf("dim_Y = %d \n", dim_Y);

                                    Matrice* m = creerMatrice(nom, dim_X, dim_Y, NULLE);

                                    while (fgets(start, 100, fichier) != NULL && strstr(start, "]") == NULL){
                                        if (strpbrk(start, numbers) != NULL){
                                            if (strlen(start) < (m->dimX * 5)){
                                                printf("Le contenu de la matrice ne correspond pas a sa dimension X. \nIl manque une ou plusieurs colonne(s) / case(s) a la matrice.");

                                                detruireMatrice(m);

                                                return NULL;
                                            }
                                            for (int j = 0; j < m->dimX; j++){
                                                if (j == 0){
                                                    m->tab2D[i][j] = strtod(start, &d_end);
                                                }
                                                else{
                                                    if (j == m->dimX){
                                                        m->tab2D[i][j] = strtod(d_end, NULL);
                                                    }
                                                    else{
                                                        m->tab2D[i][j] = strtod(d_end, &d_end);
                                                    }
                                                }
                                                printf("%5.2lf ", m->tab2D[i][j]);
                                            }
                                            printf("\n");
                                            i++;
                                            if (i == m->dimY){
                                                while (fgets(start, 100, fichier) != NULL && strstr(start, "[") == NULL){
                                                    if (strstr(start, "]") != NULL){
                                                        printf("%s\n", start);

                                                        return m;
                                                    }
                                                }
                                                printf("Il manque le chevron fermant ']' dans le fichier 'Matrice.txt'. \n");

                                                detruireMatrice(m);

                                                return NULL;
                                            }
                                        }
                                    }
                                    printf("Le contenu de la matrice ne correspond pas a sa dimension Y. \nIl manque une ou plusieurs ligne(s) a la matrice.");

                                    detruireMatrice(m);

                                    return NULL;
                                }
                            }
                            printf("Les dimensions sont incorrectes.\nVeuillez verifiez que X et Y soit bien present dans le fichier 'Matrice.txt'. \n");

                            return NULL;
                        }
                    }
                    printf("Les dimensions sont incorrectes.\nVeuillez verifiez que X et Y soit bien present dans le fichier 'Matrice.txt'. \n");
                    return NULL;
                }
            }
            printf("Il manque le nom de la matrice dans le fichier 'Matrice.txt'. \n");
            return NULL;
        }
    }
    printf("Aucune matrice n'a ete trouvee dans le fichier 'Matrice.txt'.\nVerifiez que le chevron ouvrant '[' soit bien present. \n");
    return NULL;
}

//--------------------------------------------------------------------------------------//

int main()
{
    srand(time(NULL));

    Matrice* mat = lireDepuisFichierMatrice(Matrice_save);

    afficherMatrice(mat, DECIMAL);

    return 0;
}
