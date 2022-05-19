#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char wall;
char player;
char finish;
char air;
int xPos;
int yPos;
int finishXPos;
int finishYPos;


int displayStage(int size, char stage[size][size]);
int setElement(char element, int x, int y, int size, char stage[size][size]);
int movePlayer(int x, int y, int size, char stage[size][size]);
int checkWin();
char getElement(int x, int y, int size, char stage[size][size]);
int generateMaze(int size, char stage[size][size]);
int recursive(int size, int visitedStage[size][size],int x, int y, char stage[size][size]);

int main() {
    wall = '#';
    player = '@';
    finish = 'x';
    air = ' ';
    xPos = 1;
    yPos = 1;

    int size;
    printf("Choisissez une taille : \n");
    scanf("%d",&size);

    char stage[size][size];

    srand(time(NULL));



    finishXPos = size-2;
    finishYPos = size-2;


    generateMaze(size,stage);

    setElement(player,xPos,yPos,size,stage);

    setElement(finish,finishXPos,finishYPos,size,stage);

    displayStage(size,stage);

    int ch1, ch2;

    ch1 = getch();
    ch2 = 0;
    if (ch1 == 0xE0) {
        do {
            ch2 = getch();
            switch(ch2) {
                case 75:
                    movePlayer(xPos,yPos-1,size,stage);
                    break;
                case 77:
                    movePlayer(xPos,yPos+1,size,stage);
                    break;
                case 72:
                    movePlayer(xPos-1,yPos,size,stage);
                    break;
                case 80:
                    movePlayer(xPos+1,yPos,size,stage);
                    break;
                default:
                    break;
            }
        }while (ch2!=97);
    }

    system("pause");
    return 0;
}

int displayStage(int size, char stage[size][size]){
    printf("\n----------------------\n");
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            printf("%c", stage[i][j]);
            if(j==size-1) printf("\n");
        }
    }
    printf("\n----------------------\n");
    return 1;
}

int setElement(char element, int x, int y, int size, char stage[size][size]){
    stage[x][y] = element;
    return 1;
}

char getElement(int x, int y, int size, char stage[size][size]){
    return stage[x][y];
}

int movePlayer(int x, int y, int size, char stage[size][size]){
    if(getElement(x,y,size,stage)=='#') {
        return 0;
    }
    setElement(air,xPos,yPos,size,stage);
    setElement(player,x,y,size,stage);
    xPos=x;
    yPos=y;
    checkWin();
    displayStage(size,stage);
    return 1;
}

int checkWin(){
    if(finishYPos==yPos && finishXPos==xPos){
        printf("\n\n\n !!!!!!!!!!!!!!!!!!!!!! YOU WON !!!!!!!!!!!!!!!!!!!!!! \n\n\n");
        system("pause");
        return 1;
    }
    return 0;
}

int generateMaze(int size, char stage[size][size]) {

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            setElement(wall,i,j,size,stage);
        }
    }


    int visitedStage[size][size];
    for(int i = 0; i<size;i++){
        for(int j = 0; j < size;j++){
            if(j==0 || j == size-1 || i == 0 || i == size-1){
                visitedStage[i][j] = 2;
            } else {
                visitedStage[i][j] = 0;
            }
        }
    }

    int xCurrent = finishXPos;
    int yCurrent = finishYPos;

    printf("x : %d y: %d\n",xCurrent,yCurrent);

    printf("actual cell state : %d\n",visitedStage[xCurrent][yCurrent]);

    recursive(size,visitedStage,xCurrent,yCurrent,stage);
}

int recursive(int size, int visitedStage[size][size], int x, int y, char stage[size][size]){
    if(visitedStage[x][y]!=0) return 0;

    int count = 0;
    stage[x][y+1]==air ? count++ : count;
    stage[x][y-1]==air ? count++ : count;
    stage[x+1][y]==air ? count++ : count;
    stage[x-1][y]==air ? count++ : count;
    if(count>=2) return 0;


    displayStage(size,stage);
    visitedStage[x][y]=1;
    stage[x][y] = air;
    int cells[4] = {0,1,2,3};


    for(int i = 0; i < 10;i++){
        int index = rand()%4;
        int anotherIndex = rand()%4;
        int temp = cells[index];
        cells[index] = cells[anotherIndex];
        cells[anotherIndex] = temp;
    }

    for(int i = 0; i < 4;i++){
        switch (cells[i]) {
            case 0:
                recursive(size,visitedStage,x+1,y,stage);
                break;
            case 1:
                recursive(size,visitedStage,x,y+1,stage);
                break;
            case 2:
                recursive(size,visitedStage,x,y-1,stage);
                break;
            case 3:
                recursive(size,visitedStage,x-1,y,stage);
                break;
            default:
                break;

        }
    }
}

