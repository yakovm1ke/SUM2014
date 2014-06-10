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

/* Глобальная камера */
extern my6CAMERA MY6_RndCam;


/* Параметры проецирования */
extern DBL
  MY6_RndWs, MY6_RndHs, /* размеры кадра в точках */
  MY6_RndWp, MY6_RndHp, /* размеры обрасти проецирования */
  MY6_RndProjDist;      /* расстояние до плоскости проекции */

/* Матрицы */
extern MATR
  MY6_RndMatrWorld, /* матрица преобразования мировой СК */
  MY6_RndMatrView;  /* матрица преобразования видовой СК */

/* Функция преобразования из мировой системы коорлинат в кадр */
POINT MY6_RndWorldToScreen( VEC P );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
