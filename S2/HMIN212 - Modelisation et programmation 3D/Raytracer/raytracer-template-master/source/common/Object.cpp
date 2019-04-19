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
  

  result.t_o = raySquareIntersection(INVC * p0_w, INVC * V_w);
  result.name = this->name;
/*
  result.ID_ = -1;
  vec4 pointO = this->INVC * p0_w;
  vec4 VO = this->INVC * V_w;
  bool inverse = false;
  double t = raySquareIntersection(pointO, VO);
  
  if (t > EPSILON)
  { //TODO: Ray-square setup 
  	
  	//pour eviter les clippings et positif si ca touche
  	vec4 pointW = p0_w + t*V_w;
	vec4 pointReflectO = this->INVC * pointW; //il faut verifier si c est dans le carre et pas en dehors
	
	if(pointReflectO.x>=mesh.vertices[0].x && pointReflectO.x<=mesh.vertices[1].x && pointReflectO.y>=mesh.vertices[0].y && pointReflectO.y<=mesh.vertices[1].y)
	{
		vec4 normale = mesh.normals[0];
		
		if(inverse)
		{
			normale = -mesh.normals[0];
		}

		result.P_o = pointReflectO;
		result.P_w = pointW; //normales ?
		result.N_o = normale;
		result.N_w = this->C;
    	result.t_w = t; //t wormld et t object ???
    	result.t_o = t;
    	result.name = this->name;
    	result.ID_ = 0;
    }
  }
*/
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

  vec4 plane_normal = normalize(cross(a - b, a - c)); 
  vec4 line_dir = normalize(V);

  vec4 toA = a - p0;

  vec4 proj = dot(toA, V);

  float denom = dot(plane_normal, line_dir);

  if (denom > 1e-6)
  {
    vec4 p0_to_a = a - p0;
    t = dot(p0_to_a, plane_normal) / denom;

    if (t > 1e-6)
    {
      vec4 proj_on_plane = p0 + line_dir * t;
      
      if (proj_on_plane.x < 1 && proj_on_plane.x > -1 && proj_on_plane.y < 1 && proj_on_plane.y > -1)
      {
        return t;
      }
    }

    return INFINITY; // plane intersection quand t >= 0
  }

  return INFINITY;
}
