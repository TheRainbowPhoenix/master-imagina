#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <GL/glut.h> 

class Point3;

class Vector3 {
public:
	double x, y, z;

	static Vector3 zero;
	static Vector3 one;

	static Vector3 up;
	static Vector3 down;
	static Vector3 left;
	static Vector3 right;
	
	static Vector3 forward;
	static Vector3 backward;

	Vector3();
	Vector3(double x, double y = 0, double z = 0);

	void normalize();
	Vector3 normalized() const;

	double length() const;
	double length_squared() const;

	double dot(Vector3& v) const;
	Vector3 cross(Vector3& v) const;

	double angle(Vector3& v) const;

	explicit operator Point3() const;
	operator GLdouble*() const;

	static double dot(const Vector3& left, const Vector3& right);
	static double cross(const Vector3& left, const Vector3& right);

	static double angle(const Vector3& from, const Vector3& to);
	static double distance(const Vector3& left, const Vector3& right);

	static Vector3 project(const Vector3& v, const Vector3& on_normal);
	static Vector3 project_on_plane(const Vector3& v, const Vector3& plane_normal);

	static Vector3 reflect(const Vector3& direction, const Vector3& normal);

	static Vector3 min(const Vector3& left, const Vector3& right);
	static Vector3 max(const Vector3& left, const Vector3& right);
};

Vector3 operator-(const Vector3& left);

Vector3& operator+=(Vector3& left, const Vector3& right);
Vector3& operator-=(Vector3& left, const Vector3& right);

Vector3 operator+(const Vector3& left, const Vector3& right);
Vector3 operator-(const Vector3& left, const Vector3& right);

Vector3 operator*(const Vector3& left, double right);
Vector3 operator*(double left, const Vector3& right);

Vector3& operator*=(Vector3& left, double right);

Vector3 operator/(const Vector3& left, double right);

Vector3& operator/=(Vector3& left, double right);

bool operator==(const Vector3& left, const Vector3& right);
bool operator!=(const Vector3& left, const Vector3& right);

std::ostream& operator<<(std::ostream& os, const Vector3& v);
std::istream& operator>>(std::istream& is, Vector3& v);

#endif