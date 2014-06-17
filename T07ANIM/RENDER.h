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

extern COLOR MY6_DefaultColor;

BOOL MY6_RndGObjLoad( my6GOBJ *GObj, CHAR *FileName );

VOID MY6_RndGObjFree( my6GOBJ *GObj );

VOID MY6_RndGObjDraw( my6GOBJ *GObj);

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
