#include "game.h"

void initBoard(Board board){
    for(unsigned int x=0; x<7; ++x)
        for(unsigned int y=0; y<6; ++y)
            board[x][y]=' ';
}

void askPlayersType(Player *p1, Player *p2){
    for(unsigned int i=1; i<=2; ++i){
        unsigned int choosenType=0;
        do {
            printf("\nChoose the type of the player %u.\n1 : HUMAN\n2 : IA\n", i);
            while (scanf("%u", &choosenType)!=1){
                printf("Error : not an number !\n");

                //empty buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
        }while(choosenType!=1 && choosenType!=2);
        if(i==1){
            if(choosenType==1){
                p1->type=HUMAN;
            }else
                p1->type=IA;
        }
        if(i==2){
            if(choosenType==1){
                p2->type=HUMAN;
            }else
                p2->type=IA;
        }
    }
}

void initGame(Game *game, bool isOnline){
    game->player1.symbole='x';
    game->player2.symbole='o';

    if(!isOnline)
        askPlayersType(&game->player1, &game->player2);
    else
    {
        game->player1.type=HUMAN;
        game->player2.type=HUMAN;
    }
    game->currentPlayer=&game->player1;

    initBoard(game->board);
    game->gameState=PLAYING;
    game->nbTurn=0;

}


void printBoard(const Board board) {
    printf("------------------------------\n");
    for(unsigned int y=0; y<6; ++y) {
        for(unsigned int x=0; x<7; ++x) {
            if(board[x][y]=='x')
                printf("| " "\e[0;31m" "%c " "\e[0m", board[x][y]);
            else
                printf("| " "\e[0;34m" "%c " "\e[0m", board[x][y]);
        }
        printf("|");
        if(y!=5)
            printf("\n------------------------------\n");
    }
    printf("\n--1---2---3---4---5---6---7---\n");
}


char switchPlayer(Game* game){
    if(game->currentPlayer == &game->player1)
        game->currentPlayer = &game->player2;
    else
        game->currentPlayer = &game->player1;
}

int getY(Board board, unsigned int x){
    for(int y=5; y>=0; --y){
        if(board[x][y]==' ')
            return y;
    }
    return -1;
}

unsigned int totalAlign(Game *game, Vect2 direction){
    unsigned int tot=0;
    Point auxPlay=game->lastPlay;

    while(  auxPlay.x>=0 && auxPlay.x<7 &&
        auxPlay.y>=0 && auxPlay.y<6 &&
        game->board[auxPlay.x][auxPlay.y]==game->currentPlayer->symbole)
    {
        ++tot;
        auxPlay.x+=direction.x;
        auxPlay.y+=direction.y;
    }

    direction.x*=-1;
    direction.y*=-1;

    auxPlay=game->lastPlay;
    while(  auxPlay.x>=0 && auxPlay.x<7 &&
        auxPlay.y>=0 && auxPlay.y<6 &&
        game->board[auxPlay.x][auxPlay.y]==game->currentPlayer->symbole)
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

unsigned int maxAlign(Game *game){
    Vect2 direction;
    unsigned int maxAl=0;
    //DIAGONAL
    direction.x=1; direction.y=1;
    maxAl=max(maxAl, totalAlign(game, direction));

    direction.x=1; direction.y=-1;
    maxAl=max(maxAl, totalAlign(game, direction));

    //HORINZONTAL

    direction.x=1; direction.y=0;
    maxAl=max(maxAl, totalAlign(game, direction));

    //VERTICAL
    direction.x=0; direction.y=1;
    maxAl=max(maxAl, totalAlign(game, direction));

    return maxAl;
}

//for player and IA : called only if isValidPlay()
void makePlay(Game *game, unsigned int x){
    game->lastPlay.x=x;
    game->lastPlay.y=getY(game->board, x);

    game->board[game->lastPlay.x][game->lastPlay.y]=game->currentPlayer->symbole;
    ++game->nbTurn;

    game->coups[game->nbTurn-1]=game->lastPlay.x;

    if(game->nbTurn==42){
        game->gameState=DRAW;
    }
    else if(is4Align(game)){
        game->gameState=WIN;
    }

    switchPlayer(game);
}


bool isValidPlay(Board board, unsigned int x) {
    if(!(x>=0 && x<7)) {
        printf("Choosen column must be between 1 and 7. Retry.\n");
        return false;
    }
    if(board[x][0]!=' '){
        //printf("Choosen column is full. Retry.\n");
        return false;
    }
    //else
    return true;
}