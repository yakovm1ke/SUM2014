/* FILENAME: RENDER.H
 * PROGRAMMER: MY6
 * PURPOSE: Render Declaretion Module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} my6CAMERA;

typedef struct tagmy6GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} my6GOBJ;

extern COLOR MY6_DefaultColor;

BOOL MY6_RndGObjLoad( my6GOBJ *GObj, CHAR *FileName );

VOID MY6_RndGObjFree( my6GOBJ *GObj );

VOID MY6_RndGObjDraw( my6GOBJ *GObj);

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
