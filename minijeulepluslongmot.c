#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_WORD_LENGTH 10
#define INDEX_FILE "index.txt"


typedef struct Node {
    char mot[100];
    struct Node *suivant;
} Node;

// Fonction pour cr�er un nouveau n�ud
Node* creerNoeud(char mot[]) {
    Node *nouveauNoeud = (Node*)malloc(sizeof(Node));
    if (nouveauNoeud == NULL) {
        printf("Erreur: Impossible de cr�er un nouveau n�ud.\n");
        exit(1);
    }
    strcpy(nouveauNoeud->mot, mot);
    nouveauNoeud->suivant = NULL;
    return nouveauNoeud;
}



///////////////TIMING///////////////

void timing()
{

  int seconds = 30;

  while (seconds > 0)
  {



    int s = seconds % 60;

    printf("\ril vous reste : %02d", s);

    fflush(stdout);

    clock_t stop = clock() + CLOCKS_PER_SEC;

    while (clock() < stop) { }

    seconds--;





  }

  printf("\rentrez maintenant!                      \n");}


//////////////////////CREATEINDEXFILE////////////////////



// Fonction pour cr�er un fichier d'index a partir du fichier de dictionnaire
void createIndexFile() {
    // Ouvrir le fichier du dictionnaire en mode lecture
    FILE *dictionaryFile = fopen("dictionnaire.txt", "r");

    // Ouvrir le fichier d'index en mode �criture
    FILE *indexFile = fopen("index.txt", "w");

    // V�rifier si l'ouverture des fichiers a r�ussi
    if (dictionaryFile == NULL || indexFile == NULL) {
        printf("Erreur lors de l'ouverture des fichiers");
        return;
    }

    long pos = 0; // Variable pour stocker la position courante dans le fichier
    char Letter = '\0'; // Variable pour stocker la lettre en cours de traitement

    // Boucle principale pour parcourir le fichier du dictionnaire
    while (1) {
        char word[100]; // Tableau pour stocker un mot du dictionnaire

        // Lire un mot du fichier du dictionnaire
        if (fscanf(dictionaryFile, "%s", word) == EOF) {
            break; // Sortir de la boucle si la fin du fichier est atteinte
        }

        // V�rifier si la premi�re lettre du mot est diff�rente de la lettre en cours
        if (word[0] != Letter) {
            // �crire la lettre et sa position dans le fichier d'index
            fprintf(indexFile, "%c %ld\n", word[0], pos);
            Letter = word[0]; // Mettre � jour la lettre en cours
        }

        pos = ftell(dictionaryFile); // Mettre � jour la position dans le fichier
    }


    fclose(dictionaryFile);
    fclose(indexFile);
}






//////////////////////SEARCHWORD////////////////



// Fonction pour rechercher un mot dans le fichier du dictionnaire en utilisant un fichier d'index
int searchWord(const char *word) {
    // Ouvrir le fichier d'index en mode lecture
    FILE *indexFile = fopen("index.txt", "r");

    // Ouvrir le fichier du dictionnaire en mode lecture
    FILE *dictionaryFile = fopen("dictionnaire.txt", "r");

    // V�rifier si l'ouverture des fichiers a r�ussi
    if (indexFile == NULL || dictionaryFile == NULL) {
        perror("Erreur lors de l'ouverture des fichiers");
        return -1; // Retourner une valeur indiquant une erreur
    }

    char firstLetter = word[0]; // Extraire la premi�re lettre du mot � rechercher
    long position = -1; // Variable pour stocker la position du mot dans le fichier du dictionnaire

    // Recherche dans l'index
    while (1) {
        char letter;
        long pos;

        // Lire une ligne de l'index contenant une lettre et sa position dans le fichier
        if (fscanf(indexFile, "%c %ld\n", &letter, &pos) == EOF) {
            break; // Sortir de la boucle si la fin du fichier d'index est atteinte
        }

        // V�rifier si la lettre correspond � la premi�re lettre du mot recherch�
        if (letter == firstLetter) {
            position = pos; // Mettre � jour la position du mot dans le fichier du dictionnaire
            break;
        }
    }

    // V�rifier si le mot a �t� trouv� dans l'index
    if (position != -1) {
        fseek(dictionaryFile, position, SEEK_SET); // D�placer le curseur du fichier du dictionnaire � la position du mot

        char currentWord[100]; // Variable pour stocker le mot actuellement lu dans le fichier du dictionnaire

        // Parcourir le fichier du dictionnaire � partir de la position trouv�e
        while (fscanf(dictionaryFile, "%s", currentWord) != EOF) {
            // Comparer le mot actuel avec le mot recherch�
            if (strcmp(currentWord, word) == 0) {
                //printf("Le mot %s a ete trouve.\n", word);
                fclose(indexFile);
                fclose(dictionaryFile);
                return 1; // Retourner 1 pour indiquer que le mot a �t� trouv�
            }
        }
    } else {
        printf("Le mot %s n a pas ete trouv�.\n", word);
    }

    // Fermer les fichiers
    fclose(indexFile);
    fclose(dictionaryFile);

    return 0; // Retourner 0 pour indiquer que le mot n'a pas �t� trouv�
}







////////////////////GENERATERANDOMLETTERS///////////////





// Fonction pour g�n�rer une s�quence al�atoire de 10 lettres avec un nombre sp�cifi� de voyelles
void generateRandomLetters(char *letters, int numVowels) {
    // D�finir des tableaux pour les voyelles et les consonnes
    const char vowels[] = "aeiouy";
    const char consonants[] = "bcdfghjklmnpqrstvwxz";

    // Calculer le nombre de consonnes en fonction du nombre total de lettres et de voyelles
    int numConsonants = 10 - numVowels;

    // Initialiser le g�n�rateur de nombres al�atoires avec le temps actuel
    srand(time(NULL));

    // G�n�rer des voyelles al�atoires
    for (int i = 0; i < numVowels; i++) {
        letters[i] = vowels[rand() % (sizeof(vowels) - 1)];
    }

    // G�n�rer des consonnes al�atoires
    for (int i = numVowels; i < 10; i++) {
        letters[i] = consonants[rand() % (sizeof(consonants) - 1)];
    }

    // M�langer les lettres en utilisant l'algorithme de Fisher-Yates
    for (int i = 9; i > 0; i--) {
        // G�n�rer un indice al�atoire dans la portion non m�lang�e restante
        int j = rand() % (i + 1);

        // �changer les lettres aux indices i et j
        char temp = letters[i];
        letters[i] = letters[j];
        letters[j] = temp;
    }

    // Ajouter un terminateur null � la fin de la s�quence de lettres g�n�r�e
    letters[10] = '\0';
}






//////////////////SOLVEURMOTLEPLUSLONG/////////////





// Fonction pour trouver le mot le plus long dans le dictionnaire en utilisant un ensemble de lettres al�atoires
void SolveurMotLePlusLong(char lettrealeatoire[]) {
    int valide, j, i;

    // Ouvrir le fichier du dictionnaire en mode lecture
    FILE *f = fopen("dictionnaire.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char s[100]; // Suppose la longueur maximale d'un mot est de 100 caract�res

    // Variables pour stocker les mots les plus longs trouv�s
    Node *meilleursMots = NULL;
    int meilleurLongueur = 0;

    // Tableau pour compter le nombre d'occurrences de chaque lettre dans le mot al�atoire
    int lettreAleatoireCount[26] = {0};

    // Remplir le tableau avec le nombre d'occurrences de chaque lettre dans le mot al�atoire
    for (i = 0; i < strlen(lettrealeatoire); i++) {
        if (lettrealeatoire[i] >= 'a' && lettrealeatoire[i] <= 'z') {
            lettreAleatoireCount[lettrealeatoire[i] - 'a']++;
        }
    }

    // Parcourir toutes les lignes du fichier du dictionnaire
    while (fgets(s, sizeof(s), f) != NULL) {
        int n = strlen(s); // Prendre en compte le caract�re de nouvelle ligne '\n'

        // Supprimer le caract�re de nouvelle ligne si pr�sent
        if (s[n - 1] == '\n') {
            s[n - 1] = '\0';
            n--;
        }

        // V�rifier si la longueur du mot est plus longue que la meilleure longueur actuelle
        if (n > meilleurLongueur) {
            int tempCounter = 0;
            char tempWord[100];
            strcpy(tempWord, s);

            valide = 1; // Initialiser valide � 1 au d�but de chaque it�ration

            // Tableau pour compter le nombre d'occurrences de chaque lettre dans le mot trouv�
            int tempWordCount[26] = {0};

            // Remplir le tableau tempWordCount avec les occurrences de chaque lettre dans tempWord
            for (i = 0; i < n; i++) {
                if (tempWord[i] >= 'a' && tempWord[i] <= 'z') {
                    tempWordCount[tempWord[i] - 'a']++;
                }
            }

            // Comparer les occurrences de chaque lettre dans tempWord avec celles de lettrealeatoire
            for (i = 0; i < 26; i++) {
                if (tempWordCount[i] > lettreAleatoireCount[i]) {
                    valide = 0;
                    break; // Sortir de la boucle d�s qu'une lettre non conforme est trouv�e
                }
            }

            // Si le mot est valide, le comparer avec les lettres al�atoires
            if (valide) {
                for (i = 0; i < n; i++) {
                    for (j = 0; j < strlen(lettrealeatoire); j++) {
                        if (lettrealeatoire[j] == tempWord[i]) {
                            tempWord[i] = '_';
                            tempCounter++;
                            break; // Sortir de la boucle int�rieure d�s qu'une correspondance est trouv�e
                        }
                    }
                }

                // Si toutes les lettres du mot sont utilis�es, mettre � jour les meilleurs mots
                if (tempCounter == n) {
                    meilleurLongueur = n;
                    // R�initialiser la liste des meilleurs mots
                    while (meilleursMots != NULL) {
                        Node *temp = meilleursMots;
                        meilleursMots = meilleursMots->suivant;
                        free(temp);
                    }
                    meilleursMots = creerNoeud(s);
                }
            }
        }
        // Si la longueur du mot est �gale � la meilleure longueur actuelle, ajouter le mot � la liste des meilleurs mots
        else if (n == meilleurLongueur) {
            int tempCounter = 0;
            char tempWord[100];
            strcpy(tempWord, s);

            valide = 1; // Initialiser valide � 1 au d�but de chaque it�ration

            // Tableau pour compter le nombre d'occurrences de chaque lettre dans le mot trouv�
            int tempWordCount[26] = {0};

            // Remplir le tableau tempWordCount avec les occurrences de chaque lettre dans tempWord
            for (i = 0; i < n; i++) {
                if (tempWord[i] >= 'a' && tempWord[i] <= 'z') {
                    tempWordCount[tempWord[i] - 'a']++;
                }
            }

            // Comparer les occurrences de chaque lettre dans tempWord avec celles de lettrealeatoire
            for (i = 0; i < 26; i++) {
                if (tempWordCount[i] > lettreAleatoireCount[i]) {
                    valide = 0;
                    break; // Sortir de la boucle d�s qu'une lettre non conforme est trouv�e
                }
            }

            // Si le mot est valide, le comparer avec les lettres al�atoires
            if (valide) {
                for (i = 0; i < n; i++) {
                    for (j = 0; j < strlen(lettrealeatoire); j++) {
                        if (lettrealeatoire[j] == tempWord[i]) {
                            tempWord[i] = '_';
                            tempCounter++;
                            break; // Sortir de la boucle int�rieure d�s qu'une correspondance est trouv�e
                        }
                    }
                }

                // Si toutes les lettres du mot sont utilis�es, ajouter le mot � la liste des meilleurs mots
                if (tempCounter == n) {
                    Node *nouveauNoeud = creerNoeud(s);
                    nouveauNoeud->suivant = meilleursMots;
                    meilleursMots = nouveauNoeud;
                }
            }
        }
    }

    // Fermer le fichier du dictionnaire
    fclose(f);

    // Afficher les meilleurs mots trouv�s ou indiquer qu'aucun mot n'a �t� trouv�
    if (meilleurLongueur > 0) {
        printf("Les mots les plus longs sont :\n");
        Node *courant = meilleursMots;
        while (courant != NULL) {
            printf("%s\n", courant->mot);
            courant = courant->suivant;
        }
    } else {
        printf("Aucun mot trouve.\n");
    }

    // Lib�rer la m�moire allou�e pour la liste cha�n�e
    Node *temp;
    while (meilleursMots != NULL) {
        temp = meilleursMots;
        meilleursMots = meilleursMots->suivant;
        free(temp);
    }
}

//////////MOTVALIDE//////


int motValide(char *motPropose, char *lettres) {
    int len_mot = strlen(motPropose);
    int len_lettres = strlen(lettres);
    int i, j;

    // Compte le nombre d'occurrences de chaque lettre dans le mot propos�
    int occurrences_mot[26] = {0};
    for (i = 0; i < len_mot; i++) {
        occurrences_mot[motPropose[i] - 'a']++;
    }

    // Compte le nombre d'occurrences de chaque lettre dans les lettres al�atoires
    int occurrences_lettres[26] = {0};
    for (j = 0; j < len_lettres; j++) {
        occurrences_lettres[lettres[j] - 'a']++;
    }

    // V�rifie si chaque lettre dans le mot propos� est pr�sente dans les lettres al�atoires et si le nombre d'occurrences ne d�passe pas celui dans les lettres al�atoires
    for (i = 0; i < 26; i++) {
        if (occurrences_mot[i] > occurrences_lettres[i]) {
            return 0; // Si une lettre appara�t plus de fois dans le mot propos� que dans les lettres al�atoires, le mot n'est pas valide
        }
    }

    // Si toutes les lettres du mot propos� sont pr�sentes dans les lettres al�atoires et n'utilisent pas plus d'occurrences, le mot est valide
    return 1;
}


///////////////PLAYGAME//////////////



void playGame() {

    int score1=0,score2=0;
    srand(time(NULL));

    for (int round = 1; round <= 8; round++) {
        int valide1=0,valide2=0;
        printf("\e[1;35m");
        printf("\t*** Manche %d***\n\n", round);

        int numVowels;
        do {
            printf("\e[1;36m");
            printf("Nombre de voyelles (minimum 2) : ");
            scanf("%d", &numVowels);
        } while (numVowels < 2 );

        char letters[10];
        generateRandomLetters(letters, numVowels);
        printf("\e[1;34m");

        printf("*** Lettres tirees : %s ***\n\n", letters);
        printf("---> le joueur 1 : entrer votre proposition \n ");
        timing();

        char playerWord[MAX_WORD_LENGTH + 1];

        scanf("%s", playerWord);

        if (searchWord(playerWord)&& motValide(playerWord, letters)) {
            printf("Mot valide !\n");
            valide1=1;

        } else {
            printf("Mot non valide !\n");
        }
        printf("---> le joueur 2 : entrer votre proposition\n ");
        timing();
        char playerWord2[MAX_WORD_LENGTH + 1];
        scanf("%s", playerWord2);
         if (searchWord(playerWord2)&& motValide(playerWord2, letters)) {
            printf("Mot valide !\n");
            valide2=1;

            printf("\e[1;31m");
            SolveurMotLePlusLong(letters);
        } else {
            printf("Mot non valide !\n");
            printf("\e[1;31m");
            SolveurMotLePlusLong(letters);

        }
        if((valide1==1)&&(valide2==1)){
        if ( strlen(playerWord)>strlen(playerWord2)){
            score1++;
        }
        else if(strlen(playerWord)<strlen(playerWord2)){
          score2++;

        }
        else {
            score1++;
            score2++;
        }}
         if(valide1==1 && valide2==0){
            score1++;
         }
         if(valide2==1 && valide1==0){
            score2++;
         }

    }

    if(score1==score2){
            printf("\e[1;32m");
        printf("\n\n [-- LE MEME SCORE --] \n");
        printf("score le joueur 1 --> %d \n",score1);
        printf("score le joueur 2 --> %d \n",score2);
    }
    else if (score1>score2){

                        printf("\e[1;32m");
                        printf("\n\n [-- VAINQUEUR --]--> joueur 1 \n  ");
                        printf("score le joueur 1 --> %d \n",score1);
                        printf("score le joueur 2 --> %d \n",score2);


    }
    else {

                        printf("\e[1;32m");
                        printf("\n\n [-- VAINQUEUR --]--> joueur 2 \n  "); //le mot s'affiche et le joueur gagne la partie
                        printf("score le joueur 1 --> %d \n",score1);
                        printf("score le joueur 2 --> %d \n",score2);
    }
    printf("\e[1;32m");
    printf("\n\n [-- FIN --] \n");
}
















int main() {
     system("color F0");  //pour arriere plan blanc brillant et l'ecriture en bleu
    printf("\e[1;94m");
    printf("     *----------------------------------------------------------------* \n");
    printf("     *                                                                * \n");
    printf("     *                                                                * \n");
    printf("     *                                                                * \n");
    printf("     *                                                                * \n");
    printf("     *                                                                * \n");
    printf("     *            INFORMATIQUE 1-A-GP2                                * \n");
    printf("     *            ENSIT 2023/2024                                     * \n");
    printf("     *                                                                * \n");
    printf("     *                                                                * \n");
    printf("     *                                                                * \n");
    printf("     * ---------------------------------------------------------------* \n");
    printf("\e[1;30m");
    printf("\n please wait");
    sleep(1);
    printf(".");
    sleep(1);
    printf(".");
    sleep(1);
    printf(".");
    sleep(1);
    system("cls");
    printf("\e[1;94m");
    printf("===================================================================================== \n");
    printf("*                                                                                    *\n");
    printf("*                                   JEU LE PLUS LONG MOT :                           *\n");
    printf("*                                    MENU:                                           *\n");
    printf("*                                                                                    *\n");
    printf("===================================================================================== \n");

    printf("\n\n\n Choose: \n\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("  [1] Espace du jeu                                                                   \n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("  [2] Description                                                                     \n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("  [3] QUITTER                                                                         \n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("\n\n -->  ");
    int ch1;
        scanf("%d",&ch1);
    switch(ch1)
    {
    case 1:
        sleep(2);
        system("cls");
        printf("\e[1;32m");
        printf("\n------------------------------------------------------------------\n");
        printf("\n **********************   Allez jouez   ************************ \n ");
        printf("\n------------------------------------------------------------------\n");

    createIndexFile();
    playGame();
    break ;
    case 2 :
        system("color F0");  //pour arrire plan blanc brillant et l'ecriture en bleu
    printf("\e[1;94m");
    printf("\n\n\n");
    printf("  *--------------------------------------------------------------------------------- *\n");
    printf("  |  Dans ce jeu, votre defi est de creer des mots a partir de lettres aleatoires.   |\n");
    printf("  |  Plus le mot est long, plus vous gagnez de points. Attention, le mot doit etre   |\n");
    printf("  |  reel et se trouver dans un dictionnaire. Utilisez votre creativite pour         |\n");
    printf("  |  trouver le mot le plus long et remportez la partie.                             |\n");
    printf("  *----------------------------------------------------------------------------------*\n");

    break ;
    case 3 :
    printf("\n A bientot  \n\n\n");
    exit(0);
    default:
        printf("\n !Essayons a nouveau!  \n");
    }
    return 0;
}
