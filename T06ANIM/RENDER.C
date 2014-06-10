/* FILENAME: RENDER.C
 * PROGRAMMER: MY6
 * PURPOSE: Render functions.
 * LAST UPDATE: 10.06.2014
 */

#include "render.h"

#define UnitMatr   \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1},  \
  }                \
};
/*Global Camera */
my6CAMERA MY6_RndCam;

DBL 
  MY6_RndWs = 30, MY6_RndHs = 30,
  MY6_RndWp = 4, MY6_RndHp = 3,
  MY6_RndProjDist = 5;

MATR MY6_RndMatrWorld = UnitMatr;
MATR MY6_RndMatrView = UnitMatr;

POINT MY6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = PointTransform(P, MatrMulMatr(MY6_RndMatrWorld, MY6_RndMatrView));

  /* проецирование */
  Pp.X *= MY6_RndProjDist / Pp.Z;
  Pp.Y *= MY6_RndProjDist / Pp.Z;

  Ps.x = (Pp.X + MY6_RndWp / 2) / MY6_RndWp * (MY6_RndWs - 1);
  Ps.y = (-Pp.Y + MY6_RndHp / 2) / MY6_RndHp * (MY6_RndHs - 1);
  return Ps;
}

/*END OF 'RENDER.C' FILE*/