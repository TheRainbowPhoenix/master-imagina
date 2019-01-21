#include <cmath>
#include "Vector3.h"
#include "Point3.h"

Vector3::Vector3() : x(), y(), z() {}
Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

double Vector3::norme() const {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void Vector3::normalize() {
	double n = norme();
	x /= n;
	y /= n;
	z /= n;
}

double Vector3::scalar(Vector3& v) const {
	return x * v.x + y * v.y + z + v.y;
}

Vector3 Vector3::vectoriel(Vector3& v) const {
	return Vector3(y * v.z - z * v.y,
			       z * v.x - x * v.z,
			       x * v.y - y * v.x);
}

double Vector3::angle(Vector3& v) const {
	return acos(scalar(v)/(norme()*v.norme()));
}

Vector3::operator Point3() const {
	return Point3(x, y, z);
}

Vector3 Vector3::zero(0, 0, 0);
Vector3 Vector3::one(1, 1, 1);

////////////////////////////////////////////////// OPERATEURS DEFINITION

Vector3 operator-(const Vector3& right){
	return Vector3(-right.x, -right.y, -right.z);
}

Vector3& operator+=(Vector3& left, const Vector3& right){
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;
	return left;
}

Vector3& operator-=(Vector3& left, const Vector3& right){
	left += -right;
	return left;
}

Vector3 operator+(const Vector3& left, const Vector3& right){
	return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3 operator-(const Vector3& left, const Vector3& right){
	return (left + -right);
}

Vector3 operator*(const Vector3& left, double right){
	return Vector3(left.x * right, left.y * right, left.z * right);
}

Vector3 operator*(double left, const Vector3& right){
	return (right * left);
}

Vector3& operator*=(Vector3& left, double right){
	left.x *= right;
	left.y *= right;
	left.z *= right;
	return left;
}

Vector3 operator/(const Vector3& left, double right){
	return Vector3(left.x / right, left.y / right, left.z / right);
}

Vector3& operator/=(Vector3& left, double right){
	left.x /= right;
	left.y /= right;
	left.z /= right;
	return left;
}

bool operator==(const Vector3& left, const Vector3& right){
	return left.x == right.x && left.y == right.y && left.z == right.z;
}

bool operator!=(const Vector3& left, const Vector3& right){
	return !(left == right);
}

std::ostream& operator<<(std::ostream& os, const Vector3& vector){
	os << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";
	return os;
}

std::istream& operator>>(std::istream& is, Vector3& vector){
  is >> vector.x >> vector.y >> vector.z;
  return is;
}
