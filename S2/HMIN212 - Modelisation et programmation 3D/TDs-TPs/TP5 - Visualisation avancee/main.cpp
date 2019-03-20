///////////////////////////////////////////////////////////////////////////////
// Imagina
// ----------------------------------------------------------------------------
// IN - Synthèse d'images - Modélisation géométrique
// Auteur : Gilles Gesquière
// ----------------------------------------------------------------------------
// Base du TP 1
// programme permettant de créer des formes de bases.
// La forme représentée ici est un polygone blanc dessiné sur un fond rouge
///////////////////////////////////////////////////////////////////////////////  

#include <iostream>
#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include <unistd.h>



/* Dans les salles de TP, vous avez généralement accès aux glut dans C:\Dev. Si ce n'est pas le cas, téléchargez les .h .lib ...
Vous pouvez ensuite y faire référence en spécifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>. 
Si vous mettez glut dans le répertoire courant, on aura alors #include "glut.h" 
*/

#include <GL/glut.h> 
#include "Vector3.h"
#include "Point3.h"
#include "courbe.h"
#include "tga.h"

using namespace std;

// Définition de la taille de la fenêtre
#define WIDTH  600

#define HEIGHT 600

// Définition de la couleur de la fenêtre
#define RED   0.8
#define GREEN 0.8
#define BLUE  0.8
#define ALPHA 1

// Touche echap (Esc) permet de sortir du programme
#define KEY_ESC 27

#define KEY_I 105
#define KEY_J 108
#define KEY_K 107
#define KEY_L 106

#define KEY_UP 101
#define KEY_LEFT 100 
#define KEY_DOWN 103
#define KEY_RIGHT 102

#define KEY_TAB 9

#define KEY_U 117
#define KEY_O 111

#define KEY_H 104
#define KEY_M 109

#define KEY_SPACE 32

#define KEY_CTRL 114
#define KEY_ENTR 13

#define KEY_P 112
#define KEY_A 97
#define KEY_E 101

#define KEY_Z 122
#define KEY_Q 113
#define KEY_S 115
#define KEY_D 100

#define KEY_R 114
#define KEY_F 102

// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 
GLvoid window_special_key(int key, int x, int y); 
void load_house_tex(GLuint* textures);

double random(double n, long long precision = 100000);
double random(double min, double max, long long precision = 100000);

const int house_textures_num = 3;
GLuint house_textures[house_textures_num] = {0};

int main(int argc, char **argv) { 
  srand(time(0));
  // initialisation  des paramètres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);

  // définition et création de la fenêtre graphique, ainsi que son titre
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Premier exemple : carré");

  // initialisation de OpenGL et de la scène
  initGL();  
  init_scene();

  // choix des procédures de callback pour 
  // le tracé graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fenêtre
  glutReshapeFunc(&window_reshape);
  // la gestion des événements clavier
  glutKeyboardFunc(&window_key);
  // gestion des événement spécial
  glutSpecialFunc(&window_special_key);

  // la boucle prinicipale de gestion des événements utilisateur
  glutMainLoop();  

  return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque
GLvoid initGL() {
  glClearColor(RED, GREEN, BLUE, ALPHA);  
  glClearDepth(1.0f);                   // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
  glShadeModel(GL_SMOOTH);   // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

// Initialisation de la scene. Peut servir à stocker des variables de votre programme
// à initialiser
void init_scene() {
  load_house_tex(house_textures);
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // C'est l'endroit où l'on peut dessiner. On peut aussi faire appel
  // à une fonction (render_scene() ici) qui contient les informations 
  // que l'on veut dessiner
  render_scene();

  // trace la scène grapnique qui vient juste d'être définie
  glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height) {

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
  // possible de changer la taille de l'objet dans la fenêtre. Augmentez ces valeurs si l'objet est 
  // de trop grosse taille par rapport à la fenêtre.
  glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);

  // toutes les transformations suivantes s´appliquent au modèle de vue 
  glMatrixMode(GL_MODELVIEW);
}

Point3 point(-2 + random(4), -2 + random(4));

vector<vector<Point3> > curve_controls = {
  { Point3(0, 2), Point3(-2, -2) }
};

size_t current_control_point = 0;
size_t current_curve_control = 0;

long precision_courbe = 0;
long precision_surface = 20;

bool print_grid = true;
bool select_all = true;

double move_speed = 0.1;

long axe_x = 0;
long axe_y = 0;
long axe_z = 0;

double camera_distance = 2;
double camera_angle = 0;

Point3 camera_pos(0, 0, 0);
Point3 camera_target(0, 0, 0);
Point3 camera_direction(0, 1, 0);

double camera_y = -0.3;

void toggle(bool& b) {
  b = !b;
}

// Selectionne le prochain indice et reviens à zero avant que la valeur max ne soit atteinte
void select_next_loop(size_t max_size, size_t& current) {
  current = current >= max_size ? 0 : current + 1;
}


void increase_loop(long& value, long min, long max) {
  value = value >= max ? min : value + 1;
}

void  decrease_loop(long& value, long min, long max) {
  value = value <= min ? max : value - 1;
}

// Diminue/augmente la valeur jusqu'a un certain cap

void decrease(long& value, long min = 0) {
  value = value > min ? value - 1 : min;
}

void increase(long& value, long max = 1000000) {
  value = value < max ? value + 1 : max;
}

void select_next_control_point(const vector<Point3>& control_points, size_t& current){
  current = current >= control_points.size() ? 0 : current + 1;
}

void select_next_curve_control(const vector<vector<Point3> >& curve_controls, size_t& current){
  current = current >= curve_controls.size() ? 0 : current + 1;
}

double random(double n, long long precision){
  long long num = n * precision;
  return (rand() % num) / (double)precision;
}

double random(double min, double max, long long precision){
  return min + random((max - min), precision);
}

void print_v(const vector<Point3>& v){
  for (size_t i = 0; i < v.size(); ++i){
    cout << v[i] << " ";
  }
  cout << "\n";
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y) {

  Vector3 direction;

  switch (key) {
  case KEY_ESC:
    exit(1);
    break;
  // Select loop
  case KEY_TAB:
    select_next_loop(curve_controls[current_curve_control].size(), current_control_point);
    break;
  case KEY_ENTR:
    select_next_loop(curve_controls.size(), current_curve_control);
    break;
  case KEY_P:
    toggle(print_grid);
    break;
  // Increase/Decrease
  case KEY_U:
    decrease(precision_courbe);
    break;
  case KEY_O:
    increase(precision_courbe);
    break;
  case KEY_H:
    decrease(precision_surface);
    break;
  case KEY_M:
    increase(precision_surface);
    break;
  // Direction
  case KEY_I:
    direction = Vector3::up;
    break;
  case KEY_J:
    direction = Vector3::right;
    break;
  case KEY_K:
    direction = Vector3::down;
    break;
  case KEY_L:
    direction = Vector3::left;
    break;
  case KEY_R:
    direction = Vector3::forward;
    break;
  case KEY_F:
    direction = Vector3::backward;
    break;
  // Angle
  case KEY_Z:
    decrease_loop(axe_x, 0, 360);
    break;
  case KEY_S:
    increase_loop(axe_x, 0, 360);
    break;
  case KEY_Q:
    decrease_loop(axe_y, 0, 360);
    break;
  case KEY_D:
    increase_loop(axe_y, 0, 360);
    break;
  case KEY_A:
    decrease_loop(axe_z, 0, 360);
    break;
  case KEY_E:
    increase_loop(axe_z, 0, 360);
    break;
  // Default
  default:
    printf ("La touche %d n´est pas active.\n", key);
    break;
  }

  if (!select_all)
    curve_controls[current_curve_control][current_control_point] += direction * move_speed;
  else
    for (Point3& p : curve_controls[current_curve_control])
      p += direction * move_speed;

  glutPostRedisplay();
}

GLvoid window_special_key(int key, int x, int y){

  Vector3 direction;

  switch (key) {
  case KEY_ESC:  
    exit(1);                    
    break;
  // Direction
  case KEY_UP:
    camera_y+=0.1;
    direction = Vector3::up;
    break;
  case KEY_DOWN:
    direction = Vector3::down;
    camera_y-=0.1;
    break;
  case KEY_LEFT:
    direction = Vector3::left;
    break;
  case KEY_RIGHT:
    direction = Vector3::right;
    break;
  case KEY_CTRL:
    toggle(select_all);
    break;
  default:
    printf ("La touche %d n´est pas active.\n", key);
    break;
  }

  curve_controls[current_curve_control][current_control_point] += direction * move_speed;

  glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////


void draw_cubes(/*const vector<vector<Point3> >& cubes*/) {
  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  //for (auto& cube : cubes) {
    glVertex3d(0, 0, 0);
    glVertex3d(2, 0, 0);
    glVertex3d(2, 2, 0);
    glVertex3d(0, 2, 0);
  //}
  glEnd();
}

void draw_house(GLuint* textures) {

  glColor3f(0.1, 0.4, 0.1);
  glBegin(GL_QUADS);
    glVertex3d(8, 0, 10);
    glVertex3d(-8, 0, 10);
    glVertex3d(-8, 0, -10);
    glVertex3d(8, 0, -10);
  glEnd();
  glColor3f(1, 1, 1);
  
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // MUR PROFOND
  // glColor3f(0, 1, 0);
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 1); glVertex3d(-4, 5, 5);
    glMultiTexCoord2f(GL_TEXTURE0, 1, 1); glVertex3d(4, 5, 5);
    glMultiTexCoord2f(GL_TEXTURE0, 1, 0); glVertex3d(4, 0, 5);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 0); glVertex3d(-4, 0, 5);
  glEnd();

  // MUR DROIT
  // glColor3f(0, 0, 1);
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 1); glVertex3d(4, 5, 5);
    glMultiTexCoord2f(GL_TEXTURE0, 1, 1); glVertex3d(4, 5, -5);
    glMultiTexCoord2f(GL_TEXTURE0, 1, 0); glVertex3d(4, 0, -5);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 0); glVertex3d(4, 0, 5);
  glEnd();

  // MUR DEVANT
  //glColor3f(1, 0, 0);
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 1); glVertex3d(4, 5, -5);
    glMultiTexCoord2f(GL_TEXTURE0, 1, 1); glVertex3d(-4, 5, -5);
    glMultiTexCoord2f(GL_TEXTURE0, 1, 0); glVertex3d(-4, 0, -5);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 0); glVertex3d(4, 0, -5);
  glEnd();

  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // MUR GAUCHE
  // glColor3f(0, 0, 1);
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE1, 0, 0); glVertex3d(-4, 5, -5);
    glMultiTexCoord2f(GL_TEXTURE1, 1, 0); glVertex3d(-4, 5, 5);
    glMultiTexCoord2f(GL_TEXTURE1, 1, 1); glVertex3d(-4, 0, 5);
    glMultiTexCoord2f(GL_TEXTURE1, 0, 1); glVertex3d(-4, 0, -5);
  glEnd();

  glActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // TRIANGLE PROFOND
  // glColor3f(0, 1, 0);
  glBegin(GL_TRIANGLES);
    glMultiTexCoord2f(GL_TEXTURE2, 0.5, 0.5); glVertex3d(0, 8, 5);
    glMultiTexCoord2f(GL_TEXTURE2, 1, 0);     glVertex3d(4, 5, 5);
    glMultiTexCoord2f(GL_TEXTURE2, 0, 0);     glVertex3d(-4, 5, 5);
  glEnd();
  
  // TOIT DROIT
  // glColor3f(0, 1, 0);
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE2, 0, 1); glVertex3d(0, 8, 5);
    glMultiTexCoord2f(GL_TEXTURE2, 1, 1); glVertex3d(0, 8, -5);
    glMultiTexCoord2f(GL_TEXTURE2, 1, 0); glVertex3d(4, 5, -5);
    glMultiTexCoord2f(GL_TEXTURE2, 0, 0); glVertex3d(4, 5, 5);
  glEnd();
  
  // TOIT GAUCHE
  // glColor3f(0, 1, 0);
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE2, 0, 1); glVertex3d(0, 8, -5);
    glMultiTexCoord2f(GL_TEXTURE2, 1, 1); glVertex3d(0, 8, 5);
    glMultiTexCoord2f(GL_TEXTURE2, 1, 0); glVertex3d(-4, 5, 5);
    glMultiTexCoord2f(GL_TEXTURE2, 0, 0); glVertex3d(-4, 5, -5);
  glEnd();

  // TRIANGLE DEVANT
  // glColor3f(1, 0, 0);
  glBegin(GL_TRIANGLES);
    glMultiTexCoord2f(GL_TEXTURE2, 0.5, 0.5); glVertex3d(0, 8, -5);
    glMultiTexCoord2f(GL_TEXTURE2, 1, 0);     glVertex3d(-4, 5, -5);
    glMultiTexCoord2f(GL_TEXTURE2, 0, 0);     glVertex3d(4, 5, -5);
  glEnd();

  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);
}

void load_house_tex(GLuint* textures) {

  int width = 0;
  int height = 0;

  glGenTextures(3, textures);

  BYTE* mur_img = load_tga(const_cast<char*>("textures/mur.tga"), &width, &height);
  
  if (mur_img) {
    printf("Texture 1 chargé !\n");
    
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, mur_img);

    delete[] mur_img;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  BYTE* porte_img = load_tga(const_cast<char*>("textures/facade.tga"), &width, &height);

  if (porte_img) {
    printf("Texture 2 chargé !\n");
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, porte_img);
    
    delete[] porte_img;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  BYTE* toit_img = load_tga(const_cast<char*>("textures/toit.tga"), &width, &height);

  if (toit_img) {
    printf("Texture 3 chargé !\n");
    
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, toit_img);

    delete[] toit_img;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }
}

Vector3 normal(const Point3& A, const Point3& B, const Point3& C) {
  Vector3 v1 = B - A;
  Vector3 v2 = C - A;
  return v1.cross(v2);
}

void render_scene() {

  // Gestion de la camera
   
  //camera_angle  = 0.1 * precision_courbe;

  //Point3 camera_pos = Point3() +  Vector3(sin(camera_angle), 0, cos(camera_distance)) * camera_distance;
  camera_pos = Point3() + Vector3(sin(glutGet(GLUT_ELAPSED_TIME)/1000.0), camera_y, cos(glutGet(GLUT_ELAPSED_TIME)/1000.0)) * camera_distance;

  gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, camera_target.x, camera_target.y, camera_target.z, camera_direction.x, camera_direction.y, camera_direction.z);
  
  usleep(100000);
    
  glEnable(GL_CULL_FACE);

  draw_house(house_textures);

  /*
  Point3 line_start(-2, 2);
  Point3 line_end(2, -2);

  Point3 line_start_2(2, 2);
  Point3 line_end_2(-2, -2);

  Point3 projection = point.project_on_line(line_start, line_end);

  draw_point(point);
  draw_line(point, projection);

  glColor3f(1, 0, 0);
 
  draw_point(projection);
  draw_line(line_start, line_end);
  */

  
/*
  vector<Point3> courbe_hermite = hermite_cubic_curve(Point3(0, 0), Point3(2, 0), Vector3(1, 1), Vector3(1, -1), 100);
  draw_curve(courbe_hermite);
*/


  //vector<Point3> courbe_bezier_bernstein;
  //courbe_bezier_bernstein = bezier_curve_bernstein(control_points, precision_courbe); 
  //draw_curve(courbe_bezier_bernstein);

/*
  vector<Point3> courbe_bezier_casteljau;
  courbe_bezier_casteljau = bezier_curve_casteljau(curve_controls[0], precision_courbe); 
  draw_curve(courbe_bezier_casteljau);

  vector<Point3> courbe_bezier_casteljau2;
  courbe_bezier_casteljau2 = bezier_curve_casteljau(curve_controls[1], precision_courbe); 
  draw_curve(courbe_bezier_casteljau2);

  Point3 line_start(2, 0);
  Point3 line_end(4, 2);


  draw_surface_cylindrique(courbe_bezier_casteljau, line_start, line_end, precision_surface);
  //draw_surface_reglee(courbe_bezier_casteljau, courbe_bezier_casteljau2, precision_surface);

  vector<vector<Point3> > surface;
  surface = bezier_surface_bernstein(curve_controls, precision_courbe, precision_surface);

  if (print_grid)
    draw_surface_grid(surface);
  else
    draw_surface(surface);
*/


  //double degree = 45 * (M_PI / 180.0);

  //rotate_around_y(x, degree);
  //rotate_around_y(y, degree);
  //rotate_around_y(z, degree);


  Point3 start = curve_controls[0][1];
  Point3 end = curve_controls[0][0];

  //glColor3f(0, 0, 1);
  
  //vector<vector<Point3> > facettes;
  //facettes = facettes_cylindre(start, end, precision_courbe, precision_surface);
  //facettes = facettes_cone(start, end, precision_courbe, precision_surface);
  //facettes = facettes_sphere(end, 2, precision_courbe, precision_courbe);
  //draw_cubes();
/*
  if (print_grid)
    draw_facettes_grid(facettes);
  else
    draw_facettes(facettes);
*/
  // INTERFACE DE CONTROLE
  
  //glColor3f(1, 1, 0);
  
  Point3 center(0, 0, 0);

  draw_vect(center, Vector3::one * 3);

  //draw_line(start, end);
  //draw_point(start);
  //draw_point(end);
  
  //double angle_x = axe_x * (M_PI / 180.0);
  //double angle_y = axe_y * (M_PI / 180.0);
  //double angle_z = axe_z * (M_PI / 180.0);

/*  Point3 p = start;

  Vector3 x = Vector3::right * intensity;
  Vector3 y = Vector3::up * intensity;
  Vector3 z = Vector3::forward * intensity;
  
  Vector3 rot;
  rot += rotate_around_x((x + y + z), angle_x);
  rot += rotate_around_y((x + y + z), angle_y);
  rot += rotate_around_z((x + y + z), angle_z);

  glColor3f(1, 0, 0);
  draw_line(p, p + x);
  glColor3f(0, 1, 0);
  draw_line(p, p + y);
  glColor3f(0, 0, 1);
  draw_line(p, p + z);
  draw_line(p, p + rot);
*/
  //std::cout << "angle_x : " << axe_x << ", angle_y: " << axe_y << ", angle_z: " << axe_z << "\n";
  
  /*
  for (auto& e : curve_controls)
    draw_curve(e);

  for (size_t k = 0; k < curve_controls.size() - 1; ++k) {
    draw_line(curve_controls[k][0], curve_controls[k+1][0]);
    draw_line(curve_controls[k][curve_controls[k].size()-1], curve_controls[k+1][curve_controls[k+1].size()-1]);
  }
  
  glColor3f(1, 1, 0);
  
  if (select_all) {
    draw_curve(curve_controls[current_curve_control]);
  } else {
    draw_point(curve_controls[current_curve_control][current_control_point]);
  }
  */
 
  glutPostRedisplay();
}
