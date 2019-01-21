#include "Point3.h"
#include "Vector3.h"

Point3::Point3() : x(), y(), z() {}
Point3::Point3(double x, double y, double z) : x(x), y(y), z(z)  {}


Point3 Point3::project_on_line(Point3 start, Point3 end){
	return project_on_line(end - start, start);
}

Point3 Point3::project_on_line(Vector3 line_vector, Point3 start){
	line_vector.normalize();

	Vector3 start_to_me = *this - start;

	return start + line_vector * start_to_me.norme(); 
}

Point3 Point3::project_on_plane(Point3 plane_point, Vector3 plane_normal){
	double plane_to_me = (*this - plane_point).scalar(plane_normal) / plane_normal.norme();
	return *this - plane_normal * plane_to_me;
}

Point3::operator Vector3() {
	return Vector3(x, y, z);
}