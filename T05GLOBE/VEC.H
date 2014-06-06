/* FILENAME: VEC.H
 * PROGRAMMER: MY6
 * PURPOSE: Vec declaration module.
 * LAST UPDATE: 06.06.2014
 */
#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include "def.h"

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))

typedef double DBL;
typedef float FLT;

typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

extern MATR UnitMatrix;

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;

  return v;
}
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet( V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z );
}
__inline VEC SubVec(VEC V1, VEC V2)
{
  return VecSet( V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z );
}
__inline VEC VecDivNum( VEC V, DBL N )
{ 
  return VecSet( V.X / N, V.Y / N, V.Z / N );
}
__inline VEC VecNeg( VEC V )
{
  return VecSet( -V.X, -V.Y, -V.Z );
}
__inline VEC VecCrossVec(VEC V1, VEC V2)
{
  return VecSet( V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X );
}
__inline VEC VecMulNum( VEC V, DBL N )
{
  return VecSet( V.X * N, V.Y * N, V.Z * N );
}
__inline VEC VecNormalize( VEC V )
{
  DBL len = V.X * V.X + V.Y + V.Y + V.Z * V.Z;
  
  if(len != 0 && len != 1)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
}

__inline DBL VecDotVec(VEC V1, VEC V2)
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z + V2.Z;
}
__inline DBL VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
}
__inline DBL VecLen2( VEC V )
{
  DBL len = VecDotVec(V, V);

  return len;
}
__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}
__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}
__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR m = UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  
  return m;
}
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a, co, si;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);

  m.A[1][1] = co;
  m.A[2][2] = co;
  m.A[1][2] = si;
  m.A[2][1] = -si;
  
  return m;
}
__inline MATR MatrRotateY( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a, co, si;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);

  m.A[0][0] = co;
  m.A[2][2] = co;
  m.A[2][0] = si;
  m.A[0][2] = -si;
  
  return m;
}
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a, co, si;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);

  m.A[0][0] = co;
  m.A[1][1] = co;
  m.A[1][0] = si;
  m.A[1][0] = -si;
  
  return m;
}
__inline MATR MatrTranspose( MATR M )
{
  MATR N;
  INT i, j;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      N.A[j][i] = M.A[i][j];
  return N;
}
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}
__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  INT i, j;
  DBL det = MatrDeterm(M);
  INT perm[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return UnitMatrix;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        MatrDeterm3x3(
          M.A[perm[i][0]][perm[j][0]],
          M.A[perm[i][0]][perm[j][1]],
          M.A[perm[i][0]][perm[j][2]],
          M.A[perm[i][1]][perm[j][0]],
          M.A[perm[i][1]][perm[j][1]],
          M.A[perm[i][1]][perm[j][2]],
          M.A[perm[i][2]][perm[j][0]],
          M.A[perm[i][2]][perm[j][1]],
          M.A[perm[i][2]][perm[j][2]]) / det;
  return r;
}
__inline VEC PointTransform(VEC V, MATR M)
{
  return VecSet( V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                   V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                   V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
__inline VEC VectorTransform(VEC V, MATR M)
{
  return VecSet(   V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                   V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                   V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}
#endif