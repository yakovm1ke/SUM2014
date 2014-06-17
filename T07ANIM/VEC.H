/* FILENAME: VEC.H
 * PROGRAMMER: MY6
 * PURPOSE: Math declaration module.
 * LAST UPDATE: 17.06.2014
 */
#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#include <windows.h>

#pragma warning(disable: 4244)

#define MY6_PI 3.14159265358979323846
#define MY6_D2R(A) ((A) * (MY6_PI / 180.0))
#define MY6_R2D(A) ((A) * (180.0 / MY6_PI))

typedef double DBL;
typedef float FLT;

typedef struct tagCOLOR
{
  FLT B, G, R, A;
} COLOR;

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

extern MATR UnitMatrix;

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
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
__inline VEC VecSubVec(VEC V1, VEC V2)
{
  return VecSet( V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z );
}
__inline VEC VecDivNum( VEC V, FLT N )
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
__inline VEC VecMulNum( VEC V, FLT N )
{
  return VecSet( V.X * N, V.Y * N, V.Z * N );
}
__inline VEC VecNormalize( VEC V )
{
  FLT len = V.X * V.X + V.Y + V.Y + V.Z * V.Z;
  
  if(len != 0 && len != 1)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
}

__inline FLT VecDotVec(VEC V1, VEC V2)
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}
__inline FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
}
__inline FLT VecLen2( VEC V )
{
  FLT len = VecDotVec(V, V);

  return len;
}
__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}
__inline MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}
__inline MATR MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  MATR m = UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  
  return m;
}
__inline MATR MatrRotateX( FLT AngleInDegree )
{
  MATR m = UnitMatrix;
  FLT a, co, si;

  a = MY6_D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);

  m.A[1][1] = co;
  m.A[2][2] = co;
  m.A[1][2] = si;
  m.A[2][1] = -si;
  
  return m;
}
__inline MATR MatrRotateY( FLT AngleInDegree )
{
  MATR m = UnitMatrix;
  FLT a, co, si;

  a = MY6_D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);

  m.A[0][0] = co;
  m.A[2][2] = co;
  m.A[2][0] = si;
  m.A[0][2] = -si;
  
  return m;
}
__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  MATR m = UnitMatrix;
  FLT a, co, si;

  a = MY6_D2R(AngleInDegree);
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
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}
__inline FLT MatrDeterm( MATR M )
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
  FLT det = MatrDeterm(M);
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
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}
__inline VEC VectorTransform(VEC V, MATR M)
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR m;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
}

__inline MATR MatrViewLookAt( VEC Loc, VEC At, VEC Up )
{
  VEC
    Dir = VecSubVec(At, Loc),
    Right = VecCrossVec(Dir, Up);
  MATR m;

  Dir = VecNormalize(Dir);
  Right = VecNormalize(Right);
  Up = VecCrossVec(Right, Dir);

  m.A[0][0] = Right.X;
  m.A[1][0] = Right.Y;
  m.A[2][0] = Right.Z;
  m.A[3][0] = -VecDotVec(Right, Loc);
  m.A[0][1] = Up.X;
  m.A[1][1] = Up.Y;
  m.A[2][1] = Up.Z;
  m.A[3][1] = -VecDotVec(Up, Loc);
  m.A[0][2] = -Dir.X;
  m.A[1][2] = -Dir.Y;
  m.A[2][2] = -Dir.Z;
  m.A[3][2] = VecDotVec(Dir, Loc);
  m.A[0][3] = 0;
  m.A[1][3] = 0;
  m.A[2][3] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrViewLookAt' function */

__inline MATR MatrProjection( FLT Left, FLT Right,
                              FLT Bottom, FLT Top,
                              FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
} /* End of 'MatrProjection' function */

__inline COLOR ColorSet( FLT R, FLT G, FLT B )
{
  COLOR C;

  C.R = R;
  C.G = G;
  C.B = B;
  C.A = 1;
  return C;
} /* End of 'ColorSetA' function */

__inline COLOR ColorSetA( FLT R, FLT G, FLT B, FLT A )
{
  COLOR C;

  C.R = R;
  C.G = G;
  C.B = B;
  C.A = A;
  return C;
} /* End of 'ColorSetA' function */
#endif /* __VEC_H_ */

/* END OF 'VEC.H' FILE */
