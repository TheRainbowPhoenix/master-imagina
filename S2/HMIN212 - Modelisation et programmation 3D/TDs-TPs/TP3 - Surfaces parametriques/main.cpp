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


/* Dans les salles de TP, vous avez généralement accès aux glut dans C:\Dev. Si ce n'est pas le cas, téléchargez les .h .lib ...
Vous pouvez ensuite y faire référence en spécifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>. 
Si vous mettez glut dans le répertoire courant, on aura alors #include "glut.h" 
*/

#include <GL/glut.h> 
#include "Vector3.h"
#include "Point3.h"
#include "courbe.h"

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
#define KEY_J 106
#define KEY_K 107
#define KEY_L 108

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

// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 
GLvoid window_special_key(int key, int x, int y); 

double random(double n, long long precision = 100000);
double random(double min, double max, long long precision = 100000);

int main(int argc, char **argv)
{ 
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
GLvoid initGL() 
{
  glClearColor(RED, GREEN, BLUE, ALPHA);        
}

// Initialisation de la scene. Peut servir à stocker des variables de votre programme
// à initialiser
void init_scene()
{
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // C'est l'endroit où l'on peut dessiner. On peut aussi faire appel
  // à une fonction (render_scene() ici) qui contient les informations 
  // que l'on veut dessiner
  render_scene();

  // trace la scène grapnique qui vient juste d'être définie
  glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{  
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
  // possible de changer la taille de l'objet dans la fenêtre. Augmentez ces valeurs si l'objet est 
  // de trop grosse taille par rapport à la fenêtre.
  glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

  // toutes les transformations suivantes s´appliquent au modèle de vue 
  glMatrixMode(GL_MODELVIEW);
}

Point3 point(-2 + random(4), -2 + random(4));

vector<vector<Point3> > curve_controls = {
  { Point3(1, 1), Point3(1, 3), Point3(4, 3), Point3(4, 1) },
  { Point3(-4, -2), Point3(-4, 0), Point3(0, 0), Point3(0, -2) }
};

size_t current_control_point = 0;
size_t current_curve_control = 0;

long precision_courbe = 20;
long precision_surface = 20;

size_t select_next_control_point(const vector<Point3>& control_points, size_t current){
  return ++current >= control_points.size() ? 0 : current;
}

size_t select_next_curve_control(const vector<vector<Point3> >& curve_controls, size_t current){
  return ++current >= curve_controls.size() ? 0 : current;
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y) 
{  
  switch (key) {    
  case KEY_ESC:  
    exit(1);                    
    break;
  case KEY_TAB:
    current_control_point = select_next_control_point(curve_controls[current_curve_control], current_control_point);
    break;
  case KEY_U:
    precision_courbe =  --precision_courbe >= 0 ? precision_courbe : 0;  
    break;
  case KEY_O:
    precision_courbe++;
    break;
  case KEY_H:
    precision_surface =  --precision_surface >= 0 ? precision_surface : 0;  
    break;
  case KEY_M:
    precision_surface++;
    break;
  case KEY_I:
    curve_controls[current_curve_control][current_control_point] += Vector3::up * 0.1;
    break;
  case KEY_K:
    curve_controls[current_curve_control][current_control_point] += Vector3::down * 0.1;
    break;
  case KEY_J:
    curve_controls[current_curve_control][current_control_point] += Vector3::left * 0.1;
    break;
  case KEY_L:
    curve_controls[current_curve_control][current_control_point] += Vector3::right * 0.1;
    break;
  case KEY_SPACE:
    current_curve_control = select_next_curve_control(curve_controls, current_curve_control);
    break;
  default:
    printf ("La touche %d n´est pas active.\n", key);
    break;
  }
  glutPostRedisplay();
}

GLvoid window_special_key(int key, int x, int y){
  switch (key) {    
  case KEY_ESC:  
    exit(1);                    
    break;
  case KEY_UP:
    curve_controls[current_curve_control][current_control_point] += Vector3::up * 0.1;
    break;
  case KEY_DOWN:
    curve_controls[current_curve_control][current_control_point] += Vector3::down * 0.1;
    break;
  case KEY_LEFT:
    curve_controls[current_curve_control][current_control_point] += Vector3::left * 0.1;
    break;
  case KEY_RIGHT:
    curve_controls[current_curve_control][current_control_point] += Vector3::right * 0.1;
    break;
  default:
    printf ("La touche %d n´est pas active.\n", key);
    break;
  }
  glutPostRedisplay();
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

//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene() {

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


  glColor3f(1, 0, 0);
  draw_curve(curve_controls[0]);
  draw_curve(curve_controls[1]);

  glColor3f(0, 0, 1);
  
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


  //draw_surface_cylindrique(courbe_bezier_casteljau, line_start, line_end, precision_surface);
  draw_surface_reglee(courbe_bezier_casteljau, courbe_bezier_casteljau2, precision_surface);
*/
  
  vector<Point3> surface;
  surface = bezier_surface_bernstein(curve_controls, precision_courbe, precision_surface);

  draw_surface(surface);



  glColor3f(1, 1, 0);
  draw_point(curve_controls[current_curve_control][current_control_point]);

  //print_v(courbe_bezier_bernstein);
 // print_v(courbe_bezier_casteljau);
  cout << "fin\n";
}
