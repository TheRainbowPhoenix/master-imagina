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
      
  result.t_o = raySphereIntersection(INVC * p0_w, INVC * V_w);
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

  if (discriminant > 0)
    t = (-b - sqrt(discriminant)) / 2.0 * a;

  return t;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Square::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;
  
  //TODO: Ray-square setup
  // result.N_w = TRAINC * result.N_o;
  // result.N_w = TRAINC * result.N_o;
  return result;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
double Square::raySquareIntersection(vec4 p0, vec4 V){
  double t   = std::numeric_limits< double >::infinity();
  
  //TODO: Ray-square intersection;

  vec4 a = mesh.vertices[5]; // up left
  vec4 b = mesh.vertices[1]; // up right
  vec4 c = mesh.vertices[2]; // down right
  vec4 d = mesh.vertices[0]; // down left

  plane_normal = normalize(cross(a - b, a - c)); 

  vec4 toA = a - p0;

  vec4 proj = dot(toA, V);

  return t;
}
