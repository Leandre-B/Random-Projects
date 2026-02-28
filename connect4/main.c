#include <stdio.h>
#include  <stdlib.h>

typedef char Board[7][6];

typedef struct Vect2{
    int x;
    int y;
}Vect2;

typedef struct Point{
    int x;
    int y;
}Point;

enum GameState {
    PLAYING, DRAW, WIN
};

typedef struct Game{
    Board board;
    enum GameState gameState;
    char current_player;
    Point lastPlay;
}Game;


char switchPlayer(char p){
    if(p=='x')
        return 'o';
    else
        return 'x';
}

void initBoard(Board board){
    for(unsigned int x=0; x<7; ++x)
        for(unsigned int y=0; y<6; ++y)
            board[x][y]=' ';
}

void printBoard(const Board board) {
    printf("-----------------------------\n");
    for(unsigned int y=0; y<6; ++y) {
        for(unsigned int x=0; x<7; ++x) {
            if(board[x][y]=='x')
                printf("| " "\e[0;31m" "%c " "\e[0m", board[x][y]);
            else
                printf("| " "\e[0;34m" "%c " "\e[0m", board[x][y]);
        }
        printf("|");
        printf("\n-----------------------------\n");
    }
}

unsigned int totalAlign(Game *game, Vect2 direction){
    unsigned int tot=0;
    Point auxPlay=game->lastPlay;

    while(  auxPlay.x>=0 && auxPlay.x<7 &&
            auxPlay.y>=0 && auxPlay.y<=6 &&
            game->board[auxPlay.x][auxPlay.y]==game->current_player)
    {
        ++tot;
        auxPlay.x+=direction.x;
        auxPlay.y+=direction.y;
    }

    direction.x*=-1;
    direction.y*=-1;

    auxPlay=game->lastPlay;
    while(  auxPlay.x>=0 && auxPlay.x<7 &&
        auxPlay.y>=0 && auxPlay.y<=6 &&
        game->board[auxPlay.x][auxPlay.y]==game->current_player)
    {
        ++tot;
        auxPlay.x+=direction.x;
        auxPlay.y+=direction.y;
    }
    return tot-1; //orignal point is counted 2 times
}

bool is4Align(Game *game){
    Vect2 direction;

    //DIAGONAL
    direction.x=1; direction.y=1;
    if (totalAlign(game, direction)>=4){
        return true;
    }

    direction.x=1; direction.y=-1;
    if (totalAlign(game, direction)>=4){
        return true;
    }

    //HORINZONTAL

    direction.x=1; direction.y=0;
    if (totalAlign(game, direction)>=4){
        return true;
    }

    //VERTICAL
    direction.x=0; direction.y=1;
    if (totalAlign(game, direction)>=4){
        return true;
    }

    return false;
}


void makePlay(Game *game){
    game->board[game->lastPlay.x][game->lastPlay.y]=game->current_player;

    if(is4Align(game)){
        printf("Played %c win!\n", game->current_player);
        game->gameState=WIN;
    }
}


int getY(Board board, unsigned int x){
    for(int y=5; y>=0; --y){
        if(board[x][y]==' ')
            return y;
    }
    return -1;
}
bool isValidPlay(Board board, unsigned int x) {
    if(!(x>=0 && x<7)) {
        printf("Choosen column must be between 1 and 7. Retry.\n");
        return false;
    }
    if(getY(board, x)==-1){
        printf("Choosen column is full. Retry.\n");
        return false;
    }
    //else
    return true;
}



int max(int a, int b) {
    return (a>b) ? a : b;
}

int min(int a, int b) {
    return (a<b) ? a : b;
}


Game copyGame(Game game){
    Game copy;
    for(unsigned int x=0; x<7; ++x)
        for(unsigned int y=0; y<6; ++y)
            copy.board[x][y]=game.board[x][y];
    copy.lastPlay=game.lastPlay;
    copy.gameState=game.gameState;
    copy.current_player=game.current_player;

    return copy;
}


int valuePosition(Game *game){
    char player='o'; //IA
    return rand()%200 -100;
}

unsigned int minimax(Game *game, unsigned int depth, bool maximizingPlayer){

    if(depth==0 || game->gameState==DRAW){
        return valuePosition(game);
    }
    if(maximizingPlayer){
        int value=-101;
        for(unsigned int x=0; x<7; ++x){
            if(isValidPlay(game->board, x)){
                Game child=copyGame(*game);

                child.lastPlay.x=x;
                child.lastPlay.y=getY(game->board, x);
                child.board[child.lastPlay.x][game->lastPlay.y]='o';

                value=max(value, minimax(&child, depth-1, !maximizingPlayer));
            }
        }
        return value;

    }else{
        int value=101;
        for(unsigned int x=0; x<7; ++x){
            if(isValidPlay(game->board, x)){
                Game child=copyGame(*game);

                child.lastPlay.x=x;
                child.lastPlay.y=getY(game->board, x);
                child.board[game->lastPlay.x][game->lastPlay.y]='x';

                value=min(value, minimax(&child, depth-1, !maximizingPlayer));
            }
        }
        return value;
    }
}

unsigned int bestMove(Game *game){
    int bestMove = -1;
    int bestValue = -1000;

    for (int x=0; x<7; x++) {
        if(isValidPlay(game->board, x)){
            Game child=copyGame(*game);

            child.lastPlay.x=x;
            child.lastPlay.y=getY(game->board, x);
            child.board[child.lastPlay.x][child.lastPlay.y]='o';

            int value = minimax(&child, 5 - 1, false);

            if (value > bestValue) {
                bestValue = value;
                bestMove = x;
            }

        }
    }

    return bestMove;
}

int main() {
    Game game;

    initBoard(game.board);
    game.gameState=PLAYING;
    game.current_player='x';

    printf("Connect 4 !\n");

    while(game.gameState==PLAYING) {
        unsigned int choosen_x;
        if(game.current_player=='x'){
            do {
                printf("Choose where you want to play (1-7) : ");
                while (scanf("%u", &choosen_x)!=1){
                    printf("Error : not an number !\n");

                    //empty buffer
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);

                    printf("Choose where you want to play (1-7) : ");
                }
                printf("%u\n", choosen_x);
                --choosen_x;
            }while(!isValidPlay(game.board, choosen_x));
        }else
            choosen_x=bestMove(&game);

        game.lastPlay.x=choosen_x;
        game.lastPlay.y=getY(game.board, choosen_x);
        makePlay(&game);


        printBoard(game.board);



        //switch player (1-0);
        printf("\n");
        game.current_player = switchPlayer(game.current_player);
    }


    return 0;
}
