#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

class Point3;

class Vector3 {
public:
	double x, y, z;

	Vector3();
	Vector3(double x, double y = 0, double z = 0);

	double norme() const;
	void normalize();
	double scalar(Vector3& v) const;
	Vector3 vectoriel(Vector3& v) const;
	double angle(Vector3& v) const;

	operator Point3() const;

	static Vector3 zero;
	static Vector3 one;
};

Vector3 operator-(const Vector3& right);

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

std::ostream& operator<<(std::ostream& os, const Vector3& vector);

std::istream& operator>>(std::istream& is, Vector3& vector);

#endif