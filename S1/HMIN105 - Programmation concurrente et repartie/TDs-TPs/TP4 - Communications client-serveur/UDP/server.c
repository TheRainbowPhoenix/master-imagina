#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "warn.h" // Verifications d'erreurs

int main(int argc, char const *argv[]){

    PRINT_USAGE_IF(argc < 2, "Usage %s <PORT>\n", argv[0]);

    const char* port = argv[1];

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET; // INET for IPv4
    hints.ai_socktype =  SOCK_DGRAM;
    hints.ai_flags =  AI_PASSIVE;

    printf("Accès aux informations liées au port %s...\n", port);

    // Pas besoin de spécifier l'hote comme nous créons un serveur.
    int error = getaddrinfo(NULL, port, &hints, &res); WARN_ERROR_GAI(error);

    printf("Création du socket...\n");

    int sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); WARN_ERROR(sock_fd);

    // Permet de réutilliser le port directement après la fin du programme
    // int optval = 1;
    // setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    char address[256] = "";
    // Convertion de l'adresse IP en chaine de charactère.
    inet_ntop(AF_INET, &((struct sockaddr_in*)res->ai_addr)->sin_addr, address, sizeof(address)); WARN_ERROR_IF(strcmp(address, "") == 0);

    printf("Affectation de l'adresse %s au socket...\n", address);

    error = bind(sock_fd, res->ai_addr, res->ai_addrlen); WARN_ERROR(error);

    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);

    printf("Serveur UDP en attente de connexion...\n");
    printf("IP : %s, PORT : %s\n", address, port);

    while(1){

        char* msg = NULL;
        size_t msg_len = 0;
        ssize_t read_size = 0;

        read_size = recvfrom(sock_fd, &msg_len, sizeof(msg_len), 0, (struct sockaddr*)&addr, &addrlen); WARN_ERROR(read_size);
        
        msg = (char*)malloc(msg_len * sizeof(char));

        read_size = recvfrom(sock_fd, msg, msg_len, 0, (struct sockaddr*)&addr, &addrlen); WARN_ERROR(read_size);

        char host[NI_MAXSERV];
        char serv[NI_MAXSERV];

        error = getnameinfo((struct sockaddr*)&addr, addrlen, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV); WARN_ERROR_GAI(error);

        printf("┌[Message provenant de l'IP : %s, PORT : %s]\n", host, serv);
        printf("└─▶ %s", msg);
        printf("Réponse : ");

        read_size = getline(&msg, &msg_len, stdin); WARN_ERROR(read_size);
        
        // Envoie la taille de la chaine de character.
        read_size = sendto(sock_fd, &msg_len, sizeof(msg_len), 0, (struct sockaddr *)&addr, sizeof(addr)); WARN_ERROR(read_size);

        // Envoie la chaine de character
        read_size = sendto(sock_fd, msg, msg_len, 0, (struct sockaddr *)&addr, sizeof(addr)); WARN_ERROR(read_size);

        free(msg);
    }

    close(sock_fd);

    return 0;
}
