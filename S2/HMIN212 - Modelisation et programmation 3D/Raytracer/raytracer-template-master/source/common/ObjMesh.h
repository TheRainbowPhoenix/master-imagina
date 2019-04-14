//////////////////////////////////////////////////////////////////////////////
//
//  --- ObjMesh.h ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/


#ifndef __OBJMESH_H__
#define __OBJMESH_H__

#include "common.h"

using namespace Angel;

class Mesh{
public:
  bool hasUV;
  std::vector < vec4 > vertices;
  std::vector < vec2 > uvs;
  std::vector < vec3 > normals;
  
  vec3 box_min;
  vec3 box_max;
  vec3 center;
  float scale;
  
  mat4 model_view;
  
  Mesh(): box_min((std::numeric_limits< float >::max)(),
                  (std::numeric_limits< float >::max)(),
                  (std::numeric_limits< float >::max)() ),
  box_max(0,0,0),
  center(0,0,0),
  scale(1.0),
  model_view(){ }
  
  Mesh(const char * path)
  : box_min((std::numeric_limits< float >::max)(),
            (std::numeric_limits< float >::max)(),
            (std::numeric_limits< float >::max)() ),
  box_max(0,0,0),
  center(0,0,0),
  scale(1.0),
  model_view(){ loadOBJ(path); }
  
  unsigned int getNumTri(){ return vertices.size()/3; }
  
  bool loadOBJ(const char * path);
  
  class SphereTriangle{
  public:
    SphereTriangle(vec3 _p1, vec3 _p2, vec3 _p3): a(_p1), b(_p2), c(_p3){};
    vec3 a;
    vec3 b;
    vec3 c;
  };
  
  
  vec3 setLength(vec3 p1, float r){
    float rl = r/length(p1);
    return vec3(p1.x*rl, p1.y*rl, p1.z*rl);
  }
  
  bool makeSubdivisionSphere(int steps=8);
  
  bool makeParametricSphere(int steps=32){ return true; }
  
  friend std::ostream& operator << ( std::ostream& os, const Mesh& v ) {
    os << "Vertices:\n";
    for(unsigned int i=0; i < v.vertices.size(); i++){
      os << "\t\t" << v.vertices[i] << "\n";
    }
    os << "Normals:\n";
    for(unsigned int i=0; i < v.normals.size(); i++){
      os << "\t\t" << v.normals[i] << "\n";
    }
    
    return os;
  }
  
};


#endif  //#ifndef __OBJMESH_H__
