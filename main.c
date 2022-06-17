#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

void version();
void initSDL();
void quitSDL();
void SDL_ExitWithError(const char *message);
void createWindowAndRenderer();
void changeColor(int red, int green, int blue);
SDL_Rect * createRectangle(int x, int y, int w, int h);
void createLine(int x1, int y1, int x2, int y2);
void createPoint(int x, int y);
void setBackground();
void closeWindowAndRendererAndThrowError(const char *message);
void updateRenderer();
SDL_Rect * createFilledRectangle(int x, int y, int w, int h);

void generateMaze(int size, int ** visitedStage);

int recursive(int size, int ** visitedStage, int x, int y);

void initPlayer();
void initEnd();

void moveDown();
void moveUp();
void moveRight();
void moveLeft();

void displayMaze();

void updatePlayerPos();

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAZE_SIZE 30

struct Player {
    int x;
    int y;
};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen = NULL;

SDL_Rect playerRectangle;
SDL_Surface *playerImage = NULL;
SDL_Texture *playerTexture = NULL;
int** visitedStage = NULL;
struct Player player = {1,1};

int main(int argc, char **argv){
    initSDL();
    createWindowAndRenderer();
    screen = SDL_GetWindowSurface(window);


    SDL_bool program_launched = SDL_TRUE;



    visitedStage = malloc(MAZE_SIZE * sizeof(int*));
    for (int i = 0; i < MAZE_SIZE; i++)
        visitedStage[i] = malloc(MAZE_SIZE * sizeof(int));

    generateMaze(MAZE_SIZE, visitedStage);
    printf("AVANT : %d / %d\n", playerRectangle.x, playerRectangle.y);
    initPlayer();
    printf("APRES : %d / %d\n", playerRectangle.x, playerRectangle.y);
    initEnd();
    updateRenderer();

    while(program_launched){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                // mouvement de souris
//                case SDL_MOUSEMOTION:
//                    printf("%d / %d\n",event.motion.x, event.motion.y);
//                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // event.button.button pour check le clic de souris :
                    // SDL_BUTTON_LEFT | SDL_BUTTON_MIDDLE | SDL_BUTTON_RIGHT
                    //printf("Clic en : %d / %d\n", event.button.x, event.button.y);
                    createLine(event.button.x,event.button.y,event.button.x+50,event.button.y+50);
                    updateRenderer();
                    break;
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_DOWN:
                            printf("DOWN\n");
                            moveDown();
                            break;
                        case SDLK_RIGHT:
                            printf("RIGHT\n");
                            moveRight();
                            break;
                        case SDLK_UP:
                            printf("UP\n");
                            moveUp();
                            break;
                        case SDLK_LEFT:
                            printf("LEFT\n");
                            moveLeft();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    /*----------------------------------------------*/

    // Close all and quit
    quitSDL();
    return EXIT_SUCCESS;
}

void updatePlayerPos(){
    SDL_RenderClear(renderer);
    displayMaze();
    SDL_RenderCopy(renderer,playerTexture,NULL,&playerRectangle);
    updateRenderer();
    printf("POS : %d / %d\nVISITED_POS : %d / %d\n",playerRectangle.x,playerRectangle.y,player.x,player.y);
}

void moveDown(){
    if(visitedStage[player.x][player.y+1]>=1){
        return;
    }
    playerRectangle.y += 20;
    visitedStage[player.x][player.y] = 0;
    player.y += 1;
    visitedStage[player.x][player.y] = -1;

    // MOVE PLAYER ????
    // idk
    updatePlayerPos();
}

void moveRight(){
    if(visitedStage[player.x+1][player.y]>=1){
        return;
    }
    playerRectangle.x += 20;
    visitedStage[player.x][player.y] = 0;
    player.x += 1;
    visitedStage[player.x][player.y] = -1;

    // MOVE PLAYER ????
    // idk
    updatePlayerPos();
}

void moveUp(){
    if(visitedStage[player.x][player.y-1]>=1){
        return;
    }
    playerRectangle.y -= 20;
    visitedStage[player.x][player.y] = 0;
    player.y -= 1;
    visitedStage[player.x][player.y] = -1;

    // MOVE PLAYER ????
    // idk
    updatePlayerPos();
}

void moveLeft(){
    if(visitedStage[player.x-1][player.y]>=1){
        return;
    }
    playerRectangle.x -= 20;
    visitedStage[player.x][player.y] = 0;
    player.x -= 1;
    visitedStage[player.x][player.y] = -1;

    // MOVE PLAYER ????
    // idk
    updatePlayerPos();
}

void displayMaze(){
    for(int i = 0; i<MAZE_SIZE;i++){
        for(int j = 0; j < MAZE_SIZE;j++){
            if(visitedStage[i][j] >= 1){
                changeColor(20,20,20);
                createFilledRectangle(i*20,j*20,20,20);
            } else {
                changeColor(40,255,40);
                createFilledRectangle(i*20,j*20,20,20);
            }
        }
    }
}

// 0 is air, 1 are walls, 2 are borders
void generateMaze(int size, int ** visitedStage) {
    printf("sizeMaze1 : %d\n",size);
    for(int i = 0; i<size;i++){
        for(int j = 0; j < size;j++){
            if(j==0 || j == size-1 || i == 0 || i == size-1){
                visitedStage[i][j] = 2; // borders
            } else {
                visitedStage[i][j] = 1; // walls
            }
        }
    }
    printf("sizeMaze2 : %d\n",size);

    int xCurrent = size-2;
    int yCurrent = size-2;


    printf("x : %d y: %d\n",xCurrent,yCurrent);

    printf("actual cell state : %d\n",visitedStage[xCurrent][yCurrent]);

    changeColor(40,255,40);
    recursive(size,visitedStage,xCurrent,yCurrent);

    displayMaze();
}

int recursive(int size, int ** visitedStage, int x, int y){
    printf("entering in recursive : size = %d\n",size);
    if(visitedStage[x][y]!=1) return 0;
    printf("size : %d\n",size);
    int count = 0;
    visitedStage[x][y+1]==0 ? count++ : count;
    visitedStage[x][y-1]==0 ? count++ : count;
    visitedStage[x+1][y]==0 ? count++ : count;
    visitedStage[x-1][y]==0 ? count++ : count;
    if(count>=2) return 0;
    printf("count is < 2\n");


    //updateRenderer();
    visitedStage[x][y]=0;

    createFilledRectangle(x*20,y*20,20,20);
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
                recursive(size,visitedStage,x+1,y);
                break;
            case 1:
                recursive(size,visitedStage,x,y+1);
                break;
            case 2:
                recursive(size,visitedStage,x,y-1);
                break;
            case 3:
                recursive(size,visitedStage,x-1,y);
                break;
            default:
                break;

        }
    }
}

void initEnd(){
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;

    image = IMG_Load("images/star.png");
    if(image == NULL){
        closeWindowAndRendererAndThrowError("Unable to load playerImage .ico");
    }

    texture = SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);

    if(texture == NULL){
        closeWindowAndRendererAndThrowError("Unable to create playerTexture");
    }

    SDL_Rect rectangle;
    rectangle.w = 16;
    rectangle.h = 16;
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0){
        closeWindowAndRendererAndThrowError("Unable to load playerTexture");
    }

    rectangle.x = MAZE_SIZE*20-37;
    rectangle.y = MAZE_SIZE*20-37;

    if(SDL_RenderCopy(renderer,texture,NULL,&rectangle) != 0){
        closeWindowAndRendererAndThrowError("Unable to display playerTexture");
    }
}

void initPlayer(){

    playerImage = IMG_Load("images/player.gif");
    if(playerImage == NULL){
        closeWindowAndRendererAndThrowError("Unable to load playerImage .gif");
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, playerImage);
    SDL_FreeSurface(playerImage);

    if(playerTexture == NULL){
        closeWindowAndRendererAndThrowError("Unable to create playerTexture");
    }


    playerRectangle.w = 16;
    playerRectangle.h = 16;
    if(SDL_QueryTexture(playerTexture, NULL, NULL, &playerRectangle.w, &playerRectangle.h) != 0){
        closeWindowAndRendererAndThrowError("Unable to load playerTexture");
    }

    playerRectangle.x = 23;
    playerRectangle.y = 23;

    if(SDL_RenderCopy(renderer, playerTexture, NULL, &playerRectangle) != 0){
        closeWindowAndRendererAndThrowError("Unable to display playerTexture");
    }
}

void changeColor(int red, int green, int blue){
    if(SDL_SetRenderDrawColor(renderer,red,green,blue,SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Unable to change renderer color");
}

//void createImageBMP()

SDL_Rect * createRectangle(int x, int y, int w, int h){
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    if(SDL_RenderDrawRect(renderer, &rectangle) != 0){
        SDL_ExitWithError("Unable to draw playerRectangle");
    }

    return &rectangle;
}

SDL_Rect * createFilledRectangle(int x, int y, int w, int h){
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    if(SDL_RenderFillRect(renderer, &rectangle) != 0){
        SDL_ExitWithError("Unable to draw playerRectangle");
    }

    return &rectangle;
}

void createPoint(int x, int y){
    if(SDL_RenderDrawPoint(renderer, x, y) != 0){
        SDL_ExitWithError("Unable to draw point");
    }
}

void createLine(int x1, int y1, int x2, int y2){
    if(SDL_RenderDrawLine(renderer, x1, y1, x2, y2) != 0){
        SDL_ExitWithError("Unable to draw line");
    }
}

void setBackground(){
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;

    image = IMG_Load("images/background.bmp");
    if(image == NULL){
        closeWindowAndRendererAndThrowError("Unable to load playerImage .bmp");
    }

    texture = SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);

    if(texture == NULL){
        closeWindowAndRendererAndThrowError("Unable to create playerTexture");
    }

    SDL_Rect rectangle;
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0){
        closeWindowAndRendererAndThrowError("Unable to load playerTexture");
    }

    rectangle.x = (WINDOW_WIDTH - rectangle.w) / 2;
    rectangle.y = (WINDOW_HEIGHT - rectangle.h) / 2;

    if(SDL_RenderCopy(renderer,texture,NULL,&rectangle) != 0){
        closeWindowAndRendererAndThrowError("Unable to display playerTexture");
    }
}

void createWindowAndRenderer(){
    // Window + renderer
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH,WINDOW_HEIGHT,0,&window,&renderer) != 0){
        SDL_ExitWithError("Unable to create window and renderer");
    }
    SDL_SetWindowTitle(window,"Labyrinthe");
}

void closeWindowAndRendererAndThrowError(const char *message){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_ExitWithError(message);
}

void updateRenderer(){
    SDL_RenderPresent(renderer);
}











void quitSDL(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void initSDL(){
    version();
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_ExitWithError("SDL init failed");
    }
}

void SDL_ExitWithError(const char *message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void version(){
    SDL_version nb;
    SDL_VERSION(&nb);

    printf("Bienvenue sur la SDL %d.%d.%d !\n", nb.major, nb.minor, nb.patch);
}
