#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <limits>

#include <GL/glut.h>

#include "Vector3.h"
#include "Point3.h"
#include "courbe.h"

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
	//glutPostRedisplay();
}

void windowTimer(int) {
	//cerr << "FRAMES : " << FRAMES << "\n";

	FRAMES++;

	glutPostRedisplay();

	glutTimerFunc(1000.0 / FPS, windowTimer, 0);
}

bool toggle_t = false;
double grid_unit_size = 1; 

void normalKeyHandler(unsigned char key, int, int) {
	switch (key) {
		case KEY_ESC:
			exit(EXIT_SUCCESS);
			break;
		case 't':
			toggle_t = !toggle_t;
			break;
		case 'o':
			grid_unit_size += 0.1;
			break;
		case 'u':
			grid_unit_size = grid_unit_size > 0 ? grid_unit_size - 0.1 : grid_unit_size;
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

Vector3 left_button_press_pos;
bool left_button_pressed = false;
Vector3 last_mouse_pos;
Vector3 current_mouse_pos;
Vector3 camera_current_pos = {0, 0, ORTHO};

double gl_ortho_size = 1;


double current_angle = 0;

int prev_x = 0;
int prev_y = 0;

void mouseButtonHandler(int button, int state, int x, int y) {

	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				left_button_press_pos = {(double)x, (double)y};
				left_button_pressed = true;
				prev_x = x;
				prev_y = y;
				//std::cerr << "press left click at: (" << x << ", " << y << ")\n";
			} else if (state == GLUT_UP) {
				prev_x = -1;
				prev_y = -1;
				//std::cerr << "release left click at: (" << x << ", " << y << ")\n";
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				//std::cerr << "press right click at: (" << x << ", " << y << ")\n";
			} else if (state == GLUT_UP) {
				//std::cerr << "release right click at: (" << x << ", " << y << ")\n";
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN) {
				//std::cerr << "press middle click at: (" << x << ", " << y << ")\n";
			} else if (state == GLUT_UP) {
				//std::cerr << "release middle click at: (" << x << ", " << y << ")\n";
			}
			break;
		case 3:
			gl_ortho_size -= 0.01;
			//std::cerr << "press mouse wheel scroll up\n";
			break;
		case 4:
			gl_ortho_size += 0.01;
			//std::cerr << "press mouse wheel scroll down\n";
			break;
		default:
			break;
	}
}


double horiz_angle = 0;
double vert_angle = 0;

void mouseMotionHandler(int x, int y) {
	last_mouse_pos = current_mouse_pos;
	cout << "mouse position : (" << x << ", " << y << ")\n";
	current_mouse_pos = {(double)x, (double)y};

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

	// glPolygonMode(GL_FRONT, GL_LINE);
}


int nb_sommets = 0;
int nb_triangles = 0;
int nb_aretes = 0;

vector<Vector3> points;
vector<Vector3> aretes;

// float (*points_data)[3];
// float (*aretes_data)[3];

Vector3 min_pos(numeric_limits<double>::max(), numeric_limits<double>::max(), numeric_limits<double>::max());
Vector3 max_pos(numeric_limits<double>::min(), numeric_limits<double>::min(), numeric_limits<double>::min());



void cameraOrbitAround(const Point3& target) {

  Vector3 pos = Vector3(sin(glutGet(GLUT_ELAPSED_TIME)/1000.0), 1, cos(glutGet(GLUT_ELAPSED_TIME)/1000.0));

  gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, 0, 1, 0);
}

Point3 fusion(const Point3& a, const Point3& b) {
	Vector3 ab = b - a;
	return a + (ab / 2);
}

void fusion(vector<Vector3>& points, vector<Vector3>& aretes, int point1_index, int point2_index)
{
	Point3 fusion_point = fusion((Point3)points[point1_index], (Point3)points[point2_index]);
	points.push_back((Vector3)fusion_point);

	for (Vector3& v : aretes)
	{
		if (v.x == point1_index) 
			v.x = points.size() - 1;
		else if (v.x == point2_index)
			v.x = points.size() - 1;
		
		if (v.y == point1_index)
			v.y = points.size() - 1;
		else if (v.y == point2_index)
			v.y = points.size() - 1;
		
		if (v.z == point1_index)
			v.z = points.size() - 1;
		else if (v.z == point2_index)
			v.z = points.size() - 1;
	}
}

struct Grid3
{
	Vector3 position;
	Vector3 dimension;
	double unit_size;

	Grid3() : position(), dimension(), unit_size() {}
	Grid3(Vector3 pos, Vector3 dim, double size) : position(pos), dimension(dim), unit_size(size) {}

	void draw2d(Vector3 pos)
	{
		if (unit_size == 0)
			return;

		for (int i = 0; i <= ceil(dimension.y / unit_size); ++i)
		{
			Point3 start = (Point3)pos + Vector3::up * i * unit_size;
			draw_line(start, start + Vector3::right * ceil(dimension.x / unit_size) * unit_size);
		}

		for (int j = 0 ; j <= ceil(dimension.x / unit_size); ++j)
		{
			Point3 start = (Point3)pos + Vector3::right * j * unit_size;
			draw_line(start, start + Vector3::up * ceil(dimension.y / unit_size) * unit_size);
		}
	}

	void draw()
	{
		if (unit_size == 0)
			return;

		for (int k = 0; k <= ceil(dimension.z / unit_size); ++k)
		{
			draw2d(position + Vector3::forward * k * unit_size);
		}
	}
};


// Renvoie la position du point dans la grille (des chiffres rond)
Vector3 grid_position(const Grid3& grid, const Vector3& point)
{
	Vector3 grid_pos = (point - grid.position) / grid.unit_size;
	
	grid_pos.x = floor(grid_pos.x);
	grid_pos.y = floor(grid_pos.y);
	grid_pos.z = floor(grid_pos.z);

	return grid_pos;
}

void replace(vector<Vector3>& aretes, int index_to_replace, int index_replacer)
{
	cerr << "points[" << index_replacer << "] replace points[" << index_to_replace << "]\n";

	for (Vector3& v : aretes)
	{
		if (v.x == index_to_replace) 
			v.x = index_replacer;
		
		if (v.y == index_to_replace)
			v.y = index_replacer;
		
		if (v.z == index_to_replace)
			v.z = index_replacer;
	}
}

void simplification(vector<Vector3>& points, vector<Vector3>& aretes, const Grid3& grid)
{
	vector<bool> traiter(points.size());

	for (size_t i = 0 ; i < points.size() ; ++i)
	{
		Vector3 grid_pos_i = grid_position(grid, points[i]);
		
		cerr << "point[" << i << "] = " << grid_pos_i << "\n";

		for (size_t j = i + 1 ; j < points.size() ; ++j)
		{
			Vector3 grid_pos_j = grid_position(grid, points[j]);
			
			if (!traiter[i] && grid_pos_i == grid_pos_j)
			{
				traiter[i] = true;
				replace(aretes, j, i);
			}
		}
	}
}

Grid3 mesh_grid;

void initScene() {

	string format;

	cin >> format;
	cin >> nb_sommets >> nb_triangles >> nb_aretes;

	points.resize(nb_sommets);

	for (int i = 0 ; i < nb_sommets ; ++i)
	{
		cin >> points[i];

		if (min_pos.x > points[i].x)
			min_pos.x = points[i].x;
		if (min_pos.y > points[i].y)
			min_pos.y = points[i].y;
		if (min_pos.z > points[i].z)
			min_pos.z = points[i].z;

		if (max_pos.x < points[i].x)
			max_pos.x = points[i].x;
		if (max_pos.y < points[i].y)
			max_pos.y = points[i].y;
		if (max_pos.z < points[i].z)
			max_pos.z = points[i].z;
	}

	int nb_cotes;

	aretes.resize(nb_triangles);

	for (int i = 0 ; i < nb_triangles ; ++i)
	{
		cin >> nb_cotes;
		cin >> aretes[i];
	}

	// for (const Vector3& a : aretes)
	// {
	// 	cerr << a << "\n";
	// }

	// for (const Vector3& a : aretes)
	// {
	// 	cerr << a << "\n";
	// }
	
	// glEnableClientState(GL_VERTEX_ARRAY);
	// glEnableClientState(GL_NORMAL_ARRAY);

	// glVertexPointer(3, GL_FLOAT, 0, points);
	// glNormalPointer(GL_FLOAT, 0, normals);
}

void renderScene() {

	// GESTION DE LA CAMERA

	glOrtho(-gl_ortho_size, gl_ortho_size, -gl_ortho_size, gl_ortho_size, gl_ortho_size, -gl_ortho_size);
	glRotatef(vert_angle, 1, 0, 0);
	glRotatef(horiz_angle, 0, 1, 0);

	// GESTION DES LUMIÈRES

	glPointSize(5);

	float radius = ORTHO * (gl_ortho_size);

	light0Pos[0] = sin(glutGet(GLUT_ELAPSED_TIME)/1000.0) * radius;
	light0Pos[1] = 1 * radius / 5;
	light0Pos[2] = cos(glutGet(GLUT_ELAPSED_TIME)/1000.0) * radius;

	glBegin(GL_POINTS);
		glVertex3fv(light0Pos);
	glEnd();
	
	draw_vect({0, 0, 0}, Vector3::one.normalized() * 15);

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

	mesh_grid.position = min_pos;
	mesh_grid.dimension = max_pos - min_pos;
	mesh_grid.unit_size = grid_unit_size;

	// vector<Vector3> save = aretes;

	simplification(points, aretes, mesh_grid);

	glBegin(GL_TRIANGLES);

	glColor3d(0, 1, 0);

	for (Vector3 index : aretes)
	{
		Vector3 triangleNorm = normal(points[index.x], points[index.y], points[index.z]).normalized();
		glNormal3d(triangleNorm.x, triangleNorm.y, triangleNorm.z);

		glVertex3dv((double*)points[index.x]);
		glVertex3dv((double*)points[index.y]);
		glVertex3dv((double*)points[index.z]);
	}

	glEnd();

	// aretes = save;

	glColor3d(1, 0, 0);

	if (toggle_t)
		mesh_grid.draw();

	/*
	for (Vector3 index : aretes)
	{	
		Vector3 triangleNorm = normal(points[index.x], points[index.y], points[index.z]).normalized();
		
		Point3 triangle_center = (Point3)((points[index.x] + points[index.y] + points[index.z]) / 3);
		
		draw_vect(triangle_center, triangleNorm);
	}
	*/
}
