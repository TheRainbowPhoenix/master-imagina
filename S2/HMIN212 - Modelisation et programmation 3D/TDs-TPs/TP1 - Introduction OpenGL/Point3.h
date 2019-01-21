#ifndef POINT3_H
#define POINT3_H

class Vector3;

class Point3 {
public:
	double x, y, z;

	Point3();
	Point3(double x, double y = 0, double z = 0);

	Point3 project_on_line(Point3 line_start, Point3 line_end);
	Point3 project_on_line(Vector3 line_v, Point3 line_p);
	Point3 project_on_plane(Point3 on_plane, Vector3 normal_of_plane);

	operator Vector3();
};

#endif