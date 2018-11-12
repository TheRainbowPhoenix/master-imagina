#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "warnevent.h" // Verifications d'erreurs

int main(int argc, char const *argv[]){

	PRINT_USAGE_IF(argc < 3, "Usage %s <ADRESS> <PORT>\n", argv[0]);
	
	const char* adr = argv[1];
	short port = atoi(argv[2]);

	printf("Création du socket...\n");

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0); WARN_ERROR(sock_fd);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

   	// Convertion de L'IP (chaine de charactère) en adresse network.
	int res = inet_pton(AF_INET, adr, &(serv_addr.sin_addr)); WARN_ERROR(res);

	char* msg = NULL;
	size_t msg_len = 0;
	ssize_t read_size = 0;

	// Permet de réutilliser le port directement après la fin du programme
    // int optval = 1;
    // setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

	printf("Message a envoyer : ");

	read_size = getline(&msg, &msg_len, stdin); WARN_ERROR(read_size);
	
	// Envoie la taille de la chaine de character.
	read_size = sendto(sock_fd, &msg_len, sizeof(msg_len), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); WARN_ERROR(read_size);

	// Envoie la chaine de character
	read_size = sendto(sock_fd, msg, msg_len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); WARN_ERROR(read_size);

	free(msg);

	socklen_t addrlen = sizeof(serv_addr);

	printf("Client UDP en attente de réponse...\n");

    read_size = recvfrom(sock_fd, &msg_len, sizeof(msg_len), 0, (struct sockaddr*)&serv_addr, &addrlen); WARN_ERROR(read_size);
    
    msg = malloc(msg_len * sizeof(char));

    read_size = recvfrom(sock_fd, msg, msg_len, 0, (struct sockaddr*)&serv_addr, &addrlen); WARN_ERROR(read_size);

    printf("Réponse reçu : %s",msg);

    free(msg);
	close(sock_fd);
	
	return 0;
}
