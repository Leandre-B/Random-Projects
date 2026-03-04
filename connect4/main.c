#include <stdio.h>
#include  <stdlib.h>
#include <stdbool.h>

typedef char Board[7][6];

typedef struct Vect2{
    int x;
    int y;
}Vect2;

typedef struct Point{
    int x;
    int y;
}Point;

int max(int a, int b) {
    return (a>b) ? a : b;
}

int min(int a, int b) {
    return (a<b) ? a : b;
}



enum GameState {
    PLAYING, DRAW, WIN
};

typedef struct Game{
    Board board;
    enum GameState gameState;
    char current_player;
    Point lastPlay;
    unsigned int nb_turn;
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

unsigned int totalAlign(Game *game, Vect2 direction){
    unsigned int tot=0;
    Point auxPlay=game->lastPlay;

    while(  auxPlay.x>=0 && auxPlay.x<7 &&
            auxPlay.y>=0 && auxPlay.y<6 &&
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
        auxPlay.y>=0 && auxPlay.y<6 &&
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


void makePlay(Game *game){
    game->board[game->lastPlay.x][game->lastPlay.y]=game->current_player;
    ++game->nb_turn;

    if(game->nb_turn==42)
        game->gameState=DRAW;
    else if(is4Align(game)){
        //printf("Played %c win!\n", game->current_player);
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
    if(board[x][0]!=' '){
        //printf("Choosen column is full. Retry.\n");
        return false;
    }
    //else
    return true;
}



Game copyGame(Game *game){
    Game copy;
    for(unsigned int x=0; x<7; ++x)
        for(unsigned int y=0; y<6; ++y)
            copy.board[x][y]=game->board[x][y];
    copy.nb_turn=game->nb_turn;
    copy.lastPlay=game->lastPlay;
    copy.gameState=game->gameState;
    copy.current_player=game->current_player;

    return copy;
}

unsigned int nbPossibleAlingFromPos(Game *game, Vect2 direction){
    unsigned int tot=0;
    Point auxPlay=game->lastPlay;
    while(  auxPlay.x>=0 && auxPlay.x<7     &&
            auxPlay.y>=0 && auxPlay.y<6      &&
            (game->board[auxPlay.x][auxPlay.y]==(game->board[game->lastPlay.x][game->lastPlay.y]) ||
        game->board[auxPlay.x][auxPlay.y]==' '))
    {
        ++tot;
        auxPlay.x+=direction.x;
        auxPlay.y+=direction.y;
    }

    direction.x*=-1;
    direction.y*=-1;

    auxPlay=game->lastPlay;
    while(  auxPlay.x>=0 && auxPlay.x<7      &&
        auxPlay.y>=0 && auxPlay.y<6          &&
        (game->board[auxPlay.x][auxPlay.y]== (game->board[game->lastPlay.x][game->lastPlay.y]) ||
        game->board[auxPlay.x][auxPlay.y]==' '))
    {
        ++tot;
        auxPlay.x+=direction.x;
        auxPlay.y+=direction.y;
    }
    return tot-1; //orignal point is counted 2 times
}

unsigned int nbOfLinePossibleFromPosition(Game *game){
    unsigned int tot=0;
    Vect2 direction;

    //DIAGONAL
    direction.x=1; direction.y=1;
    if (nbPossibleAlingFromPos(game, direction)>=4){
        ++tot;
    }

    direction.x=1; direction.y=-1;
    if (nbPossibleAlingFromPos(game, direction)>=4){
        ++tot;
    }

    //HORINZONTAL

    direction.x=1; direction.y=0;
    if (nbPossibleAlingFromPos(game, direction)>=4){
        ++tot;
    }

    //VERTICAL
    direction.x=0; direction.y=1;
    if (nbPossibleAlingFromPos(game, direction)>=4){
        ++tot;
    }

    return tot;
}

int valuePosition(Game *game, char p, unsigned int depth){
    if(game->gameState==WIN)
        return (game->current_player==p ? 400000+depth : -400000+depth);

    unsigned int nb_align_o=0;
    unsigned int nb_align_x=0;
    unsigned int save_x = game->lastPlay.x;
    unsigned int save_y = game->lastPlay.y;
    for(unsigned int x=0; x<7; ++x){
        for(unsigned int y=0; y<6; ++y){
            if(game->board[x][y]=='o'){
                game->lastPlay.x=x;
                game->lastPlay.y=y;
                nb_align_o+=nbOfLinePossibleFromPosition(game);
            }else if(game->board[x][y]=='x'){
                game->lastPlay.x=x;
                game->lastPlay.y=y;
                nb_align_x+=nbOfLinePossibleFromPosition(game);
            }

        }
    }
    game->lastPlay.x=save_x;
    game->lastPlay.y=save_y;
    // printf("o : %u\n", nb_align_o);
    // printf("x : %u\n", nb_align_x);


    //unsigned int weigth[]={1, 3, 5, 7, 5, 3, 1};
    int heur;
    if(p=='o')
        heur=nb_align_o - nb_align_x;
    else
        heur=nb_align_x - nb_align_o;
    //heur=(heur)*50 + weigth[game->lastPlay.x]*10;
    return (heur);

}

int minimaxAB(Game *game, unsigned int depth, bool maximizingPlayer, char p, int alpha, int beta){

    if(depth==0 || game->gameState==DRAW || game->gameState==WIN){
        return valuePosition(game, p, depth);
    }
    if(maximizingPlayer){
        int value=-500000;
        for(unsigned int x=0; x<7; ++x){
            if(isValidPlay(game->board, x)){
                //Game child=copyGame(game);

                Point savePlay = game->lastPlay;
                enum GameState saveState = game->gameState;
                unsigned int saveNb_turn=game->nb_turn;

                game->lastPlay.x=x;
                game->lastPlay.y=getY(game->board, x);

                game->current_player=switchPlayer(game->current_player);
                makePlay(game);

                value=max(value, minimaxAB(game, depth-1, !maximizingPlayer, p, alpha, beta));

                //restore
                game->board[game->lastPlay.x][game->lastPlay.y]=' ';
                game->lastPlay=savePlay;
                game->gameState=saveState;
                game->current_player=switchPlayer(game->current_player);
                game->nb_turn=saveNb_turn;

                if( value>=beta)
                    return value;
                alpha=max(alpha, value);
            }
        }
        return value;

    }else{
        int value=500000;
        for(unsigned int x=0; x<7; ++x){
            if(isValidPlay(game->board, x)){
                //Game child=copyGame(game);

                Point savePlay = game->lastPlay;
                enum GameState saveState = game->gameState;
                unsigned int saveNb_turn=game->nb_turn;

                game->lastPlay.x=x;
                game->lastPlay.y=getY(game->board, x);

                game->current_player=switchPlayer(game->current_player);
                makePlay(game);

                value=min(value, minimaxAB(game, depth-1, !maximizingPlayer, p, alpha, beta));

                //restore
                game->board[game->lastPlay.x][game->lastPlay.y]=' ';
                game->lastPlay=savePlay;
                game->gameState=saveState;
                game->current_player=switchPlayer(game->current_player);
                game->nb_turn=saveNb_turn;

                if(alpha>=value)
                    return value;
                beta=min(beta, value);
            }
        }
        return value;
    }
}

unsigned int bestMove(Game *game){
    int bestMv = -1;
    int bestValue = -500000;
    char player=game->current_player;
    for (int x=0; x<7; x++) {
        if(isValidPlay(game->board, x)){
            Game child=copyGame(game);

            child.lastPlay.x=x;
            child.lastPlay.y=getY(game->board, x);
            makePlay(&child);
            int value = minimaxAB(&child, 9, false, player, -500000, 500000);
            printf("%i |  ",value);
            if (value > bestValue) {
                bestValue = value;
                bestMv = x;
            }

        }
    }
    return bestMv;
}

int main() {
    Game game;

    initBoard(game.board);
    game.gameState=PLAYING;
    game.current_player='x';
    game.nb_turn=0;
    unsigned int coups[42];
    printf("Connect 4 !\n");
    printBoard(game.board);
    while(game.gameState==PLAYING) {
        unsigned int choosen_x;
        if(game.current_player=='o' && false){
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
        }else{
            choosen_x=bestMove(&game);
            printf("IA plays in %d\n", choosen_x+1);
        }

        game.lastPlay.x=choosen_x;
        game.lastPlay.y=getY(game.board, choosen_x);
        makePlay(&game);
        coups[game.nb_turn-1]=choosen_x;

        printBoard(game.board);



        //switch player (1-0);
        printf("\n");
        game.current_player = switchPlayer(game.current_player);
    }

    if(game.gameState==DRAW)
        printf("Math is a draw !\n");
    else if(game.gameState==WIN)
        printf("Player %c win !\n", switchPlayer(game.current_player));


    for(unsigned int i=1; i<game.nb_turn; i+=2){
        printf("%u ", coups[i]+1);
    }
    printf("\n");

    for(unsigned int i=0; i<game.nb_turn; i+=2){
        printf("%u ", coups[i]+1);
    }
    printf("\n");


    return 0;
}
