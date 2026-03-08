#pragma once

#include "utils.h"

#include <stdio.h>
#include  <stdlib.h>
#include <stdbool.h>

typedef char Board[7][6];

enum GameState {
    PLAYING, DRAW, WIN
};

enum PlayerType {
    IA, HUMAN
};

typedef struct Player {
    enum PlayerType type;
    char symbole;
}Player;

typedef struct Game{
    Board board;
    enum GameState gameState;

    Player *currentPlayer;
    Player player1;
    Player player2;

    Point lastPlay;
    unsigned int nbTurn;
    unsigned int coups[42];
}Game;


void initGame(Game *game, bool isOnline);

void printBoard(const Board board);
int getY(Board board, unsigned int x);
char switchPlayer(Game* game);

//for player and IA : called only if isValidPlay()
void makePlay(Game *game, unsigned int x);

bool isValidPlay(Board board, unsigned int x);