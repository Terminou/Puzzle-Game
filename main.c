#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<string.h>

typedef struct
{
    char *word; //word and corresponding hint
    char *clue;
    int x; //Starting x and y positions
    int y;
    char direction; //H for horizontal, V for vertical
    int f; //solved or not
} Word_t;

Word_t* loadTextFile( FILE* myFile, int nrWords );
void displayBoard(int rows, int cols, char** myBoard);
int isBoardFilled(int rows, int cols, char** myBoard);
char** createArray(int rows, int cols);
int findIndex(int arr[], int size, int val);
void playGame(char** myBoard, int nrwords, Word_t *words, int x, int y, int countToFinish);
char** updateBoard(char** myBoard, Word_t *words, int solve);


int main()
{

    char** board = (char **)malloc(sizeof(char *));
    FILE* filep;
    int numOfrows, numOfcolumns, numOfwords;
    Word_t* words;
    printf("Enter name of text file: ");
    char fname [20];
    scanf("%s",fname);
    filep = fopen(fname,"r");
    if (filep == NULL)
    {
        printf("File could not be opened. ");
        while (filep == NULL)
        {
            printf("Please try again: ");
            scanf("%s",fname);
            filep = fopen(fname,"r");
            if (filep != NULL)
            {
                break;
            }
        }

    }
    fscanf(filep, "%d %d %d", &numOfrows, &numOfcolumns, &numOfwords );
    words = (Word_t*)malloc(sizeof(Word_t)*numOfwords);
    words = loadTextFile(filep, numOfwords);
    fclose(filep);
    printf("Game is %d rows x %d cols with %d words\nWords and clues are read!\n",
           numOfrows,numOfcolumns,numOfwords);
    board = createArray(numOfrows, numOfcolumns);
    playGame(board,numOfwords,words,numOfrows,numOfcolumns,0);
    free(board);
    free(words);
    return 0;
}

Word_t* loadTextFile( FILE* myFile, int nrWords )
{
    Word_t* temp = (Word_t*)malloc(sizeof(Word_t)*nrWords);
    for (int i = 0; i < nrWords; ++i)
    {
        temp[i].word = (char*)malloc(256);
        temp[i].clue = (char*)malloc(256);
    }

    for(int i = 0; i < nrWords; i++)
    {
        fscanf(myFile, " %c %d %d %s %[^\n]\n", &temp[i].direction, &temp[i].x, &temp[i].y, temp[i].word, temp[i].clue );
    }
    return temp;
}

char** createArray(int rows, int cols)
{
    char **tempBoard = (char **)malloc(rows * sizeof(char *));
    for (int i=0; i<rows; i++)
        tempBoard[i] = (char *)malloc(cols * sizeof(char));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            tempBoard[i][j] = '#';
        }
    return tempBoard;
}

void displayBoard(int rows, int cols, char** myBoard)
{
    printf("Current puzzle:\n\n");
    printf("      ");
    for (int j = 0; j < cols; j++)
    {
        printf("%d  ", j+1);

    }
    printf("    \n");
    printf("      ");
    for (int j = 0; j < cols; j++)
    {
        printf("-- ");
    }
    for (int i = 0; i < rows; i++)
    {
        printf("\n%d   | ",i+1);
        for (int j = 0; j < rows; j++)
        {
            printf("%c  ",myBoard[i][j]);
        }
    }
    printf("\n    ");
    for (int i = 0; i < rows; i++)
    {
        printf("***");
    }
    printf("\n");

}
int isBoardFilled(int rows, int cols, char** myBoard)
{
    int ct = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if(myBoard[i][j] == '_')
            {
                ct++;
            }
        }
    }
    if (ct==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int findIndex(int arr[], int size, int val)
{
    for (int i = 0; i < size ; i++)
    {
        if (arr[i] == val)
        {
            return 1;
        }

    }
    return 0;
}
char** updateBoard(char** myBoard, Word_t *words, int solve)
{
    int dirX = words[solve-1].x;
    int dirY = words[solve-1].y;
    if (words[solve-1].f == 1 && words[solve-1].direction == 'H')
    {
        for (int i = 0; i < strlen(words[solve-1].word); i++)
        {
            myBoard[dirX-1][dirY+i-1] = words[solve-1].word[i];
        }
    }

    if (words[solve-1].f == 1 && words[solve-1].direction == 'V')
    {
        for (int i = 0; i < strlen(words[solve-1].word); i++)
        {
            myBoard[dirX+i-1][dirY-1] = words[solve-1].word[i];
        }
    }

    if (words[solve-1].f != 1 && words[solve-1].direction == 'H')
    {
        for (int i = 0; i < strlen(words[solve-1].word); i++)
        {
            if (myBoard[words[solve-1].x-1][words[solve-1].y+i-1] == '#')
            {
                myBoard[words[solve-1].x-1][words[solve-1].y+i-1] = '_';
            }
        }
    }
    if (words[solve-1].f != 1 && words[solve-1].direction == 'V')
    {
        for (int i = 0; i < strlen(words[solve-1].word); i++)
        {
            if (myBoard[words[solve-1].x+i-1][words[solve-1].y-1] == '#')
            {
                myBoard[words[solve-1].x+i-1][words[solve-1].y-1] = '_';
            }
        }
    }


    return myBoard;
}
void playGame(char** myBoard, int nrwords, Word_t *words, int x, int y, int countToFinish)
{

    for (int i = 1; i <= nrwords; i++)
    {
        updateBoard(myBoard, words, i);
    }
    displayBoard(x, y, myBoard);
    for (int i = 0; i < nrwords; i++)
    {
        words[i].f = 0;
    }
    while (countToFinish != nrwords)
    {
        printf("\n\nAsk for hint: \n");
        printf("#  Direction    row  col");
        printf("\n----------------------------");
        for (int i = 0; i < nrwords; i++)
        {
            if (words[i].f != 1)
            {

                printf("\n%d: ", i+1);
                if (words[i].direction == 'H' && i<9 )
                    printf("Horizontal    ");
                else if (words[i].direction == 'H' && i>=9 )
                    printf("Horizontal   ");
                else if (words[i].direction == 'V' && i<9 )
                    printf("Vertical      ");
                else
                    printf("Vertical     ");

                printf("%d    %d", words[i].x, words[i].y);

            }
        }
        printf("\n");
        int solve;
        while (1)
        {
            printf("\nEnter -1 to exit\n");
            printf("Which word to solve next?: ");
            scanf("%d",&solve);
            if (solve == -1)
            {
                printf("Successful exit. \n");
                exit(0);
            }
            if (words[solve-1].f == 0)
            {
                break;
            }
            else if (words[solve-1].f == 1)
            {
                printf("That word has been solved before");
            }
            else
            {
                printf("Input is wrong. Enter a number between %d and %d.",1,nrwords);
            }

        }

        if (words[solve-1].f == 0)
        {
            printf("Current hint: %s\n", words[solve-1].clue);
            printf("Enter your solution: ");
            char solution [20];
            scanf("%s",solution);

            if (strcmp(strupr(solution), words[solve-1].word)== 0  )
            {
                printf("\nCorrect!\n\n");
                words[solve-1].f = 1;
                countToFinish++;
                updateBoard(myBoard, words, solve);
                displayBoard(x, y, myBoard);
                if (isBoardFilled(x,y,myBoard) == 1)
                {
                    printf("\n\nCongratulations! You beat the puzzle!\n");
                    exit(0);
                }

            }
            else
            {
                printf("WRONG ANSWER\n");
                updateBoard(myBoard, words, solve);
                displayBoard(x, y, myBoard);
            }
        }
    }
}
