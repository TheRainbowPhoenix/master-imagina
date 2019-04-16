#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include "Vec3.h"
#include "Skeleton.h"
#include <algorithm>
#include <cmath>

#include <GL/glut.h>


// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

struct MeshVertex {
    inline MeshVertex () {
        w.clear();
    }
    inline MeshVertex (const Vec3 & _p, const Vec3 & _n) : p (_p), n (_n) {
        w.clear();
    }
    inline MeshVertex (const MeshVertex & vertex) : p (vertex.p), n (vertex.n) , w(vertex.w) {
    }
    inline virtual ~MeshVertex () {}
    inline MeshVertex & operator = (const MeshVertex & vertex) {
        p = vertex.p;
        n = vertex.n;
        w = vertex.w;
        return (*this);
    }
    // membres :
    Vec3 p; // une position
    Vec3 n; // une normale
    std::vector< double > w; // skinning weights
};

struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};

Vec3 project_on_line(Vec3 point, Vec3 origin, Vec3 direction)
{
    Vec3 start_to_me = point - origin;
    float dist_to_line = Vec3::dot(start_to_me, direction) / direction.length();
    
    direction.normalize();
    direction *= dist_to_line;

    return origin + direction;
}

class Mesh {
public:
    std::vector<MeshVertex> V;
    std::vector<MeshTriangle> T;

    void loadOFF (const std::string & filename);
    void recomputeNormals ();


    void computeSkinningWeights( Skeleton const & skeleton ) {
        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :

        // Indications:
        // you should compute weights for each vertex w.r.t. the skeleton bones
        // so each vertex will have B weights (B = number of bones)
        // these weights shoud be stored in vertex.w:

        for (MeshVertex& vertex : V)
        {
            double weight_sum = 0;
            // vertex.w.resize(skeleton.bones.size());

            for (unsigned int i = 0 ; i < skeleton.bones.size() ; ++i)
            {
                Vec3 P0 = skeleton.articulations[skeleton.bones[i].joints[0]].p;
                Vec3 P1 = skeleton.articulations[skeleton.bones[i].joints[1]].p;

                Vec3 Pi = vertex.p;

                float distFromP0 = (Pi - P0).length();
                float distFromP1 = (Pi - P1).length(); 

                float length = 0;

                Vec3 bone_origin = P0;
                Vec3 bone_dir = P0 - P1;

                float dot1 = Vec3::dot(Pi - P0, P0 - P1);
                float dot2 = Vec3::dot(Pi - P1, P1 - P0);
                
                if (dot1 > 0 && dot2 > 0)
                {
                    Vec3 proj = project_on_line(Pi, bone_origin, bone_dir);
                    length = (Pi - proj).length();
                }
                else
                {
                    length = std::min(distFromP1, distFromP0);
                }

                float weight = 1 / (length * length * length);

                weight_sum += weight;
                
                // vertex.w[i] = weight;
                vertex.w.push_back(weight);
            }

            // std::cerr << "bones : " << skeleton.bones.size() << "\n";

            for (double& weight : vertex.w)
            {
                weight /= weight_sum;
                // std::cerr << "weight : " << weight << "\n";
            }
            
            // exit(1);
        }


        //---------------------------------------------------//
        //---------------------------------------------------//
        //---------------------------------------------------//
    }

    void draw() const {
        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
        glEnd ();
    }

    // 1 / d²

    void drawTransformedMesh( SkeletonTransformation const & transfo ) const {
        std::vector< Vec3 > newPositions( V.size() );

        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :
         
        
        for( unsigned int i = 0 ; i < V.size() ; ++i )
        {
            Vec3 p = V[i].p;

            for (unsigned int j = 0 ; j < V[i].w.size() ; ++j)
            {
                Mat3 rotation = transfo.boneTransformations[j].worldSpaceRotation;
                Vec3 translation = transfo.boneTransformations[j].worldSpaceTranslation;

                p += V[i].w[j] * (rotation * V[i].p + translation);
            }

            newPositions[i] = p;
        }

        //---------------------------------------------------//
        //---------------------------------------------------//
        //---------------------------------------------------//

        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                Vec3 p = newPositions[ T[i].v[j] ];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (p[0], p[1], p[2]);
            }
        glEnd ();
    }
};



#endif
