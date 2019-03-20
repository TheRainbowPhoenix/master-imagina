#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

#include <GL/glut.h>

#include "Vector3.h"
#include "Point3.h"
#include "courbe.h"
#include "debug.h"

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

#define ORTHO 15.0

// PROGRAM VARIABLES

size_t FRAMES = 0;

// FUNCTIONS DECLARATIONS

void windowReshape(int width, int height);
void windowDisplay();
void windowTimer(int);

void normalKeyHandler(unsigned char key, int x, int y);
void specialKeyHandler(int key, int x, int y);
void mouseButtonHandler(int button, int state, int x, int y);
void mouseMotionHandler(int x, int y);

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
	glutMouseFunc(&mouseButtonHandler);   // Gestion des bouttons de la souris
	glutMotionFunc(&mouseMotionHandler);  // Gestion du mouvement de la souris

	initGL();
	initScene();

	glutMainLoop();

	return EXIT_SUCCESS;
}

void windowReshape(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-ORTHO, ORTHO, -ORTHO, ORTHO, -ORTHO, ORTHO);

	glMatrixMode(GL_MODELVIEW);
}

void windowDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	renderScene();  // Dessine les objets de la scene

	glutSwapBuffers();
}

void windowTimer(int) {
	// cerr << "FRAMES : " << FRAMES << "\n";

	FRAMES++;

	glutPostRedisplay();

	glutTimerFunc(1000.0 / FPS, windowTimer, 0);
}


size_t meridiens = 20;
size_t parallele = 20;

void normalKeyHandler(unsigned char key, int, int) {
	switch (key) {
		case KEY_ESC:
			exit(EXIT_SUCCESS);
			break;
		case 'u':
			meridiens--;
			break;
		case 'o':
			meridiens++;
			break;
		case ',':
			parallele--;
			break;
		case ':':
			parallele++;
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

double gl_ortho_size = 1;

double current_angle = 0;
double horiz_angle = 0;
double vert_angle = 0;

int prev_x = 0;
int prev_y = 0;

void mouseButtonHandler(int button, int state, int x, int y) {

	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				prev_x = x;
				prev_y = y;
				// std::cerr << "press left click at: (" << x << ", " << y << ")\n";
			} else if (state == GLUT_UP) {
				prev_x = -1;
				prev_y = -1;
				// std::cerr << "release left click at: (" << x << ", " << y << ")\n";
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				// std::cerr << "press right click at: (" << x << ", " << y << ")\n";
			} else if (state == GLUT_UP) {
				// std::cerr << "release right click at: (" << x << ", " << y << ")\n";
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN) {
				// std::cerr << "press middle click at: (" << x << ", " << y << ")\n";
			} else if (state == GLUT_UP) {
				// std::cerr << "release middle click at: (" << x << ", " << y << ")\n";
			}
			break;
		case 3:
			gl_ortho_size -= 0.01;
			// std::cerr << "press mouse wheel scroll up\n";
			break;
		case 4:
			gl_ortho_size += 0.01;
			// std::cerr << "press mouse wheel scroll down\n";
			break;
		default:
			break;
	}
}

void mouseMotionHandler(int x, int y) {
	// cerr << "mouse position : (" << x << ", " << y << ")\n";
	
	int dx = x - prev_x;
	int dy = y - prev_y;

	prev_x = x;
	prev_y = y;

	horiz_angle += dx * 0.5; /* assuming deltas are in pixels */
	vert_angle += dy * 0.5;

	if(vert_angle > 90) vert_angle = 90;
	if(vert_angle < -90) vert_angle = -90;
}

float light0Pos[] = {-5.0, 5.0, 5.0, 1.0};

void initGL() {
	glEnable(GL_DEPTH_TEST);  // Active le test de profondeur
	glEnable(GL_COLOR_MATERIAL); // Active les couleurs
	glEnable(GL_CULL_FACE);

	// glCullFace(GL_BACK);
	// glColor3d(0, 0, 1);

	// glCullFace(GL_FRONT);
	// glColor3d(1, 0, 0);
	
	glEnable(GL_LIGHTING);    // Active l'éclairage met tout en noir par defaut
	glEnable(GL_LIGHT0);
	
	glClearColor(RED, GREEN, BLUE, ALPHA);  // Initialisation de la couleur du fond
	glColor4f(1, 1, 1, 1); // Initialisation de la couleur d'écriture
	
	glClearDepth(1);  // Set background depth to farthest

	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	glPolygonMode(GL_FRONT, GL_LINE);
}

// float (*points_data)[3];
// float (*aretes_data)[3];

vector<Point3> maillage;
vector<vector<Point3> > figure;
Point3 p_start = {0, 0, 0};
Point3 p_end = {0, 10, 0};
double radius = 5;

void initScene() {

	// maillage = maillage_cylindre(p_start, p_end, radius, meridiens);
	// maillage = maillage_sphere(p_start, radius, meridiens, parallele);

	// glEnableClientState(GL_VERTEX_ARRAY);
	// glEnableClientState(GL_NORMAL_ARRAY);

	// glVertexPointer(3, GL_FLOAT, 0, points);
	// glNormalPointer(GL_FLOAT, 0, normals);
	
}

void cameraOrbitAround(const Point3& target) {
  Vector3 pos = Vector3(sin(glutGet(GLUT_ELAPSED_TIME)/1000.0), 1, cos(glutGet(GLUT_ELAPSED_TIME)/1000.0));
  gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, 0, 1, 0);
}

// Point3 cameraPosition() {
// 	int viewport[4];
// 	double camera_pos[3];
// 	double matModelView[16]; 
// 	double matProjection[16];

// 	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView); 
// 	glGetDoublev(GL_PROJECTION_MATRIX, matProjection); 
// 	glGetIntegerv(GL_VIEWPORT, viewport); 
// 	gluUnProject((viewport[2]-viewport[0])/2, (viewport[3]-viewport[1])/2, 0.0, matModelView, matProjection, viewport, &camera_pos[0],&camera_pos[1],&camera_pos[2]);

// 	return Point3(camera_pos[0], camera_pos[1], camera_pos[2]);
// }

void renderScene() {

	// GESTION DE LA CAMERA

	glOrtho(-gl_ortho_size, gl_ortho_size, -gl_ortho_size, gl_ortho_size, gl_ortho_size, -gl_ortho_size);
	glRotatef(vert_angle, 1, 0, 0);
	glRotatef(horiz_angle, 0, 1, 0);

	// GESTION DES LUMIÈRES

	glPointSize(5);

	double light0Radius = ORTHO * (gl_ortho_size);

	light0Pos[0] = sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0) * light0Radius;
	light0Pos[1] = 1 * light0Radius / 5;
	light0Pos[2] = cos(glutGet(GLUT_ELAPSED_TIME) / 1000.0) * light0Radius;

	glBegin(GL_POINTS);
		glColor3f(1, 1, 0); glVertex3fv(light0Pos);
	glEnd();

	float light0Diff[] = {1.0, 0.2, 0.2, 1.0}; 
	float light0Spec[] = {1.0, 0.2, 0.2, 1.0}; 
	float light0Amb[]  = {0.5, 0.5, 0.5, 1.0};

	float matDiff[] = {1.0, 1.0, 1.0, 1.0}; 
	float matSpec[] = {1.0, 1.0, 1.0, 1.0}; 
	float matAmb[]  = {1.0, 1.0, 1.0, 1.0}; 

	float matShininess[] = {15.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);

	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

	// GESTION DES DESSINS
	
	glColor3f(1, 0, 0);

	maillage = maillage_sphere(p_start, radius, meridiens, parallele);
	draw_maillage(maillage);

	gl_check_error("all");
}
