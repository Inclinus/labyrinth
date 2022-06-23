#include "level.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// SDL Utils related to main window and renderer
void initSDL();
void quitSDL();
void createWindowAndRenderer();
void updateRenderer();

// SDL Utils to draw
void createFilledRectangle(int x, int y, int w, int h);
void changeColor(int red, int green, int blue);

// SDL Error utils
void SDL_ExitWithError(const char *message);
void closeWindowAndRendererAndThrowError(const char *message);

// Maze related functions
void generateMaze();
int recursive(int x, int y);
void displayMaze();

// Initialize stuff functions
void initPlayer();
void initEnd();

// Move functions
void moveDown();
void moveUp();
void moveRight();
void moveLeft();
void updatePlayerPos();

// Check on move functions
void checkWin();
void checkAction();
void explode(int x, int y);

// A little struct for the player
struct Player {
    int x;
    int y;
} Player;

// Maze parameters
int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;
int MAZE_SIZE = 0;
int BOMBS = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Player's related stuff
SDL_Rect playerRectangle;
SDL_Surface *playerImage = NULL;
SDL_Texture *playerTexture = NULL;

// Star's related stuff
SDL_Surface *endSurface = NULL;
SDL_Texture *endTexture = NULL;
SDL_Rect endRectangle;

// Virtual grid of the game
int ** visitedStage = NULL;

// Global player
struct Player player;

// SDL Bool to do the game loop
SDL_bool program_launched;

// Time (used to calculate time of one game)
time_t begin;
time_t end;


int levelMain(int argc, char **argv, int size, int bombs){
    // Here we are ! Let's initialize this
    program_launched = SDL_TRUE;
    struct Player pl = {1,1};
    player = pl;
    window = NULL;
    renderer = NULL;
    playerImage = NULL;
    playerTexture = NULL;
    endSurface = NULL;
    endTexture = NULL;
    visitedStage = NULL;

    // Define maze size
    MAZE_SIZE = size;

    // Simple value to know if bombs are active or not, used in #generateMaze function
    BOMBS = bombs;

    // Define window parameters
    WINDOW_WIDTH = 20*size;
    WINDOW_HEIGHT = WINDOW_WIDTH;


    initSDL();
    createWindowAndRenderer();

    // Allocate memory to the entire maze double depth array
    visitedStage = malloc(MAZE_SIZE * sizeof(int*));
    for (int i = 0; i < MAZE_SIZE; i++)
        visitedStage[i] = malloc(MAZE_SIZE * sizeof(int));

    // Generate randomly the maze, init player and star
    generateMaze(visitedStage);
    initPlayer();
    initEnd();
    updateRenderer();

    // Time to begin (MOUAHAHAH)
    begin = time(NULL);

    /*----------------------------------------------*/
    while(program_launched){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_DOWN:
                            moveDown();
                            break;
                        case SDLK_RIGHT:
                            moveRight();
                            break;
                        case SDLK_UP:
                            moveUp();
                            break;
                        case SDLK_LEFT:
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
    return 0;
}

void updatePlayerPos(){
    SDL_RenderClear(renderer);
    // If we don't display the maze, only the player and the star will be in the renderer
    displayMaze();
    SDL_RenderCopy(renderer,playerTexture,NULL,&playerRectangle);
    // Check if the player is on the star or not
    checkWin();
    SDL_RenderCopy(renderer,endTexture,NULL,&endRectangle);
    updateRenderer();
}

void checkWin(){
    if(playerRectangle.x == endRectangle.x && playerRectangle.y == endRectangle.y) {
        end = time(NULL);
        int partyTime = end-begin;
        int h = partyTime/3600;
        int min = (partyTime - (3600*h)) /60;
        int seconds = partyTime - (3600*h) - (min*60);
        printf("\n\n\nBRAVO !! Vous avez attrapé l'étoile et remporté la partie !\n Quelle performance !\n\nTemps de la partie : %d hour(s)) %d min(s) %d seconds\n\n",h,min,seconds);
        program_launched = SDL_FALSE;
    }
}


void moveDown(){
    // If we are trying to go over a wall or a border
    if(visitedStage[player.x][player.y+1]>=1){
        return;
    }
    playerRectangle.y += 20;
    visitedStage[player.x][player.y] = 0;
    player.y += 1;
    // Check if an actions to be done on the player position
    checkAction();
    // Set the player on the grid
    visitedStage[player.x][player.y] = -1;
    // Update player position by refreshing the screen
    updatePlayerPos();
}

void moveRight(){
    // If we are trying to go over a wall or a border
    if(visitedStage[player.x+1][player.y]>=1){
        return;
    }
    playerRectangle.x += 20;
    visitedStage[player.x][player.y] = 0;
    player.x += 1;
    // Check if an actions to be done on the player position
    checkAction();
    // Set the player on the grid
    visitedStage[player.x][player.y] = -1;
    // Update player position by refreshing the screen
    updatePlayerPos();
}

void moveUp(){
    // If we are trying to go over a wall or a border
    if(visitedStage[player.x][player.y-1]>=1){
        return;
    }
    playerRectangle.y -= 20;
    visitedStage[player.x][player.y] = 0;
    player.y -= 1;
    // Check if an actions to be done on the player position
    checkAction();
    // Set the player on the grid
    visitedStage[player.x][player.y] = -1;
    // Update player position by refreshing the screen
    updatePlayerPos();
}

void moveLeft(){
    // If we are trying to go over a wall or a border
    if(visitedStage[player.x-1][player.y]>=1){
        return;
    }
    playerRectangle.x -= 20;
    visitedStage[player.x][player.y] = 0;
    player.x -= 1;
    // Check if an actions to be done on the player position
    checkAction();
    // Set the player on the grid
    visitedStage[player.x][player.y] = -1;
    // Update player position by refreshing the screen
    updatePlayerPos();
}

void checkAction(){
    // If the player is on a bomb (bomb is defined to be a "-5" in the virtual grid)
    if(visitedStage[player.x][player.y] == -5)
        // Let's destroy everything around
        explode(player.x,player.y);
}

/**
 * Destroy everything around @param x and @param y position
 */
void explode(int x, int y){
    int arrayX[3] = {x,x-1,x+1};
    int arrayY[3] = {y, y+1, y-1};
    for(int i = 0; i < 3;i++){
        for(int j = 0; j < 3;j++){
            if(i==0 && j==0) continue;
            if(visitedStage[arrayX[i]][arrayY[j]]!=2) visitedStage[arrayX[i]][arrayY[j]] = 0;
        }
    }
}

/**
 * Display the maze, 1 are walls, 2 are border, -1 is the player, -5 are bombs, 0 is the way
 */
void displayMaze(){
    for(int i = 0; i<MAZE_SIZE;i++){
        for(int j = 0; j < MAZE_SIZE;j++){
            if(visitedStage[i][j] >= 1){
                changeColor(20,20,20);
                createFilledRectangle(i*20,j*20,20,20);
            } else if(visitedStage[i][j] == 0 || visitedStage[i][j] == -1) {
                changeColor(40,255,40);
                createFilledRectangle(i*20,j*20,20,20);
            } else if(visitedStage[i][j] == -5){
                changeColor(255,40,120);
                createFilledRectangle(i*20,j*20,20,20);
            }
        }
    }
}

void generateMaze() {
    // Setup random
    srand(time(NULL));

    // Fill all the maze with walls, precise if it's border
    for(int i = 0; i<MAZE_SIZE;i++){
        for(int j = 0; j < MAZE_SIZE;j++){
            if(j==0 || j == MAZE_SIZE-1 || i == 0 || i == MAZE_SIZE-1){
                visitedStage[i][j] = 2; // borders
            } else {
                visitedStage[i][j] = 1; // walls
            }
        }
    }

    // Start point for the generation algorithm
    int xCurrent = MAZE_SIZE-2;
    int yCurrent = MAZE_SIZE-2;

    // Select color for the maze way (green)
    changeColor(40,255,40);

    // Algorithm for the maze way
    recursive(xCurrent,yCurrent);

    if(BOMBS){
        // Place some bombs into the maze
        int count = MAZE_SIZE*MAZE_SIZE/20;
        for(int i = 0; i<MAZE_SIZE;i++){
            for(int j = 0; j < MAZE_SIZE;j++){
                if(visitedStage[i][j] == 0 && (i!=1 && j!=1)){
                    if(rand()%30==3 && count >=1) {
                        visitedStage[i][j] = -5; // -5 are bombs
                        count--;
                    }
                }
            }
        }
    }


    // Verification (we don't know what can happen)
    if(visitedStage[player.x][player.y]==1) visitedStage[player.x][player.y] = 0;

    displayMaze();
}

int recursive(int x, int y){
    // If we are not on a wall, give up
    if(visitedStage[x][y]!=1) return 0;

    // Count how many rectangle are "way"'s rectangle around x y pos
    int count = 0;
    visitedStage[x][y+1]==0 ? count++ : count;
    visitedStage[x][y-1]==0 ? count++ : count;
    visitedStage[x+1][y]==0 ? count++ : count;
    visitedStage[x-1][y]==0 ? count++ : count;
    if(count>=2) return 0; // if we have 2 or more, let's give up

    // Set the way on actual x / y
    visitedStage[x][y]=0;

    // create way, green rectangle
    createFilledRectangle(x*20,y*20,20,20);
    int cells[4] = {0,1,2,3};

    // More randomness
    for(int i = 0; i < 10;i++){
        int index = rand()%4;
        int anotherIndex = rand()%4;
        int temp = cells[index];
        cells[index] = cells[anotherIndex];
        cells[anotherIndex] = temp;
    }

    // 4 recursive for one, in random order
    for(int i = 0; i < 4;i++){
        switch (cells[i]) {
            case 0:
                recursive(x+1,y);
                break;
            case 1:
                recursive(x,y+1);
                break;
            case 2:
                recursive(x,y-1);
                break;
            case 3:
                recursive(x-1,y);
                break;
            default:
                break;

        }
    }
}

void initEnd(){
    endSurface = IMG_Load("images/star.png");
    if(endSurface == NULL){
        closeWindowAndRendererAndThrowError("Unable to load playerImage .ico");
    }

    endTexture = SDL_CreateTextureFromSurface(renderer, endSurface);
    SDL_FreeSurface(endSurface);

    if(endTexture == NULL){
        closeWindowAndRendererAndThrowError("Unable to create playerTexture");
    }


    // We have a star of 16x16 pixels
    endRectangle.w = 16;
    endRectangle.h = 16;
    if(SDL_QueryTexture(endTexture, NULL, NULL, &endRectangle.w, &endRectangle.h) != 0){
        closeWindowAndRendererAndThrowError("Unable to load playerTexture");
    }

    // Place end star on the correct place
    endRectangle.x = MAZE_SIZE * 20 - 37;
    endRectangle.y = MAZE_SIZE * 20 - 37;

    if(SDL_RenderCopy(renderer, endTexture, NULL, &endRectangle) != 0){
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


    // We have a player of 16x16 pixels
    playerRectangle.w = 16;
    playerRectangle.h = 16;
    if(SDL_QueryTexture(playerTexture, NULL, NULL, &playerRectangle.w, &playerRectangle.h) != 0){
        closeWindowAndRendererAndThrowError("Unable to load playerTexture");
    }

    // define first position of the player
    playerRectangle.x = 23; // 23
    playerRectangle.y = 23; // 23

    if(SDL_RenderCopy(renderer, playerTexture, NULL, &playerRectangle) != 0){
        closeWindowAndRendererAndThrowError("Unable to display playerTexture");
    }
}

void changeColor(int red, int green, int blue){
    if(SDL_SetRenderDrawColor(renderer,red,green,blue,SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Unable to change renderer color");
}

void createFilledRectangle(int x, int y, int w, int h){
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    if(SDL_RenderFillRect(renderer, &rectangle) != 0){
        SDL_ExitWithError("Unable to draw playerRectangle");
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

    for (int i = 0; i < MAZE_SIZE; i++)
        free(visitedStage[i]);
    free(visitedStage);


    SDL_Quit();
}

void initSDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_ExitWithError("SDL init failed");
    }
}

void SDL_ExitWithError(const char *message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
