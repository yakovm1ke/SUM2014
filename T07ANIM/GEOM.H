/* FILENAME: GEOM.H
 * PROGRAMMER: MY6
 * PURPOSE: Geom declaration module.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __GEOM_H_
#define __GEOM_H_

#include "anim.h"

/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagmy6UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} my6UV;

/* ������� ������������� ���������� ��������� */
__inline my6UV MY6_UVSet( FLT U, FLT V )
{
  my6UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'MY6_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagmy6VERTEX
{
  VEC P;    /* ���������� */
  my6UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} my6VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagmy6PRIM_TYPE
{
  MY6_PRIM_TRIMESH, MY6_PRIM_GRID
} my6PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagmy6PRIM
{
  my6PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  my6VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} my6PRIM;

/* ���� ��-��������� */
extern COLOR MY6_DefaultColor;

extern my6PRIM prim;

/* ������� �������� ��������� */
BOOL MY6_PrimCreate( my6PRIM *P, my6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL MY6_PrimCreateSphere( my6PRIM *P,
       VEC C, FLT R, INT M, INT N );

BOOL MY6_PrimCreateCylinder( my6PRIM *P, FLT H, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID MY6_PrimFree( my6PRIM *P );

/* ������� ��������� ��������� */
VOID MY6_PrimDraw( my6PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagmy6MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} my6MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagmy6GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  my6MATERIAL *Mtls;         /* ������� ���������� */
  my6PRIM *Prims;            /* ������ ���������� */
  FLT Data[10];
} my6GEOM;

//extern my6PRIM prim = {0};
//extern my6MATERIAL mtl = {0};


/* ������� ���������� ��������� � ������� */
INT MY6_GeomAddMaterial( my6GEOM *G,
                         my6MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT MY6_GeomAddPrim( my6GEOM *G, my6PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID MY6_GeomFree( my6GEOM *G );

/* ������� ��������� */
VOID MY6_GeomDraw( my6GEOM *G );

VOID MY6_GeomTransform( my6GEOM *G, MATR M );

/* ������� �������� ��������������� ������� */
BOOL MY6_GeomLoad( my6GEOM *G, CHAR *FileName );

#endif /* __GEOM_H_ */

/* END OF GEOM.H FILE */
