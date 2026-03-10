#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main() {
    //                  IPV4       TCP      default
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        AF_INET,
        htons(9999),
        INADDR_ANY,
    };

    int opt=1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(sockfd, (struct sockaddr*)&address, sizeof(address));

    listen(sockfd, 2);

    int clientfd_1 = accept(sockfd, 0, 0);
    printf("Client 1 connecté.\n");

    int clientfd_2 = accept(sockfd, 0, 0);
    printf("Client 2 connecté.\n");

    // stdin - 0
    struct pollfd fds[3] = {
        { 0, POLLIN, 0 },
        { clientfd_1, POLLIN, 0 },
        { clientfd_2, POLLIN, 0 },
    };

    bool firstMsg=true;
    for (;;) {

        if(firstMsg){
            printf("Envoie signle turn\n");
            char msg[]="YOUR_TURN\n";
            send(clientfd_1, msg, strlen(msg), 0);
            firstMsg=false;
        }

        char buffer[256] = { 0 };
        poll(fds, 3, -1);

        //envoie message au clients
        if (fds[0].revents & POLLIN) {
            read(0, buffer, sizeof(buffer));
            send(clientfd_1, buffer, strlen(buffer), 0);
            send(clientfd_2, buffer, strlen(buffer), 0);
        }

        //recupere message client1 et l'envoie au client2
        if (fds[1].revents & POLLIN) {
            if (recv(clientfd_1, buffer, sizeof(buffer), 0) == 0) {
                printf("Client 1 déconnecté\n");
                return 0;
            }

            printf("Client 1 dit : %s\n", buffer);
            send(clientfd_2, buffer, strlen(buffer), 0);

            sleep(0); //separe les 2 send
            char msg[]="YOUR_TURN\n";
            send(clientfd_2, msg, strlen(msg), 0);
        }

        //recupere message client2 et l'envoie au client1
        if (fds[2].revents & POLLIN) {
            if (recv(clientfd_2, buffer, sizeof(buffer), 0) == 0) {
                printf("Client 2 déconnecté\n");
                return 0;
            }

            printf("Client 2 dit : %s\n", buffer);
            send(clientfd_1, buffer, strlen(buffer), 0);
            sleep(0);
            char msg[]="YOUR_TURN\n";
            send(clientfd_1, msg, strlen(msg), 0);
        }
    }

    return 0;
}
