#ifndef SKELETON_H
#define SKELETON_H


#include <vector>
#include <queue>
#include <map>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include "Vec3.h"

#include <cmath>

#include <GL/glut.h>


// -------------------------------------------
// Basic Skeleton class
// -------------------------------------------

struct Articulation {
    // membres :
    Vec3 p; // une position

    int fatherBone; // there should be only 1
    std::vector< unsigned int > childBones;

    void setFatherBone( int f ) {
        if( fatherBone >= 0 ) {
            assert(fatherBone == f);
        }
        fatherBone = f;
    }
    bool isRoot() const {
        return fatherBone == -1;
    }

    Articulation() : fatherBone(-1) { childBones.clear(); }
};

struct Bone {
    // membres :
    unsigned int joints[2];

    int fatherBone; // there should be only 1
    std::vector< unsigned int > childBones;

    void setFatherBone( int f ) {
        if( fatherBone >= 0 ) {
            assert(fatherBone == f);
        }
        fatherBone = f;
    }
    bool isRoot() const {
        return fatherBone == -1;
    }

    Bone() : fatherBone(-1) { childBones.clear(); }
};

struct BoneTransformation{
    // membres :
    Mat3 localRotation;

    Mat3 worldSpaceRotation;
    Vec3 worldSpaceTranslation;

    BoneTransformation() : localRotation(Mat3::Identity()) ,  worldSpaceRotation(Mat3::Identity()) , worldSpaceTranslation(0,0,0) {}
};

struct SkeletonTransformation{
    // membres :
    std::vector< BoneTransformation > boneTransformations;
    std::vector< Vec3 > articulationsTransformedPosition;
};


struct Skeleton {
    // membres :
    std::vector< Articulation > articulations;
    std::vector< Bone > bones;
    std::vector< unsigned int > orderedBoneIndices; // process them by order in the hierarchy

    void buildStructure() {
        orderedBoneIndices.clear();
        std::vector< unsigned int > rootBones; // why not have several

        for( unsigned int b = 0 ; b < bones.size() ; ++b ) {
            Articulation & a0 = articulations[ bones[b].joints[0] ];
            Articulation & a1 = articulations[ bones[b].joints[1] ];
            a0.childBones.push_back( b );
            a1.setFatherBone( b );
        }

        for( unsigned int aIdx = 0 ; aIdx < articulations.size() ; ++aIdx ) {
            Articulation & a = articulations[ aIdx ];
            if( a.isRoot() ) {
                for( unsigned int bIt = 0 ; bIt < a.childBones.size() ; ++bIt ) {
                    unsigned int b = a.childBones[bIt];
                    rootBones.push_back( b );
                }
            }
            else {
                unsigned int bfIdx = a.fatherBone;
                Bone & bf = bones[bfIdx];
                for( unsigned int bIt = 0 ; bIt < a.childBones.size() ; ++bIt ) {
                    unsigned int bcIdx = a.childBones[bIt];
                    Bone & bc = bones[bcIdx];
                    bc.setFatherBone( bfIdx );
                    bf.childBones.push_back( bcIdx );
                }
            }
        }

        for( unsigned int rIt = 0 ; rIt < rootBones.size() ; ++rIt ) {
            unsigned int rootboneIdx = rootBones[rIt];
            std::queue< unsigned int > bonesIndices;
            bonesIndices.push(rootboneIdx);
            while( ! bonesIndices.empty()) {
                unsigned int bIdx = bonesIndices.front();
                bonesIndices.pop();
                orderedBoneIndices.push_back(bIdx);
                Bone & b = bones[bIdx];
                for( unsigned int bIt = 0 ; bIt < b.childBones.size() ; ++bIt ) {
                    unsigned int bcIdx = b.childBones[bIt];
                    bonesIndices.push(bcIdx);
                }
            }
        }

        assert( orderedBoneIndices.size() == bones.size() );
    }

    void load (const std::string & filename){
        std::ifstream in (filename.c_str ());
        if (!in)
            exit (EXIT_FAILURE);
        std::string tmpString;
        unsigned int sizeA;
        in >> tmpString >> sizeA;
        articulations.resize (sizeA);
        for (unsigned int i = 0; i < sizeA; i++)
            in >> articulations[i].p[0] >> articulations[i].p[1] >> articulations[i].p[2];

        unsigned int sizeB;
        in >> tmpString >> sizeB;
        bones.resize (sizeB);
        for (unsigned int i = 0; i < sizeB; i++) {
            for (unsigned int j = 0; j < 2; j++)
                in >> bones[i].joints[j];
        }
        in.close ();

        buildStructure();
    }






    void computeGlobalTransformationParameters( SkeletonTransformation & transfo ) {
        std::vector< Vec3 > & articulationsTransformedPosition = transfo.articulationsTransformedPosition;
        articulationsTransformedPosition.resize( articulations.size() );
        for( unsigned int bIt = 0 ; bIt < orderedBoneIndices.size() ; ++bIt ) {
            unsigned bIdx = orderedBoneIndices[bIt];
            Bone & b = bones[bIdx];

            if( b.isRoot() ) {
                Vec3 a0RestPos = articulations[ b.joints[0] ].p;
                Vec3 a0TargetPos = a0RestPos;
                articulationsTransformedPosition[ b.joints[0] ] = a0TargetPos;
                BoneTransformation & bTransfo = transfo.boneTransformations[bIdx];
                bTransfo.worldSpaceRotation = bTransfo.localRotation;

                // set the articulation as pivot point :
                bTransfo.worldSpaceTranslation = a0TargetPos - bTransfo.worldSpaceRotation * a0RestPos;

                // update the child articulation :
                Vec3 a1RestPos = articulations[ b.joints[1] ].p;
                Vec3 a1TargetPos = bTransfo.worldSpaceRotation * a1RestPos + bTransfo.worldSpaceTranslation;
                articulationsTransformedPosition[ b.joints[1] ] = a1TargetPos;
            }
            else{
                Vec3 a0RestPos = articulations[ b.joints[0] ].p;
                Vec3 a0TargetPos = articulationsTransformedPosition[ b.joints[0] ];

                BoneTransformation & bTransfo = transfo.boneTransformations[bIdx];
                BoneTransformation & bFatherTransfo = transfo.boneTransformations[b.fatherBone];
                bTransfo.worldSpaceRotation = bFatherTransfo.worldSpaceRotation * bTransfo.localRotation;

                // set the articulation as pivot point :
                bTransfo.worldSpaceTranslation = a0TargetPos - bTransfo.worldSpaceRotation * a0RestPos;

                // update the child articulation :
                Vec3 a1RestPos = articulations[ b.joints[1] ].p;
                Vec3 a1TargetPos = bTransfo.worldSpaceRotation * a1RestPos + bTransfo.worldSpaceTranslation;
                articulationsTransformedPosition[ b.joints[1] ] = a1TargetPos;
            }
        }
    }




    void computeProceduralAnim( double t , SkeletonTransformation & transfo ) {
        transfo.boneTransformations.resize( bones.size() );
        for( unsigned int bIt = 0 ; bIt < orderedBoneIndices.size() ; ++bIt ) {
            unsigned bIdx = orderedBoneIndices[bIt];
            Bone & b = bones[bIdx];
            if( b.isRoot() ) {
                BoneTransformation & bTransfo = transfo.boneTransformations[bIdx];
                bTransfo.localRotation = Mat3::Identity();
            }
            else{
                BoneTransformation & bTransfo = transfo.boneTransformations[bIdx];
                Vec3 axis( cos( 2 * M_PI * bIt / (double)(bones.size()) )  ,  sin( 2 * M_PI * bIt / (double)(bones.size()) )  , 0.0 );
                bTransfo.localRotation = Mat3::getRotationMatrixFromAxisAndAngle( axis , (0.25*M_PI) * cos( t ) );
            }
        }

        // update articulation positions:
        computeGlobalTransformationParameters(transfo);
    }




    //----------------------------------------------//
    //----------------------------------------------//
    //----------------------------------------------//
    // draw functions :
    //----------------------------------------------//
    //----------------------------------------------//
    //----------------------------------------------//
    void draw( int displayedBone ) const {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH);
        glDisable(GL_DEPTH_TEST);
        glLineWidth(3.0);
        glBegin (GL_LINES);
        for (unsigned int i = 0; i < bones.size (); i++) {
            glColor3f(1,0,0);
            {
                const Articulation & v = articulations[bones[i].joints[0]];
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
            glColor3f(1,1,1);
            {
                const Articulation & v = articulations[bones[i].joints[1]];
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
        }
        glEnd ();

        // we highlight the ordered bone number displayedBone
        if( displayedBone >= 0 && displayedBone < orderedBoneIndices.size() ) {
            displayedBone = orderedBoneIndices[displayedBone];
            glLineWidth(8.0);
            glBegin (GL_LINES);
            glColor3f(1,0,0);
            {
                const Articulation & v = articulations[bones[displayedBone].joints[0]];
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
            glColor3f(1,0,0);
            {
                const Articulation & v = articulations[bones[displayedBone].joints[1]];
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
            glEnd ();
        }

        // draw articulations:
        glPointSize(12.0);
        glBegin(GL_POINTS);
        glColor3f(0.5,0,0);
        for (unsigned int i = 0; i < articulations.size (); i++) {
                const Articulation & v = articulations[i];
                glVertex3f (v.p[0], v.p[1], v.p[2]);
        }
        glEnd();
        glEnable(GL_DEPTH);
        glEnable(GL_DEPTH_TEST);
    }
    void drawTransformedSkeleton( int displayedBone , SkeletonTransformation const & transfo ) const {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH);
        glDisable(GL_DEPTH_TEST);
        glLineWidth(3.0);
        glBegin (GL_LINES);
        for (unsigned int i = 0; i < bones.size (); i++) {
            glColor3f(1,0,0);
            {
                Vec3 p = transfo.articulationsTransformedPosition[ bones[i].joints[0] ];
                glVertex3f (p[0], p[1], p[2]);
            }
            glColor3f(1,1,1);
            {
                Vec3 p = transfo.articulationsTransformedPosition[ bones[i].joints[1] ];
                glVertex3f (p[0], p[1], p[2]);
            }
        }
        glEnd ();

        // we highlight the ordered bone number displayedBone
        if( displayedBone >= 0 && displayedBone < orderedBoneIndices.size() ) {
            displayedBone = orderedBoneIndices[displayedBone];
            glLineWidth(8.0);
            glBegin (GL_LINES);
            glColor3f(1,0,0);
            {
                Vec3 p = transfo.articulationsTransformedPosition[ bones[displayedBone].joints[0] ];
                glVertex3f (p[0], p[1], p[2]);
            }
            glColor3f(1,0,0);
            {
                Vec3 p = transfo.articulationsTransformedPosition[ bones[displayedBone].joints[1] ];
                glVertex3f (p[0], p[1], p[2]);
            }
            glEnd ();
        }

        // draw articulations:
        glPointSize(12.0);
        glBegin(GL_POINTS);
        glColor3f(0.5,0,0);
        for (unsigned int i = 0; i < articulations.size (); i++) {
            Vec3 p = transfo.articulationsTransformedPosition[ i ];
            glVertex3f (p[0], p[1], p[2]);
        }
        glEnd();
        glEnable(GL_DEPTH);
        glEnable(GL_DEPTH_TEST);
    }
};



#endif // SKELETON_H
