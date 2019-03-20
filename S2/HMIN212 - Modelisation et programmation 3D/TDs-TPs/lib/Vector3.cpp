#include <cmath>
#include "Vector3.h"
#include "Point3.h"

Vector3 Vector3::zero    (0, 0, 0);
Vector3 Vector3::one     (1, 1, 1);

Vector3 Vector3::up      (0, 1, 0);
Vector3 Vector3::down    (0, -1, 0);
Vector3 Vector3::right   (1, 0, 0);
Vector3 Vector3::left    (-1, 0, 0);

Vector3 Vector3::forward (0, 0, 1);
Vector3 Vector3::backward(0, 0, -1);

Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

void Vector3::normalize() {
	*this /= length();
}

Vector3 Vector3::normalized() const {
	return *this / length();	
}

double Vector3::length() const {
	return pow(length_squared(), 0.5);
}

double Vector3::length_squared() const {
	return (x * x) + (y * y) + (z * z);
}

double Vector3::dot(Vector3& v) const {
	return (x * v.x) + (y * v.y) + (z + v.z);
}

Vector3 Vector3::cross(Vector3& v) const {
	return Vector3((y * v.z) - (z * v.y),
			       (z * v.x) - (x * v.z),
			       (x * v.y) - (y * v.x));
}

double Vector3::angle(Vector3& v) const {
	return acos(dot(v) / (length() * v.length()));
}

Vector3::operator Point3() const {
	return Point3(x, y, z);
}

Vector3::operator double*() {
	return &x;
}

double Vector3::dot(const Vector3& left, const Vector3& right){
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z); 
}

Vector3 Vector3::min(const Vector3& left, const Vector3& right){
	return left.length_squared() < right.length_squared() ? left : right;
}

Vector3 Vector3::max(const Vector3& left, const Vector3& right){
	return left.length_squared() > right.length_squared() ? left : right;
}
////////////////////////////////////////////////// OPERATEURS DEFINITION

Vector3 operator-(const Vector3& left){
	return Vector3(-left.x, -left.y, -left.z);
}

Vector3& operator+=(Vector3& left, const Vector3& right){
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;
	return left;
}

Vector3& operator-=(Vector3& left, const Vector3& right){
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;
	return left;
}

Vector3 operator+(const Vector3& left, const Vector3& right){
	return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3 operator-(const Vector3& left, const Vector3& right){
	return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
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
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

bool operator!=(const Vector3& left, const Vector3& right){
	return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}

std::ostream& operator<<(std::ostream& os, const Vector3& v){
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Vector3& v){
  is >> v.x >> v.y >> v.z;
  return is;
}
