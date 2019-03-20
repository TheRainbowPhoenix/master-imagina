#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Point3.h"
#include "Vector3.h"

struct Triangle {

	// sens anti horraire
	Point3 a, b, c;

	Triangle(const Point3& a, const Point3& b, const Point3& c);

	double angle(const Triangle& other) const;
	
	Vector3 normal() const;

};

#endif 
