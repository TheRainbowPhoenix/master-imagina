#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <array>
#include <limits>
#include <algorithm>

#include <GL/glut.h>

#include "Vector3.h"
#include "Point3.h"
#include "courbe.h"
#include <map>

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
bool toggle_p = false;
size_t uo_value = 0;
size_t jl_value = 0;

void normalKeyHandler(unsigned char key, int, int) 
{
	switch (key) {
		case KEY_ESC:
			exit(EXIT_SUCCESS);
			break;
		case 't':
			toggle_t = !toggle_t;
			break;
		case 'u':
			uo_value = uo_value > 0 ? uo_value - 1 : uo_value;
			break;
		case 'o':
			uo_value++;
			break;
		case 'j':
			jl_value = jl_value > 0 ? jl_value - 1 : jl_value;
			break;
		case 'l':
			jl_value++;
			break;
		case 'p':
			toggle_p = true;
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
	
	// cout << "mouse position : (" << x << ", " << y << ")\n";
	
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

	glCullFace(GL_BACK);
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

int nb_sommets = 0;
int nb_triangles = 0;
int nb_aretes = 0;

vector<Vector3> points;
vector<Vector3> aretes;
vector<Vector3> subdivision;

Vector3 min_pos(numeric_limits<double>::max(), numeric_limits<double>::max(), numeric_limits<double>::max());
Vector3 max_pos(numeric_limits<double>::min(), numeric_limits<double>::min(), numeric_limits<double>::min());

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

// renvoie (sous forme d'index) le triangle i tel que pi_pj est une arete du triangle pi_pj_pk
int find_triangle(const vector<Vector3>& aretes, int pi, int pj, int start = 0)
{
	if (start < 0)
		cerr << "find_triangle : negative start\n";
	
	for (size_t i = start; i < aretes.size(); ++i)
	{
		int match_count = 0;

		if (aretes[i].x == pi || aretes[i].x == pj)
			match_count++;

		if (aretes[i].y == pi || aretes[i].y == pj)
			match_count++;

		if (aretes[i].z == pi || aretes[i].z == pj)
			match_count++;

		if (match_count == 2)
			return i;
	}

	return -1;
}

// renvoie (sous forme d'index) le point pk tel que pi_pj est une arete du triangle pi_pj_pk
int summit(const Vector3& triangle, int pi, int pj)
{
	return 
	triangle.x != pi && triangle.x != pj ? triangle.x : 
	triangle.y != pi && triangle.y != pj ? triangle.y : 
	triangle.z != pi && triangle.z != pj ? triangle.z : -1;
}

Vector3 subdivide_butterfly_point(vector<Vector3>& points, vector<Vector3>& aretes, int pi, int pj)
{
	int summits[8] = { -1 };
	int triangles[6] = { -1 };

	summits[0] = pi;
	summits[1] = pj;

	triangles[0] = find_triangle(aretes, summits[0], summits[1]);
	summits[2] = summit(aretes[triangles[0]], summits[0], summits[1]);
	
	// 

	triangles[1] = find_triangle(aretes, summits[0], summits[1], triangles[0] + 1);
	summits[3] = summit(aretes[triangles[1]], summits[0], summits[1]);

	//

	triangles[2] = find_triangle(aretes, summits[0], summits[2]);
	
	if (triangles[2] == triangles[0])
		triangles[2] = find_triangle(aretes, summits[0], summits[2], triangles[2] + 1);
	
	summits[4] = summit(aretes[triangles[2]], summits[0], summits[2]);

	//
	
	triangles[3] = find_triangle(aretes, summits[1], summits[2]);
	
	if (triangles[3] == triangles[0])
		triangles[3] = find_triangle(aretes, summits[1], summits[2], triangles[3] + 1);
	
	summits[5] = summit(aretes[triangles[3]], summits[1], summits[2]);

	//
	
	triangles[4] = find_triangle(aretes, summits[0], summits[3]);
	
	if (triangles[4] == triangles[1])
		triangles[4] = find_triangle(aretes, summits[0], summits[3], triangles[4] + 1);
	
	summits[6] = summit(aretes[triangles[4]], summits[0], summits[3]);

	//
	
	triangles[5] = find_triangle(aretes, summits[1], summits[3]);
	
	if (triangles[5] == triangles[1])
		triangles[5] = find_triangle(aretes, summits[1], summits[3], triangles[5] + 1);
	
	summits[7] = summit(aretes[triangles[5]], summits[1], summits[3]);

	cerr << "butterfly " << pi << ", " << pj << " :\n";
	
	cerr << "\ttriangles = { ";
	
	for (int i = 0; i < 6; ++i)
		cerr << triangles[i] << " ";

	cerr << "}\n";

	cerr << "\tsummits = { ";
	
	for (int i = 0; i < 8; ++i)
		cerr << summits[i] << " ";

	cerr << "}\n";

	// ajout du nouveau point

	return (points[summits[0]] + points[summits[1]]) / 2 + (points[summits[2]] + points[summits[3]]) / 8 - (points[summits[4]] + points[summits[5]] + points[summits[6]] + points[summits[7]]) / 16;
}

struct Edge
{
	int x, y;

	Edge() : x(), y() {}
	Edge(int x, int y) : x(x), y(y) {}

	bool operator==(const Edge& other) const
	{
		return (this->x == other.x && this->y == other.y) || (this->x == other.y && this->y == other.x);
	}

	bool operator!=(const Edge& other) const
	{
		return !(*this == other);
	}

	bool operator<(const Edge& other) const
	{
		return (this->x * this->x + this->y * this->y) < (other.x * other.x + other.y * other.y);
	}
};

int select_triangle(const array<int, 2>& edge_triangles, int unwanted_triangle)
{
	for (int triangle : edge_triangles)
	{
		if (triangle != unwanted_triangle)
		{
			if (triangle == -1)
			{
				cerr << "(select_triangle) negative match\n";
			}

			return triangle;
		}
	}

	cerr << "(select_triangle) no match\n";
	return -1;
}

Vector3 subdivide_butterfly_point_v2(vector<Vector3>& points, vector<Vector3>& aretes, map<Edge, array<int, 2> >& edge_to_triangles_map, Edge edge)
{
	int summits[8] = { -1 };
	int triangles[6] = { -1 };
	array<int, 2> edge_triangles;

	summits[0] = edge.x;
	summits[1] = edge.y;

	edge_triangles = edge_to_triangles_map[edge];

	triangles[0] = edge_triangles[0];
	summits[2] = summit(aretes[triangles[0]], summits[0], summits[1]);

	triangles[1] = edge_triangles[1];
	summits[3] = summit(aretes[triangles[1]], summits[0], summits[1]);

	//
	
	edge_triangles = edge_to_triangles_map[Edge(summits[0], summits[2])];

	triangles[2] = select_triangle(edge_triangles, triangles[0]);
	summits[4] = summit(aretes[triangles[2]], summits[0], summits[2]);

	//
	
	edge_triangles = edge_to_triangles_map[Edge(summits[1], summits[2])];

	triangles[3] = select_triangle(edge_triangles, triangles[0]);	
	summits[5] = summit(aretes[triangles[3]], summits[1], summits[2]);

	//
	
	edge_triangles = edge_to_triangles_map[Edge(summits[0], summits[3])];

	triangles[4] = select_triangle(edge_triangles, triangles[1]);
	summits[6] = summit(aretes[triangles[4]], summits[0], summits[3]);

	//
	
	edge_triangles = edge_to_triangles_map[Edge(summits[1], summits[3])];

	triangles[5] = select_triangle(edge_triangles, triangles[1]);
	summits[7] = summit(aretes[triangles[5]], summits[1], summits[3]);

/*
	cerr << "butterfly " << summits[0] << ", " << summits[1] << " :\n";
	
	cerr << "\ttriangles = { ";
	
	for (int i = 0; i < 6; ++i)
	{
		cerr << triangles[i] << " ";

		if (summits[i] == -1)
		{
			cerr << "triangle = -1\n";
			exit(EXIT_FAILURE);
		}
	}

	cerr << "}\n";

	cerr << "\tsummits = { ";
	
	for (int i = 0; i < 8; ++i)
	{
		cerr << summits[i] << " ";

		if (summits[i] == -1)
		{
			cerr << "summits = -1\n";
			exit(EXIT_FAILURE);
		}
	}

	cerr << "}\n";
*/
	// ajout du nouveau point

	return (points[summits[0]] + points[summits[1]]) / 2 + (points[summits[2]] + points[summits[3]]) / 8 - (points[summits[4]] + points[summits[5]] + points[summits[6]] + points[summits[7]]) / 16;
}

void subdivide_butterfly(vector<Vector3>& points, vector<Vector3>& aretes, vector<Vector3>& subdivision, Vector3 triangle)
{
	// Generation de points

	size_t b0_index = points.size();
	size_t b1_index = points.size() + 1;
	size_t b2_index = points.size() + 2;

	Vector3 b0 = subdivide_butterfly_point(points, aretes, triangle.x, triangle.y);
	Vector3 b1 = subdivide_butterfly_point(points, aretes, triangle.y, triangle.z);
	Vector3 b2 = subdivide_butterfly_point(points, aretes, triangle.z, triangle.x);

	points.push_back(b0);
	points.push_back(b1);
	points.push_back(b2);

	// Generation de triangles

	subdivision.push_back(Vector3(b2_index, triangle.x, b0_index));
	subdivision.push_back(Vector3(b0_index, triangle.y, b1_index));
	subdivision.push_back(Vector3(b1_index, triangle.z, b2_index));
	
	subdivision.push_back(Vector3(b2_index, b0_index, b1_index));
}

void subdivide_butterfly_all(vector<Vector3>& points, vector<Vector3>& aretes, vector<Vector3>& subdivision)
{
	for (Vector3 triangle : aretes)
		subdivide_butterfly(points, aretes, subdivision, triangle);
}

void subdivide_butterfly_all_v2(vector<Vector3>& points, vector<Vector3>& aretes, vector<Vector3>& subdivision)
{
	cerr << "points : " << points.size() << ", aretes : " << aretes.size() << "\n";
	map<Edge, size_t> edge_to_point_map;
	map<Edge, array<int, 2> > edge_to_triangles_map;

	for (size_t i = 0 ; i < aretes.size() ; ++i)
	{
		Edge triangle_edges[3] = { Edge(aretes[i].x, aretes[i].y), Edge(aretes[i].y, aretes[i].z), Edge(aretes[i].z, aretes[i].x) };

		for (Edge edge : triangle_edges)
		{
			if (edge_to_triangles_map.count(edge) == 0)
				edge_to_triangles_map[edge] = { (int)i, -1 };
			else
				edge_to_triangles_map[edge][1] = (int)i;
		}
	}

	for (Vector3 triangle : aretes)
	{
		Edge triangle_edges[3] = { Edge(triangle.x, triangle.y), Edge(triangle.y, triangle.z), Edge(triangle.z, triangle.x) };

		for (Edge edge : triangle_edges)
		{
			if (edge_to_point_map.count(edge) == 0)
			{
				edge_to_point_map[edge] = points.size();
				points.push_back(subdivide_butterfly_point_v2(points, aretes, edge_to_triangles_map, edge));
			}
		}
	}

	for (Vector3 triangle : aretes)
	{
		Vector3 sub_triangle(edge_to_point_map[Edge(triangle.x, triangle.y)], edge_to_point_map[Edge(triangle.y, triangle.z)], edge_to_point_map[Edge(triangle.z, triangle.x)]);

		subdivision.push_back(sub_triangle);
		subdivision.push_back(Vector3(sub_triangle.z, triangle.x, sub_triangle.x));
		subdivision.push_back(Vector3(sub_triangle.x, triangle.y, sub_triangle.y));
		subdivision.push_back(Vector3(sub_triangle.y, triangle.z, sub_triangle.z));
	}

	/*
	vector<size_t> points_count(points.size());
	
	for (Vector3 triangle : aretes)
	{
		points_count[triangle.x]++;
		points_count[triangle.y]++;
		points_count[triangle.z]++;
	}

	cerr << "\npoints_count = { ";
	
	for (size_t i = 0 ; i < points_count.size() ; ++i)
		cerr << points_count[i] << " ";

	cerr << "}\n";
	*/
}

void subdivide(vector<Vector3>& points, vector<Vector3>& aretes, int p0, int p1, int p2)
{
	// nouveaux points
	
	size_t p0_p1_index = points.size();
	size_t p1_p2_index = points.size() + 1;
	size_t p2_p0_index = points.size() + 2;

	Vector3 p0_p1 = (points[p0] + points[p1]) / 2;
	Vector3 p1_p2 = (points[p1] + points[p2]) / 2;
	Vector3 p2_p0 = (points[p2] + points[p0]) / 2;

	points.push_back(p0_p1);
	points.push_back(p1_p2);
	points.push_back(p2_p0);

	// nouveaux triangles

	Vector3 triangles[4] =
	{
		{ (double)p2_p0_index, (double)p0, (double)p0_p1_index },
		{ (double)p0_p1_index, (double)p1, (double)p1_p2_index },
		{ (double)p1_p2_index, (double)p2, (double)p2_p0_index },
		{ (double)p0_p1_index, (double)p1_p2_index, (double)p2_p0_index }
	};

	for (Vector3 triangle : triangles)
		aretes.push_back(triangle);
}

void subdivide_all(vector<Vector3>& points, vector<Vector3>& aretes)
{
	size_t nb_triangles = aretes.size();

	for (size_t i = 0 ; i < nb_triangles ; ++i)
	{
		subdivide(points, aretes, aretes[i].x, aretes[i].y, aretes[i].z);
	}
}

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

	// subdivide_all(points, aretes);

	// subdivide_butterfly_all(points, aretes, subdivision);
	
	// subdivide_butterfly(points, aretes, 0, 1);
	// subdivide_butterfly(points, aretes, 1, 2);
	// subdivide_butterfly(points, aretes, 2, 3);

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
	
	draw_vector_components({0, 0, 0}, Vector3::one.normalized() * 15);

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
	
	// vector<Vector3> save = aretes;

	if (toggle_p)
	{
		// for (size_t i = 0; i < jl_value; ++i)
		subdivide_butterfly_all_v2(points, aretes, subdivision);
		aretes.clear();
		aretes = subdivision;
		subdivision.clear();
		toggle_p = false;
	}

	vector<Vector3> save = aretes;
	
	for (size_t i = 0; i < uo_value; ++i)
		subdivide_all(points, aretes);

	// for (Vector3 point : points)
		// draw_point((Point3)point);

	double normal_factor = 1;

	draw_mesh(points, aretes, normal_factor);

	aretes = save;

	glColor3d(1, 0, 0);


	if (toggle_t)
		draw_mesh_normals(points, aretes, normal_factor);
}
