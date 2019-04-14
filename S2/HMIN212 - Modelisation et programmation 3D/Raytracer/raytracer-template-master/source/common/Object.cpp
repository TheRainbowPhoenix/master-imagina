//////////////////////////////////////////////////////////////////////////////
//
//  --- Object.cpp ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////


#include "common.h"



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Sphere::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;

  //TODO: Ray-sphere setup
    
  double radius = length(mesh.center - mesh.box_max) * 0.5;
  // std::cerr << "Radius : " << radius << "\n";
  
  result.t_o = raySphereIntersection(p0_w, V_w, mesh.center, radius);
  result.name = this->name;

  return result;
}

/* -------------------------------------------------------------------------- */
/* ------ Ray = p0 + t*V  sphere at origin O and radius r    : Find t ------- */
double Sphere::raySphereIntersection(vec4 p0, vec4 V, vec4 O, double r){
  double t  = std::numeric_limits< double >::infinity();
  
  //TODO: Ray-sphere intersection;
  
  vec4 toP0 = (p0 - O);

  double a = dot(V, V);
  double b = 2 * dot(toP0, V);
  double c = dot(toP0, toP0) - (r * r);

  double discriminant = (b * b) - 4 * a * c;

  if (discriminant >= 0)
    t = (-b - sqrt(discriminant)) / 2.0 * a;

  return t;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Square::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;
  
  //TODO: Ray-square setup
  
  return result;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
double Square::raySquareIntersection(vec4 p0, vec4 V){
  double t   = std::numeric_limits< double >::infinity();
  
  //TODO: Ray-square intersection;
  


  return t;
}
