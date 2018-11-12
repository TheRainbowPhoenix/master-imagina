#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "check.h" //Verifications d'erreurs

int main(int argc, char const *argv[])
{
	const char* port;

	if(argc < 2){
		fprintf(stderr, "Usage %s <PORT>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	port = argv[1];

    printf("Création de la socket...\n");

    int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    CHECK_FD(sock_fd)
    
    printf("Accès à l'adresse liée au port %s...\n", port);

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET; //INET6 for IPv6
    hints.ai_socktype = SOCK_STREAM;   //TCP
    hints.ai_flags = AI_PASSIVE;

    //Pas besoin de spécifier l'hote comme nous créons un serveur.
	getaddrinfo(NULL, port, &hints, &res);

	printf("Affectation de l'adresse à la socket...\n");

    //Permet de réutilliser le port directement après la fin du programme
    int optval = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    char address[256] = "";
    //Convertion de l'adresse IP en chaine de charactère.
    inet_ntop(AF_INET, &((struct sockaddr_in*)res->ai_addr)->sin_addr, address, sizeof(address));

	int error = bind(sock_fd, res->ai_addr, res->ai_addrlen);
	CHECK_ERROR(error)

    error = listen(sock_fd, 10);
    CHECK_GAI_ERROR(error);

    struct sockaddr_in* res_addr = (struct sockaddr_in*)res->ai_addr;

    printf("En ecoute sur le descripteur de fichier %d, port %d...\n", sock_fd, ntohs(res_addr->sin_port));

	printf("Serveur TCP en attente de connexion...\n");
	printf("IP : %s, PORT : %s\n", address, port);

    struct sockaddr_in client_addr;
    socklen_t client_lg = sizeof(struct sockaddr_in);

    int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_lg);
    //int client_fd = accept(sock_fd, NULL, NULL); seulement pour le descripteur de fichier
    CHECK_FD(client_fd)

    printf("Connexion en cours avec le client %d\n", client_fd);

    char msg[1024] = "";

    printf("Reception du message...\n");

    int len = recv(client_fd, msg, sizeof(msg), 0);
    msg[len] = '\0';

    printf("%s\n", msg);

    printf("Confirmation d'arrivée...\n");

    send(client_fd, &len, sizeof(int), 0);

    close(client_fd);
    close(sock_fd);

	return 0;
}
