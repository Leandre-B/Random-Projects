#include "ia.h"


unsigned int nbPossibleAlingFromPos(Game *game, Vect2 direction){
    unsigned int tot=0;
    Point auxPlay=game->lastPlay;
    while(  auxPlay.x>=0 && auxPlay.x<7     &&
            auxPlay.y>=0 && auxPlay.y<6     &&
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
        //sign is inversed because we change player after making the move
        return (game->currentPlayer->symbole==p ? -400000+depth : 400000+depth);

    unsigned int nb_align_o=0;
    unsigned int nb_align_x=0;
    Point saveLastPlay = game->lastPlay;
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
    game->lastPlay = saveLastPlay;
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

                Point saveLastPlay = game->lastPlay;
                enum GameState saveState = game->gameState;
                unsigned int saveNbTurn=game->nbTurn;

                makePlay(game, x);

                value=max(value, minimaxAB(game, depth-1, !maximizingPlayer, p, alpha, beta));

                //restore
                game->board[game->lastPlay.x][game->lastPlay.y]=' ';
                game->lastPlay=saveLastPlay;
                game->gameState=saveState;
                switchPlayer(game);
                game->nbTurn=saveNbTurn;

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

                Point saveLastPlay = game->lastPlay;
                enum GameState saveState = game->gameState;
                unsigned int saveNbTurn=game->nbTurn;

                makePlay(game, x);

                value=min(value, minimaxAB(game, depth-1, !maximizingPlayer, p, alpha, beta));

                //restore
                game->board[game->lastPlay.x][game->lastPlay.y]=' ';
                game->lastPlay=saveLastPlay;
                game->gameState=saveState;
                switchPlayer(game);
                game->nbTurn=saveNbTurn;

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
    char player=game->currentPlayer->symbole;
    // printf("check for\"%c\"\n", game->currentPlayer->symbole);
    for (int x=0; x<7; x++) {
        if(isValidPlay(game->board, x)){
            Point saveLastPlay = game->lastPlay;
            enum GameState saveState = game->gameState;
            unsigned int saveNbTurn=game->nbTurn;

            makePlay(game, x);
            int value = minimaxAB(game, 7, false, player, -500000, 500000);
            printf("%i | ",value);
            if (value > bestValue) {
                bestValue = value;
                bestMv = x;
            }

            //restore
            game->board[game->lastPlay.x][game->lastPlay.y]=' ';
            game->lastPlay=saveLastPlay;
            game->gameState=saveState;
            switchPlayer(game);
            game->nbTurn=saveNbTurn;

        }
    }
    return bestMv;
}