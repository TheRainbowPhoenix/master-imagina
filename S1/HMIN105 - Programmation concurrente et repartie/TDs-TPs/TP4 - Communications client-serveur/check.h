#ifndef CHECK_H
#define CHECK_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* Les macros commençant par CHECK permettent de verifier si des erreurs sont presentent dans le programme. */

/* Verifie si l'allocation d'un descripteur de fichier s'est bien passée, affiche l'erreur et quitte le programme si ce n'est pas le cas. */
#define CHECK_FD(FD) 								\
if(FD < 0){											\
	fprintf(stderr, "Erreur : %s\n", strerror(FD));	\
	exit(FD);										\
}													\

/* Verifie si l'allocation d'un fichier s'est bien passée, affiche l'erreur et quitte le programme si ce n'est pas le cas. */
#define CHECK_FILE(F, FNAME) 													\
if(F == NULL){																	\
	fprintf(stderr, "Erreur : Problème à l'ouverture du fichier %s\n", FNAME);	\
	exit(1); 																	\
}																				\

/* Verifie si le numero passé en paramètre correspond à une erreur, affiche l'erreur et quitte le programme si ce n'est pas le cas. */
#define CHECK_ERROR(ERRNO) 								\
if(ERRNO < 0){											\
	fprintf(stderr, "Erreur : %s\n", strerror(ERRNO));	\
	exit(ERRNO);										\
}														\

/* Verifie si les infos renvoyées par la fonction getaddrinfo sont valide, affiche l'erreur et quitte le programme si ça n'est pas le cas. */
#define CHECK_GAI_ERROR(ERRNO) 									\
if(ERRNO){														\
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ERRNO));	\
	exit(ERRNO);												\
}																\

#endif