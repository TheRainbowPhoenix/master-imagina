#ifndef COURBE_H 
#define COURBE_H 

#include <GL/glut.h> 
#include <iostream>
#include <cmath>
#include <vector>
#include "Point3.h"
#include "Vector3.h"

// TP1

void draw_point(const Point3& point);

void draw_line(const Point3& start, const Point3& end);

// TP2

void draw_curve(const std::vector<Point3>& points);

std::vector<Point3> hermite_cubic_curve(Point3 P0, Point3 P1, Vector3 V0, Vector3 V1, long nb_u);

std::vector<Point3> bezier_curve_bernstein(const std::vector<Point3>& control_points, long nb_u);

std::vector<Point3> bezier_curve_casteljau(const std::vector<Point3>& control_points, long nb_u);

// TP3

void draw_surface_cylindrique(const std::vector<Point3>& courbe_bezier, const Point3& line_start, const Point3& line_end, long nb_u);

void draw_surface_reglee(const std::vector<Point3>& courbe_bezier1, const std::vector<Point3> courbe_bezier2, long nb_u);

std::vector<std::vector<Point3> > bezier_surface_bernstein(const std::vector<std::vector<Point3> >& control_grid, long nb_u, long nb_v); 

void draw_surface(const std::vector<std::vector<Point3> >& surface);

void draw_surface_grid(const std::vector<std::vector<Point3> >& surface);

// TP4

std::vector<std::vector<Point3> > facettes_cylindre(const Point3& line_start, const Point3& line_end, double rayon, size_t nb_meridiens);

void draw_facettes(const std::vector<std::vector<Point3> >& facettes);

void draw_facettes_grid(const std::vector<std::vector<Point3> >& facettes);

//Vector3& rotate_around_z(Vector3& v, double angle);
Vector3 rotate_around_z(const Vector3& v, double angle);

//Vector3& rotate_around_y(Vector3& v, double angle);
Vector3 rotate_around_y(const Vector3& v, double angle);

//Vector3& rotate_around_x(Vector3& v, double angle);
Vector3 rotate_around_x(const Vector3& v, double angle);

//Vector3& rotate_around(Vector3& v, const Vector3& axis, double angle);
Vector3 rotate_around(const Vector3& axis, double angle);

void draw_vect(const Point3& p, const Vector3& v);

std::vector<std::vector<Point3> > facettes_cone(const Point3& line_start, const Point3& line_end, double rayon, size_t nb_meridiens);

std::vector<std::vector<Point3> > facettes_sphere(const Point3& center, double radius, size_t meridiens, size_t paralleles);


// renvoie les facettes d'un polygones sous formes de triangles
std::vector<std::vector<Point3> > facettes_polygone(const Point3& center, double radius, size_t sides);

// TP7

Vector3 normal(const Vector3& A, const Vector3& B, const Vector3& C);

std::vector<Point3> maillage_cylindre(const Point3& start, const Point3& end, double radius, size_t meridiens);

void draw_maillage(std::vector<Point3> maillage);

std::vector<Point3> maillage_sphere(const Point3& center, double radius, size_t meridiens, size_t paralleles);

void draw_vector(const Point3& position, const Vector3& vector);

void draw_vector_components(const Point3& position, const Vector3& vector);

void draw_mesh(std::vector<Vector3>& points, std::vector<Vector3> aretes, double normal_factor = 1)
;
void draw_mesh_normals(std::vector<Vector3>& points, std::vector<Vector3> aretes, double normal_factor = 1);

#endif