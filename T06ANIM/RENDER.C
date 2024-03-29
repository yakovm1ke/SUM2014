/* FILENAME: RENDER.C
 * PROGRAMMER: MY6
 * PURPOSE: Render functions.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  MY6_RndProjDist = 3;

MATR MY6_RndMatrWorld = UnitMatr;
MATR MY6_RndMatrView = UnitMatr;
MATR MY6_RndMatrProjection = UnitMatr;

static MATR MY6_RndMatrWorldViewProj;

VOID MY6_RndMatrSetup( VOID )
{
  MY6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(MY6_RndMatrWorld, MY6_RndMatrView), MY6_RndMatrProjection);
} /* End of 'MY6_RndMatrSetup' function */

POINT MY6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* �������������� �� */
  Pp = PointTransform(P, MY6_RndMatrWorldViewProj);

  /* ������������� */
  Ps.x = (Pp.X + 0.5) * (MY6_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5) * (MY6_RndHs - 1);
  return Ps;
}
BOOL MY6_RndGObjLoad( my6GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* �������� ������ */
  memset(GObj, 0, sizeof(my6GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* ������� ���������� ������ � ������ */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* �������� ������ ��� ������� � ����� ��� ������ ������� ������
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* ��������� ������ */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
      sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i %i %i", &a, &b, &c); 

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'MY6_RndGObjLoad' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       my6GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID MY6_RndGObjFree( my6GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(my6GOBJ));
} /* End of 'MY6_RndGObjFree' function */

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       my6GOBJ *GObj;
 *   - �������� ���������� ������:
 *       HDC hDC;
 * ������������ ��������: ���.
 */
VOID MY6_RndGObjDraw( my6GOBJ *GObj, HDC hDC )
{
  INT i;
  POINT *pts;

  if ((pts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* ��������� ������� �������������� */
  MY6_RndMatrSetup();

  for (i = 0; i < GObj->NumOfV; i++)
    pts[i] = MY6_RndWorldToScreen(GObj->V[i]);

  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];

    MoveToEx(hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(hDC, pts[n1].x, pts[n1].y);
    LineTo(hDC, pts[n2].x, pts[n2].y);
    LineTo(hDC, pts[n0].x, pts[n0].y);
  }
  free(pts);
} /* End of 'MY6_RndGObjDraw' function */


/*END OF 'RENDER.C' FILE*/