#include "Point3.h"
#include "Vector3.h"

Point3::Point3() : x(0), y(0), z(0) {}
Point3::Point3(double x, double y, double z) : x(x), y(y), z(z)  {}

Point3 Point3::project_on_line(Point3 start, Point3 end){
	return project_on_line(start, end - start);
}

Point3 Point3::project_on_line(Point3 origin, Vector3 direction){
	Vector3 start_to_me = *this - origin;
	double dist_to_line = start_to_me.dot(direction) / direction.length();
	return origin + direction.normalized() * dist_to_line; 
}

Point3 Point3::project_on_plane(Point3 plane_point, Vector3 plane_normal){
	double plane_dist = (*this - plane_point).dot(plane_normal) / plane_normal.length();
	return *this - plane_normal.normalized() * plane_dist;
}

Vector3 operator-(const Point3& left, const Point3& right) {
	return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
}

Point3 operator+(const Point3& p, const Vector3& v){
	return Point3(p.x + v.x, p.y + v.y, p.z + v.z);
}

Point3& operator+=(Point3& p, const Vector3& v){
	p.x += v.x;
	p.y += v.y;
	p.z += v.z;
	return p;
}

Point3 operator-(const Point3& p, const Vector3& v){
	return Point3(p.x - v.x, p.y - v.y, p.z - v.z);
}

Point3& operator-=(Point3& p, const Vector3& v){
	p.x -= v.x;
	p.y -= v.y;
	p.z -= v.z;
	return p;
}

Point3::operator Vector3() const{
	return Vector3(x, y, z);
}

bool operator==(const Point3& left, const Point3& right){
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

bool operator!=(const Point3& left, const Point3& right){
	return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}

std::ostream& operator<<(std::ostream& os, const Point3& p){
	os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Point3& p){
  is >> p.x >> p.y >> p.z;
  return is;
}