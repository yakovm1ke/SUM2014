/* FILENAME: RENDER.H
 * PROGRAMMER: MY6
 * PURPOSE: Render Declaretion Module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} my6CAMERA;

typedef struct tagmy6GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} my6GOBJ;

/* ���������� ������ */
extern my6CAMERA MY6_RndCam;


/* ��������� ������������� */
extern DBL
  MY6_RndWs, MY6_RndHs, /* ������� ����� � ������ */
  MY6_RndWp, MY6_RndHp, /* ������� ������� ������������� */
  MY6_RndProjDist;      /* ���������� �� ��������� �������� */

/* ������� */
extern MATR
  MY6_RndMatrWorld, /* ������� �������������� ������� �� */
  MY6_RndMatrView,  /* ������� �������������� ������� �� */
  MY6_RndMatrProjection;

/* ������� �������������� �� ������� ������� ��������� � ���� */
POINT MY6_RndWorldToScreen( VEC P );

BOOL MY6_RndGObjLoad( my6GOBJ *GObj, CHAR *FileName );

VOID MY6_RndGObjFree( my6GOBJ *GObj );

VOID MY6_RndGObjDraw( my6GOBJ *GObj, HDC hDC );

VOID MY6_RndMatrSetup( VOID );

static MATR MY6_RndMatrWorldViewProj;

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
