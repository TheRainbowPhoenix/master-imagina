/*
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
/*
 * trackball.h
 * A virtual trackball implementation
 * Written by Gavin Bell for Silicon Graphics, November 1988.
 */

/*
 * Pass the x and y coordinates of the last and current positions of
 * the mouse, scaled so they are from (-1.0 ... 1.0).
 *
 * The resulting rotation is returned as a quaternion rotation in the
 * first paramater.
 */

#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__

#include "common.h"

class TrackBall{
public:
  
  static void trackball(float q[4], float p1x, float p1y, float p2x, float p2y);
  
  /*
   * Given two quaternions, add them together to get a third quaternion.
   * Adding quaternions to get a compound rotation is analagous to adding
   * translations to get a compound translation.  When incrementally
   * adding rotations, the first argument here should be the new
   * rotation, the second and third the total rotation (which will be
   * over-written with the resulting new total rotation).
   */
  static void add_quats(float *q1, float *q2, float *dest);
  
  /*
   * A useful function, builds a rotation matrix in Matrix based on
   * given quaternion.
   */
  static void build_rotmatrix(float m[4][4], float q[4]);
  
  /*
   * This function computes a quaternion based on an axis (defined by
   * the given vector) and an angle about which to rotate.  The angle is
   * expressed in radians.  The result is put into the third argument.
   */
  static void axis_to_quat(float a[3], float phi, float q[4]);
  
  static void quat_mult(float q1[4], float q2[4], float dest[4]);
  
  
  /*
   * Vector utility functions...
   */
  static void vzero(float *v);
  static void vset(float *v, float x, float y, float z);
  static void vsub(const float *src1, const float *src2, float *dst);
  static void vcopy(const float *v1, float *v2);
  static void vcross(const float *v1, const float *v2, float *cross);
  static float vlength(const float *v);
  static void vscale(float *v, float div);
  static void vnormal(float *v);
  static float vdot(const float *v1, const float *v2);
  static void vadd(const float *src1, const float *src2, float *dst);
  
  /* matrix functions */
  static void matinvert(float A[4][4]);
  static void matmult(float *in,float *out,float mat[4][4]);
  static void matident(float m[4][4]);
  static void matxmat(float out[4][4],float m0[4][4],float m1[4][4]);
  static void matcopy(float dst[4][4],float src[4][4]);
  static void matprint(float a[4][4],char *s);
  
  static float matadjoint(float r[4][4],float _data[4][4]);
  static void matinv4x4(float i[4][4]);
  static void mattrans(float i[4][4]);
  
};

#endif
