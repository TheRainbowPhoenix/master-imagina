#ifndef POINT3_H
#define POINT3_H

#include <iostream>
#include <GL/glut.h> 

class Vector3;

class Point3 {
public:
	double x, y, z;

	Point3();
	Point3(double x, double y = 0, double z = 0);

	Point3 project_on_line(Point3 line_start, Point3 line_end);
	Point3 project_on_line(Point3 origin, Vector3 direction);
	Point3 project_on_plane(Point3 on_plane, Vector3 normal_of_plane);

	explicit operator Vector3() const;
	operator GLdouble*() const;
};

Vector3 operator-(const Point3& left, const Point3& right);

Point3 operator+(const Point3& p, const Vector3& v);
Point3& operator+=(Point3& p, const Vector3& v);

Point3 operator-(const Point3& p, const Vector3& v);
Point3& operator-=(Point3& p, const Vector3& v);

bool operator==(const Point3& left, const Point3& right);
bool operator!=(const Point3& left, const Point3& right);

std::ostream& operator<<(std::ostream& os, const Point3& p);
std::istream& operator>>(std::istream& is, Point3& p);

#endif