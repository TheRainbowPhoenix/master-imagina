#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <GL/glut.h>
#include <unistd.h>

#include "Vector3.h"
#include "Point3.h"

using namespace std;

// PROGRAM CONSTANTES

#define RED 0.7
#define GREEN 0.7
#define BLUE 0.7
#define ALPHA 1

#define FPS 60

#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_ENTR 13

// PROGRAM VARIABLES

size_t FRAMES = 0;

// FUNCTIONS DECLARATIONS

void windowReshape(int width, int height);
void windowDisplay();
void windowTimer(int);

void normalKeyHandler(unsigned char key, int x, int y);
void specialKeyHandler(int key, int x, int y);

void initGL();
void initScene();

void renderScene();

// FUNCTIONS DEFINITIONS

int main(int argc, char** argv) {
	srand(time(0));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Premier exemple : carré");

	glutReshapeFunc(&windowReshape);  // Redimensionnement de la fenêtre
	glutDisplayFunc(&windowDisplay);  // Tracé graphique
	glutTimerFunc(1000.0 / FPS, windowTimer, 0); // Met à jour l'affichage en fonction du nombre de FPS

	glutKeyboardFunc(&normalKeyHandler);  // Gestion des événements clavier
	glutSpecialFunc(&specialKeyHandler);  // Gestion des événements clavier spéciaux

	initGL();
	initScene();

	glutMainLoop();

	return EXIT_SUCCESS;
}

void windowReshape(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);

	glMatrixMode(GL_MODELVIEW);
}

void windowDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	renderScene();  // Dessine les objets de la scene

	glutSwapBuffers();
	//glutPostRedisplay();
}

void windowTimer(int) {
	//cerr << "FRAMES : " << FRAMES << "\n";

	FRAMES++;

	glutPostRedisplay();

	glutTimerFunc(1000.0 / FPS, windowTimer, 0);
}

void normalKeyHandler(unsigned char key, int, int) {
	switch (key) {
		case KEY_ESC:
			exit(EXIT_SUCCESS);
			break;
		default:
			printf("La touche %d n´est pas active.\n", key);
			break;
	}
}

void specialKeyHandler(int key, int, int) {
	switch (key) {
		default:
			printf("La touche %d n´est pas active.\n", key);
			break;
	}
}

void initGL() {
	glEnable(GL_DEPTH_TEST);  // Active le test de profondeur
	glEnable(GL_COLOR_MATERIAL); // Active les couleurs
	//glEnable(GL_LIGHTING);    // Active l'éclairage met tout en noir par defaut
	
	glClearColor(RED, GREEN, BLUE, ALPHA);  // Initialisation de la couleur du fond
	glColor4f(1, 1, 1, 1); // Initialisation de la couleur d'écriture
	
	glClearDepth(1);  // Set background depth to farthest

	// glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	// glShadeModel(GL_SMOOTH);   // Enable smooth shading
	// glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

}

void initScene() {}


void cameraOrbitAround(const Point3& target) {

  Vector3 pos = Vector3(sin(glutGet(GLUT_ELAPSED_TIME)/1000.0), 1, cos(glutGet(GLUT_ELAPSED_TIME)/1000.0));

  gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, 0, 1, 0);
}

void renderScene() {

	cameraOrbitAround({0, 0, 0});

	glPointSize(5);

glBegin(GL_QUADS);

glColor3d(1,0,0);glVertex3i(1,1,1);
glColor3d(0,1,0);glVertex3i(1,-1,1);
glColor3d(0,0,1);glVertex3i(-1,-1,1);
glColor3d(1,0,1);glVertex3i(-1,1,1);
	//1 face

glColor3d(1,1,0);glVertex3i(1,1,-1);
glColor3d(0,1,1);glVertex3i(1,-1,-1);
glColor3d(1,0,0);glVertex3i(-1,-1,-1);
glColor3d(1,1,1);glVertex3i(-1,1,-1);
	//2 faces

glColor3d(1,0,0);glVertex3i(1,1,1);
glColor3d(0,1,0);glVertex3i(1,-1,1);
glColor3d(0,1,1);glVertex3i(1,-1,-1);
glColor3d(1,1,0);glVertex3i(1,1,-1);
	//3 faces

glColor3d(1,0,1);glVertex3i(-1,1,1);
glColor3d(0,0,1);glVertex3i(-1,-1,1);
glColor3d(1,0,0);glVertex3i(-1,-1,-1);
glColor3d(1,1,1);glVertex3i(-1,1,-1);
	//4 faces

glColor3d(1,1,1);glVertex3i(-1,1,-1);
glColor3d(1,0,1);glVertex3i(-1,1,1);
glColor3d(1,0,0);glVertex3i(1,1,1);
glColor3d(1,1,0);glVertex3i(1,1,-1);
	//5 faces

glColor3d(1,0,0);glVertex3i(-1,-1,-1);
glColor3d(0,0,1);glVertex3i(-1,-1,1);
glColor3d(0,1,0);glVertex3i(1,-1,1);
glColor3d(0,1,1);glVertex3i(1,-1,-1);
	//6 faces
glEnd();

}
