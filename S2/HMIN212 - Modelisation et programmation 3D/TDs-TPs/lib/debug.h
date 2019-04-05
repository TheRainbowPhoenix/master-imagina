#ifndef WARN_ERROR_H
#define WARN_ERROR_H

#include <stdio.h>	// fprintf()
#include <errno.h>  // errno
#include <string.h>	// strerror()
#include <netdb.h>	// gai_strerror()

#define WARN_DEF_OUTPUT stderr
#define WARN_DEF_TYPE "ERROR"
#define WARN_DEF_ACTION exit(EXIT_FAILURE)
#define WARN_DO_NOTHING (void)0

// Commenter cette constante pour enlever tout les avertissement
#define WARN_ALL_EVENT

// Affiche des données en precisant la sortie, le type de message, le
// fichier, la ligne et la fonction depuis laquel l'affichage est appelé
#define LOG_DATA(LOG_OUTPUT, LOG_TYPE, FMT, ...) \
fprintf(LOG_OUTPUT, "[%s] in \'%s\' at \'%s:%d\' : " FMT, LOG_TYPE, __func__, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(FMT, ...) LOG_DATA(stderr, "INFO", FMT, ##__VA_ARGS__)
#define LOG_WARNING(FMT, ...) LOG_DATA(stderr, "WARNING", FMT, ##__VA_ARGS__)
#define LOG_ERROR(FMT, ...) LOG_DATA(stderr, "ERROR", FMT, ##__VA_ARGS__)

#ifdef WARN_ALL_EVENT
// Si la condition est vrai, affiche un message et applique une action.
#define WARN_EVENT(WARN_COND, WARN_OUTPUT, WARN_TYPE, WARN_DESC, WARN_ACTION, WARN_FMT, ...) \
if(WARN_COND)({ \
	LOG_DATA(WARN_OUTPUT, WARN_TYPE, "%s ("#WARN_COND")" WARN_FMT, WARN_DESC, ##__VA_ARGS__); \
	WARN_ACTION; \
})
#else
// La macro ne fera rien si WARN_ALL_EVENT n'est pas définie.
#define WARN_EVENT(WARN_COND, WARN_OUTPUT, WARN_TYPE, WARN_DESC, WARN_ACTION, WARN_FMT, ...)
#endif

// Previens l'utilisateur avec un warning si la condition est vrai
#define WARN_IF(COND) \
WARN_EVENT(COND, \
	WARN_DEF_OUTPUT, \
	"WARNING", \
	"warning condition verified", \
	WARN_DO_NOTHING, \
	"\n")

// Si la condition est vrai, l'utilisateur est prevenu d'une erreur,
// la valeur de errno est affichée et une action adaptée est appliquée.
#define WARN_ERROR_IF(COND) \
WARN_EVENT(COND, \
	WARN_DEF_OUTPUT, \
	WARN_DEF_TYPE, \
	"error condition verified", \
	WARN_DEF_ACTION, \
	"\n[ERRNO] (%s)\n", \
	strerror(errno))

// Verifie si une fonction à renvoyée -1 puis applique une action
// adapté.
#define WARN_ERROR(RES) \
WARN_EVENT(RES == -1, \
	WARN_DEF_OUTPUT, \
	WARN_DEF_TYPE, \
	"error code detected", \
	WARN_DEF_ACTION, \
	"\n[ERRNO] %s\n", \
	strerror(errno))

// Verifie si une fonction pthread à renvoyée un code d'erreur et
// applique une action adapté.
#define WARN_ERROR_PTHREAD(RES) \
WARN_EVENT(RES != 0, \
	WARN_DEF_OUTPUT, \
	WARN_DEF_TYPE, \
	"pthread error code detected", \
	WARN_DEF_ACTION, \
	"\n[ERRNO] %s\n", \
	strerror(errno))

// Verifie si une fonction getaddrinfo ou getnameinfo à renvoyée un
// code d'erreur et applique une action adapté.
#define WARN_ERROR_GAI(RES) \
WARN_EVENT(RES != 0, \
	WARN_DEF_OUTPUT, \
	WARN_DEF_TYPE, \
	"getaddrinfo error code detected", \
	WARN_DEF_ACTION, \
	"\n[ERRNO] %s\n", \
	gai_strerror(errno))

// Affiche l'usage du programme quand la condition est vérifié.
#define PRINT_USAGE_IF(COND, FMT, ...) \
if(COND)({ \
	fprintf(stderr, FMT, ##__VA_ARGS__); \
	exit(EXIT_FAILURE); \
})

#include <GL/gl.h>

#if __has_include(<GL/gl.h>)

const char* gl_error_interpreter(GLenum error_code) {
	switch(error_code)
	{
		case GL_INVALID_ENUM :
			return "Invalid enumeration";
		case GL_INVALID_VALUE :
			return "Invalid value";
		case GL_INVALID_OPERATION :
			return "Invalid operation";
		case GL_STACK_OVERFLOW :
			return "Stack overflow";
		case GL_STACK_UNDERFLOW :
			return "Stack underflow";
		case GL_OUT_OF_MEMORY :
			return "Out of memory";
		case GL_INVALID_FRAMEBUFFER_OPERATION :
			return "Invalid framebuffer operation";
		case GL_CONTEXT_LOST :
			return "Context lost";
		case GL_TABLE_TOO_LARGE :
			return "Table too large";
		default :
			return "Unknown";
	}
}

void gl_clear_error() {
	while(glGetError() != GL_NO_ERROR);
}

bool gl_check_error(const char* error_func) {
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "[GL_ERROR] in \'%s:%d\' by \'%s\' at \'%s\': %s", __FILE__, __LINE__, __func__, error_func, gl_error_interpreter(err));
		return true;
	}
	return false;
}

#define GL_WARN_ERROR(func) gl_clear_error(); do { if(gl_check_error(#func)) { exit(EXIT_FAILURE); } } while (0)

// [<GL_>ERROR] in <warn.h>:<15> at <cameraOrbitAround(const Point3& target)> : Invalid argument
#endif

#endif