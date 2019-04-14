//////////////////////////////////////////////////////////////////////////////
//
//  --- ObjMesh.cpp ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////

#include "common.h"

bool Mesh::loadOBJ(const char * path){
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< vec3 > temp_vertices;
  std::vector< vec2 > temp_uvs;
  std::vector< vec3 > temp_normals;
  
  hasUV = true;
  
  FILE * file = fopen(path, "r");
  if( file == NULL ){
    printf("Impossible to open the file !\n");
    return false;
  }
  
  char *line = new char[128];
  char *lineHeader = new char[128];
  
  while(true){
    memset(line, 0 , 128);
    memset(lineHeader, 0 , 128);
    
    if(fgets(line, 128, file) == NULL){ break; }
    sscanf(line, "%s ", lineHeader);
    
    if ( strcmp( lineHeader, "v" ) == 0 ){
      vec3 vertex;
      sscanf(&line[2], "%f %f %f", &vertex.x, &vertex.y, &vertex.z );
      temp_vertices.push_back(vertex);
      if(vertex.x < box_min.x){box_min.x = vertex.x; }
      if(vertex.y < box_min.y){box_min.y = vertex.y; }
      if(vertex.z < box_min.z){box_min.z = vertex.z; }
      if(vertex.x > box_max.x){box_max.x = vertex.x; }
      if(vertex.y > box_max.y){box_max.y = vertex.y; }
      if(vertex.z > box_max.z){box_max.z = vertex.z; }
    }else if ( strcmp( lineHeader, "vt" ) == 0 ){
      vec2 uv;
      sscanf(&line[3], "%f %f", &uv.x, &uv.y );
      temp_uvs.push_back(uv);
    }else if ( strcmp( lineHeader, "vn" ) == 0 ){
      vec3 normal;
      sscanf(&line[3], "%f %f %f", &normal.x, &normal.y, &normal.z );
      temp_normals.push_back(normal);
    }else if ( strcmp( lineHeader, "f" ) == 0 ){
      std::string vertex1, vertex2, vertex3;
      int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = sscanf(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                           &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                           &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9){
        int matches = sscanf(&line[2], "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
                             &vertexIndex[1], &normalIndex[1],
                             &vertexIndex[2], &normalIndex[2]);
        if (matches == 6){
          hasUV = false;
        } else {
          printf("File can't be read by our simple parser : ( Try exporting with other options\n");
          return false;
        }
      }
      
      /* handle negative indices */
      /* (adjust for size during processing of each face, as per the old
       *  OBJ specification, instead of after the end of the file) */
      for (int negati = 0; negati < 3; negati++){
        if (vertexIndex[negati] < 0){
          vertexIndex[negati]+=temp_vertices.size();
          vertexIndex[negati]++; /* <- OBJ indices are one-based */
        }
        if (uvIndex[negati] < 0){
          uvIndex[negati]+=temp_uvs.size();
          uvIndex[negati]++;
        }
        if (normalIndex[negati] < 0){
          normalIndex[negati]+=temp_normals.size();
          normalIndex[negati]++;
        }
      }
      
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      if(hasUV){
        uvIndices    .push_back(uvIndex[0]);
        uvIndices    .push_back(uvIndex[1]);
        uvIndices    .push_back(uvIndex[2]);
      }
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
  }
  
  delete[] line;
  delete[] lineHeader;
  
  //    std::cout << "Read " << temp_vertices.size() << " vertices\n";
  //    std::cout << "Read " << temp_normals.size() << " normals\n";
  //    std::cout << "Read " << vertexIndices.size()/3 << " faces\n";
  //
  
  // For each vertex of each triangle
  for( unsigned int i=0; i<vertexIndices.size(); i++ ){
    unsigned int vertexIndex = vertexIndices[i];
    vec4 vertex = vec4(temp_vertices[ vertexIndex-1 ], 1.0);
    vertices.push_back(vertex);
  }
  
  if(hasUV){
    for( unsigned int i=0; i<uvIndices.size(); i++ ){
      unsigned int uvIndex = uvIndices[i];
      vec2 uv = temp_uvs[ uvIndex-1 ];
      uvs.push_back(uv);
    }
  }
  
  for( unsigned int i=0; i<normalIndices.size(); i++ ){
    unsigned int normalIndex = normalIndices[i];
    vec3 normal = temp_normals[ normalIndex-1 ];
    normals.push_back(normal);
  }
  
  //    std::cout << "Total " << vertices.size() << " vertices\n";
  //    std::cout << "Total " << normals.size() << " normals\n";
  
  
  center = box_min+(box_max-box_min)/2.0;
  scale = (std::max)(box_max.x - box_min.x, box_max.y-box_min.y);
  
  model_view = Scale(1.0/scale,           //Make the extents 0-1
                     1.0/scale,
                     1.0/scale)*
  Translate(-center);  //Orient Model About Center
  
  
  return true;
}

bool Mesh::makeSubdivisionSphere(int steps){
  
  box_min = vec3(-1,-1,-1);
  box_max = vec3(1,1,1);
  
  
  std::list < SphereTriangle > tris;
  
  tris.push_back(SphereTriangle(vec3(0.57735, 0.57735, 0.57735),    vec3(-0.57735, 0.57735, -0.57735), vec3(0.57735, 0.57735, -0.57735)));
  tris.push_back(SphereTriangle(vec3(-0.57735, 0.57735, -0.57735),  vec3(0.57735, 0.57735, 0.57735), vec3(-0.57735, 0.57735, 0.57735)));
  tris.push_back(SphereTriangle(vec3(0.57735, -0.57735, 0.57735),   vec3(-0.57735, -0.57735, -0.57735), vec3(0.57735, -0.57735, -0.57735)));
  tris.push_back(SphereTriangle(vec3(-0.57735, -0.57735, -0.57735), vec3(0.57735, -0.57735, 0.57735), vec3(-0.57735, -0.57735, 0.57735)));
  tris.push_back(SphereTriangle(vec3(0.57735, 0.57735, 0.57735),    vec3(0.57735, -0.57735, -0.57735), vec3(0.57735, 0.57735, -0.57735)));
  tris.push_back(SphereTriangle(vec3(0.57735, -0.57735, -0.57735),  vec3(0.57735, 0.57735, 0.57735), vec3(0.57735, -0.57735, 0.57735)));
  tris.push_back(SphereTriangle(vec3(-0.57735, 0.57735, 0.57735),   vec3(-0.57735, -0.57735, -0.57735), vec3(-0.57735, 0.57735, -0.57735)));
  tris.push_back(SphereTriangle(vec3(-0.57735, -0.57735, -0.57735), vec3(-0.57735, 0.57735, 0.57735), vec3(-0.57735, -0.57735, 0.57735)));
  tris.push_back(SphereTriangle(vec3(0.57735, 0.57735, 0.57735),    vec3(-0.57735, -0.57735, 0.57735), vec3(0.57735, -0.57735, 0.57735)));
  tris.push_back(SphereTriangle(vec3(-0.57735, -0.57735, 0.57735),  vec3(0.57735, 0.57735, 0.57735), vec3(-0.57735, 0.57735, 0.57735)));
  tris.push_back(SphereTriangle(vec3(0.57735, 0.57735, -0.57735),   vec3(-0.57735, -0.57735, -0.57735), vec3(0.57735, -0.57735, -0.57735)));
  tris.push_back(SphereTriangle(vec3(-0.57735, -0.57735, -0.57735), vec3(0.57735, 0.57735, -0.57735), vec3(-0.57735, 0.57735, -0.57735)));
  
  for(unsigned int s=0; s < steps; s++){
    std::list< SphereTriangle > newTris;
    float l = length(tris.begin()->a);
    for(std::list<SphereTriangle>::iterator i = tris.begin(); i != tris.end(); ++i) { // go through all triangles
      vec3 mid = (i->a + i->b) * 0.5f; // point betwenn points A and B
      mid = setLength(mid, l); // put in on the sphere
      newTris.push_back(SphereTriangle(i->b, i->c, mid)); // remember new triangles
      newTris.push_back(SphereTriangle(i->a, i->c, mid));
    }
    tris.swap(newTris); // use new set of triangles;
  }
  
  
  
  for(std::list<SphereTriangle>::iterator i = tris.begin(); i != tris.end(); ++i) {
    vertices.push_back(vec4(normalize(i->a), 1.0));
    vertices.push_back(vec4(normalize(i->b), 1.0));
    vertices.push_back(vec4(normalize(i->c), 1.0));
    
    normals.push_back(normalize(i->a));
    normals.push_back(normalize(i->b));
    normals.push_back(normalize(i->c));
  }
  
  return true;
}
