//////////////////////////////////////////////////////////////////////////////
//
//  --- Angel.h ---
//
//   The main header file for all examples from Angel 6th Edition
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANGEL_H__
#define __ANGEL_H__

//----------------------------------------------------------------------------
// 
// --- Include system headers ---
//

#include <cmath>
#include <iostream>
#include <cstdio>
#include <stdlib.h>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

//----------------------------------------------------------------------------
//
//  --- Include our class libraries and constants ---
//

/* -------------------------------------------------------------------------- */
/* ----------------------- Nearly Equal Test--------------------------------- */
static inline bool nearlyEqual(double a, double b, double epsilon){
  const double absA = fabs(a);
  const double absB = fabs(b);
  const double diff = fabs(a - b);
  
  if (a == b) { // shortcut
    return true;
  } else if (a * b == 0) { // a or b or both are zero
                           // relative error is not meaningful here
    return diff < (epsilon * epsilon);
  } else { // use relative error
    return diff / (absA + absB) < epsilon;
  }
}


namespace Angel {

//  Helper function to load vertex and fragment shader files
//GLuint InitShader( const char* vertexShaderFile,
//		   const char* fragmentShaderFile );

//  Defined constant for when numbers are too small to be used in the
//    denominator of a division operation.  This is only used if the
//    DEBUG macro is defined.
const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);

//  Degrees-to-radians constant 
const GLfloat  DegreesToRadians = M_PI / 180.0;  
const GLfloat  RadiansToDegrees = 180.0/M_PI;

}  // namespace Angel

#include "vec.h"
#include "mat.h"

static void __gluMultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4],
                                GLdouble out[4])
{
  int i;
  
  for (i=0; i<4; i++) {
    out[i] =
    in[0] * matrix[0*4+i] +
    in[1] * matrix[1*4+i] +
    in[2] * matrix[2*4+i] +
    in[3] * matrix[3*4+i];
  }
}

/*
 ** Invert 4x4 matrix.
 ** Contributed by David Moore (See Mesa bug #6748)
 */
static int __gluInvertMatrixd(const GLdouble m[16], GLdouble invOut[16])
{
  double inv[16], det;
  int i;
  
  inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
  + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
  inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
  - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
  inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
  + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
  inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
  - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
  inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
  - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
  inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
  + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
  inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
  - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
  inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
  + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
  inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
  + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
  inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
  - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
  inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
  + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
  inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
  - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
  inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
  - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
  inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
  + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
  inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
  - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
  inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
  + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];
  
  det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
  if (det == 0)
    return GL_FALSE;
  
  det = 1.0 / det;
  
  for (i = 0; i < 16; i++)
    invOut[i] = inv[i] * det;
  
  return GL_TRUE;
}

static void __gluMultMatricesd(const GLdouble a[16], const GLdouble b[16],
                               GLdouble r[16])
{
  int i, j;
  
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      r[i*4+j] =
      a[i*4+0]*b[0*4+j] +
      a[i*4+1]*b[1*4+j] +
      a[i*4+2]*b[2*4+j] +
      a[i*4+3]*b[3*4+j];
    }
  }
}

GLint
gluProject(GLdouble objx, GLdouble objy, GLdouble objz,
           const GLdouble modelMatrix[16],
           const GLdouble projMatrix[16],
           const GLint viewport[4],
           GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
  double in[4];
  double out[4];
  
  in[0]=objx;
  in[1]=objy;
  in[2]=objz;
  in[3]=1.0;
  __gluMultMatrixVecd(modelMatrix, in, out);
  __gluMultMatrixVecd(projMatrix, out, in);
  if (in[3] == 0.0) return(GL_FALSE);
  in[0] /= in[3];
  in[1] /= in[3];
  in[2] /= in[3];
  /* Map x, y and z to range 0-1 */
  in[0] = in[0] * 0.5 + 0.5;
  in[1] = in[1] * 0.5 + 0.5;
  in[2] = in[2] * 0.5 + 0.5;
  
  /* Map x,y to viewport */
  in[0] = in[0] * viewport[2] + viewport[0];
  in[1] = in[1] * viewport[3] + viewport[1];
  
  *winx=in[0];
  *winy=in[1];
  *winz=in[2];
  return(GL_TRUE);
}

GLint
_gluUnProject(GLdouble winx, GLdouble winy, GLdouble winz,
             const GLdouble modelMatrix[16],
             const GLdouble projMatrix[16],
             const GLint viewport[4],
             GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
  double finalMatrix[16];
  double in[4];
  double out[4];
  
  __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
  if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);
  
  in[0]=winx;
  in[1]=winy;
  in[2]=winz;
  in[3]=1.0;
  
  /* Map x and y from window coordinates */
  in[0] = (in[0] - viewport[0]) / viewport[2];
  in[1] = (in[1] - viewport[1]) / viewport[3];
  
  /* Map to range -1 to 1 */
  in[0] = in[0] * 2 - 1;
  in[1] = in[1] * 2 - 1;
  in[2] = in[2] * 2 - 1;
  
  __gluMultMatrixVecd(finalMatrix, in, out);
  if (out[3] == 0.0) return(GL_FALSE);
  out[0] /= out[3];
  out[1] /= out[3];
  out[2] /= out[3];
  *objx = out[0];
  *objy = out[1];
  *objz = out[2];
  return(GL_TRUE);
}

static char*
readShaderSource(const char* shaderFile)
{
  FILE* fp = fopen(shaderFile, "r");
  
  if ( fp == NULL ) { return NULL; }
  
  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);
  
  fseek(fp, 0L, SEEK_SET);
  char* buf = new char[size + 1];
  fread(buf, 1, size, fp);
  
  buf[size] = '\0';
  fclose(fp);
  
  return buf;
}

static void check_shader_compilation(std::string shader_file, GLuint shader){
  GLint  compiled;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
  if ( !compiled ) {
    std::cerr << shader_file.c_str() << " failed to compile:" << std::endl;
    GLint  logSize;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
    char* logMsg = new char[logSize];
    glGetShaderInfoLog( shader, logSize, NULL, logMsg );
    std::cerr << logMsg << std::endl;
    delete [] logMsg;
    
    exit( EXIT_FAILURE );
  }
}

static void check_program_link(GLuint program){
  
  GLint  linked;
  glGetProgramiv( program, GL_LINK_STATUS, &linked );
  if ( !linked ) {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint  logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
    char* logMsg = new char[logSize];
    glGetProgramInfoLog( program, logSize, NULL, logMsg );
    std::cerr << logMsg << std::endl;
    delete [] logMsg;
    
    exit( EXIT_FAILURE );
  }
  
}


#endif // __ANGEL_H__
