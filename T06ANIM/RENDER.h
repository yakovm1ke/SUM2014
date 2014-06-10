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
  MY6_RndMatrView;  /* ������� �������������� ������� �� */

/* ������� �������������� �� ������� ������� ��������� � ���� */
POINT MY6_RndWorldToScreen( VEC P );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
