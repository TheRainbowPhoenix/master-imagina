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
	char msg[1024] = "";
	const char* adr;
	const char* port;

	if(argc < 4){
		fprintf(stderr, "Usage %s <MESSAGE> <ADRESS> <PORT>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	strcpy(msg, argv[1]);
	adr = argv[2];
	port = argv[3];

	printf("Création de la socket...\n");

	int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	CHECK_FD(sock_fd)

	printf("Accès aux information de l'adresse %s et du port %s...\n", adr, port);

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET; //IPv4 only
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;	

    int error = getaddrinfo(adr, port, &hints, &res);
    CHECK_GAI_ERROR(error);

	printf("Connection à l'adresse %s via le port %s...\n", adr, port);

	error = connect(sock_fd, res->ai_addr, res->ai_addrlen);
    CHECK_ERROR(error);

    printf("Envoi du message %s...\n", msg);

    send(sock_fd, msg, sizeof(msg), 0);

    printf("Attente de reponse...\n");

   	int resp = 0;// char resp[1024] = "";

	int len = recv(sock_fd, &resp, sizeof(int), 0);

	printf("%d\n", resp);

	close(sock_fd);
	return 0;
}