#include "game.h"
#include "ia.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

void playOffline(){
    printf("Connect 4 !\n");

    Game game;
    initGame(&game, false);
    printBoard(game.board);

    while(game.gameState==PLAYING) {
        printf("Player's %c turn\n", game.currentPlayer->symbole);

        unsigned int choosen_x;
        if(game.currentPlayer->type==HUMAN){
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
        }else { //game->playerType==IA
            choosen_x=bestMove(&game);
            printf("IA (%c)plays in %d\n", game.currentPlayer->symbole, choosen_x+1);
        }

        makePlay(&game, choosen_x);
        printBoard(game.board);
        printf("\n");
    }

    if(game.gameState==DRAW)
        printf("Math is a draw !\n");
    else if(game.gameState==WIN)
        printf("Win !\n", game.currentPlayer->symbole);


    for(unsigned int i=1; i<game.nbTurn; i+=2){
        printf("%u ", game.coups[i]+1);
    }
    printf("\n");

    for(unsigned int i=0; i<game.nbTurn; i+=2){
        printf("%u ", game.coups[i]+1);
    }
    printf("\n");
}

void playOnline(){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        AF_INET,
        htons(9999),
        INADDR_ANY
    };

    //inet_pton(AF_INET, "10.60.22.33", &address.sin_addr);

    if(connect(sockfd, (struct sockaddr*)&address, sizeof(address))==0){
        printf("Connected to the server.\n");
    }else{
        printf("Cannot connect to the server. Check if it's up.\n");
        return;
    }

    // stdin - 0
    struct pollfd fds[2] = {
        {0, POLLIN, 0},
        {sockfd, POLLIN, 0},
    };

    Game game;
    initGame(&game, true);
    printBoard(game.board);
    unsigned int choosen_x;
    bool myTurn=false;
    bool ready=false;
    while(game.gameState==PLAYING) {
        char buffer[256] = { 0 };
        poll(fds, 2, -1);

        if(fds[0].revents & POLLIN) {
            if(myTurn){
                sscanf(buffer, "%d", choosen_x);
                while(!isValidPlay(game.board, choosen_x)) {
                    printf("Not valid, retry\n");
                    while (scanf("%u", &choosen_x)!=1){
                        printf("Error : not an number !\n");

                        //empty buffer
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);

                        printf("Not valid, retry\n");
                    }
                    printf("%u\n", choosen_x);
                    --choosen_x;
                }
                send(sockfd, buffer, strlen(buffer), 0);

                makePlay(&game, choosen_x);
                printBoard(game.board);
                printf("\n");

                myTurn=false;
            }else{
                printf("It's not yout turn yet.\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
        }
        if (fds[1].revents & POLLIN) {
            if (recv(sockfd, buffer, sizeof(buffer), 0) == 0) {
                return;
            }
            if(ready){
                printf("Opponent plays in : %s\n", buffer);

                makePlay(&game, choosen_x);
                printBoard(game.board);
                printf("\n");
                sscanf(&buffer[0], "%d", choosen_x);
            }

            if(strcmp(buffer, "READY\n") == 0){
                ready=true;
                printf("Game can start\n");
                printf("Choose where you want to play (1-7) : \n");
                myTurn=true;
            }else if(strcmp(buffer, "YOUR_TURN\n") == 0){
                printf("Choose where you want to play (1-7) : \n");
                myTurn=true;
            }
        }
    }
}

int main() {
    unsigned int playMethod=0;
    do {
        printf("Play offline or online ?\n1 - Online\n2 - Offline\n");
        while (scanf("%u", &playMethod)!=1){
            printf("Error : not an number !\n");

            //empty buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Please retry :\n");
        }
    }while(playMethod!=1 && playMethod!=2);


    if(playMethod==1)
        playOnline();
    else
        playOffline();


    return 0;
}
