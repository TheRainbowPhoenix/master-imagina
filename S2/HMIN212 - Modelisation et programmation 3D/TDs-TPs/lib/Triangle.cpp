#include <cmath>
#include "Triangle.h"

Triangle::Triangle(const Point3& a, const Point3& b, const Point3& c) : a(a), b(b), c(c) {}

double Triangle::angle(const Triangle& other) const {
	Vector3 v1 = this->normal();
	Vector3 v2 = other.normal();
	return M_PI - v1.angle(v2);
}

Vector3 Triangle::normal() const {
  Vector3 v1 = b - a;
  Vector3 v2 = c - a;
  return v1.cross(v2);
}