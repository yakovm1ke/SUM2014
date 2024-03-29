/* FILENAME: PRIM.C
 * PROGRAMMER: MY6
 * PURPOSE: Primtive handle functions.
 * LAST UPDATE: 17.06.2014
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "anim.h"
#include "render.h"
#include "geom.h"
#include "shader.h"

/* ������� */
#define power(A, B) ((A) < 0 ? -pow(-A, B) : pow(A, B))

/* ���� ��-��������� */
COLOR MY6_DefaultColor = {1, 1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ����������� ��������:
 *       my6PRIM *P;
 *   - ��� ���������:
 *       my6PRIM_TYPE Type;
 *   - ��������� ����� (������, ������) ���
 *     Type == MY6_PRIM_GRID ��� ���������� ������
 *     � ��������:
 *       INT NumOfV_GW, NumOfI_GH;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL MY6_PrimCreate( my6PRIM *P, my6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH )
{
  INT i, j, nv, ni, size;

  memset(P, 0, sizeof(my6PRIM));
  /* ����������� ��������� */
  switch (Type)
  {
  case MY6_PRIM_TRIMESH:
    nv = NumOfV_GW;
    ni = NumOfI_GH;
    break;
  case MY6_PRIM_GRID:
    nv = NumOfV_GW * NumOfI_GH;
    ni = NumOfV_GW * 2;
    break;
  }
  /* �������� ������ */
  size = sizeof(my6VERTEX) * nv + sizeof(INT) * ni;
  P->V = malloc(size);
  if (P->V == NULL)
    return FALSE;
  memset(P->V, 0, size);
  
  /* ��������� �������� */
  P->Size = size;
  P->Type = Type;
  P->NumOfV = nv;
  P->NumOfI = ni;
  P->GW = NumOfV_GW;
  P->GH = NumOfI_GH;
  P->I = (INT *)(P->V + nv);
  /* ��������� ��� ������� ��-��������� */
  for (i = 0; i < nv; i++)
  {
    P->V[i].C = MY6_DefaultColor;
    P->V[i].N = VecSet(0, 1, 0);
  }

  /* ��������� ������� � �������� ��� ���. ����� */
  if (Type == MY6_PRIM_GRID)
  {
    for (i = 0; i < NumOfI_GH; i++)
      for (j = 0; j < NumOfV_GW; j++)
        P->V[i * NumOfV_GW + j].T =
          MY6_UVSet(j / (NumOfV_GW - 1.0),
                    i / (NumOfI_GH - 1.0));
    for (i = 0; i < NumOfV_GW; i++)
    {
      P->I[2 * i + 0] = NumOfV_GW + i;
      P->I[2 * i + 1] = i;
    }
  }
  return TRUE;
} /* End of 'MY6_PrimCreate' function */

/* ������� �������� �����.
 * ���������:
 *   - ����������� ��������:
 *       my6PRIM *P;
 *   - ����� �����:
 *       VEC C;
 *   - ������ �����:
 *       FLT R;
 *   - ��������� ����� (������, ������):
 *       INT M, N;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL MY6_PrimCreateSphere( my6PRIM *P, VEC C, FLT R, INT M, INT N )
{
  INT i, j;
  DBL t = clock() / 1000.0, t1;

  t1 = 2 + 2 * cos(t);
  t = 2 + 2 * sin(t * 1.01 + 2);

  if (!MY6_PrimCreate(P, MY6_PRIM_GRID, M, N))
    return FALSE;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      my6VERTEX *V = P->V + i * M + j;
      DBL
        theta = i / (N - 1.0) * MY6_PI,
        phi = j / (M - 1.0) * 2 * MY6_PI;

      V->N = VecSet(power(sin(theta), t1) * power(sin(phi), t),
                    power(cos(theta), t1),
                    power(sin(theta), t1) * power(cos(phi), t));
      V->P = VecAddVec(VecMulNum(V->N, R), C);
    }
  return TRUE;
} /* End of 'MY6_PrimCreateSphere' function */

/* ������� �������� ��������.
 * ���������:
 *   - ����������� ��������:
 *       my6PRIM *P;
 *   - ������ ��������:
 *       FLT H;
 *   - ������ ��������:
 *       FLT R;
 *   - ��������� ����� (������, ������):
 *       INT M, N;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL MY6_PrimCreateCylinder( my6PRIM *P, FLT H, FLT R, INT M, INT N )
{
  INT i, j;

  if (!MY6_PrimCreate(P, MY6_PRIM_GRID, M, N))
    return FALSE;
  /* ������ ���������� ����� */
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      my6VERTEX *V = P->V + i * M + j;
      DBL
        phi = j / (M - 1.0) * 2 * MY6_PI;

      V->N = VecSet(0, sin(phi), cos(phi));
      V->P = VecSet(-H / 2 + i * H / (N - 1), V->N.Y * R, V->N.Z * R );
      V->T.U *= 1300;
      V->T.V *= 7;  /* ���������� ����� */
    }
  return TRUE;
} /* End of 'MY6_PrimCreateCylinder' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� ��������:
 *       my6PRIM *P;
 * ������������ ��������: ���.
 */
VOID MY6_PrimFree( my6PRIM *P )
{
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(my6PRIM));
} /* End of 'MY6_PrimFree' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ��������:
 *       my6PRIM *P;
 * ������������ ��������: ���.
 */
VOID MY6_PrimDraw( my6PRIM *P )
{
  INT i;

  if (P->V == NULL)
    return;

  if (P->Buffers[0] == 0)
  {
    /* ������� � ��������� ������ ������ */
    glGenBuffers(2, P->Buffers);
    glGenVertexArrays(1, &P->VertexBuffer);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(my6VERTEX) * P->NumOfV,
      P->V, GL_STATIC_DRAW);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * P->NumOfI,
      P->I, GL_STATIC_DRAW);
  }

  /* ���������� ������ */
  glBindVertexArray(P->VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P->Buffers[1]);

  /* ������ ������� ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(my6VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(my6VERTEX), (VOID *)(sizeof(VEC)));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(my6VERTEX), (VOID *)(sizeof(VEC) + sizeof(my6UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(my6VERTEX), (VOID *)(2 * sizeof(VEC) + sizeof(my6UV)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glUseProgram(MY6_ShaderProg);
  if (P->Type == MY6_PRIM_TRIMESH)
  {
    /*
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfI; i++)
      glVertex3fv(&P->V[P->I[i]].P.X);
    glEnd();
    */
    glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  else
  {
    for (i = 0; i < P->GH - 1; i++)
    {
      /*
      INT start = i * P->GW, j;

      glBegin(GL_TRIANGLE_STRIP);
      for (j = 0; j < P->NumOfI; j++)
        glVertex3fv(&P->V[start + P->I[j]].P.X);
      glEnd();
      */
      /*
      glDrawElements(GL_TRIANGLE_STRIP, P->NumOfI, GL_UNSIGNED_INT,
        (VOID *)(i * P->GW * 2 * sizeof(INT)));
          glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
      */
      glDrawElementsBaseVertex(GL_TRIANGLE_STRIP,
        P->NumOfI, GL_UNSIGNED_INT, (VOID *)0, i * P->GW);

    }
  }
  glUseProgram(0);
} /* End of 'MY6_PrimFree' function */

/* END OF 'PRIM.C' FILE */
