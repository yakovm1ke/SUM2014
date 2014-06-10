/* FILENAME: CUBE.C
 * PROGRAMMER: MY6
 * PURPOSE: Cube Animation.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"

typedef struct tagmy6UNIT_CUBE
{
  MY6_UNIT_BASE_FIELDS; /* Включение базовых полей */
} my6UNIT_CUBE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitInit( my6UNIT_CUBE *Unit, my6ANIM *Ani )
{
} /* End of 'CubeUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitClose( my6UNIT_CUBE *Unit, my6ANIM *Ani )
{
} /* End of 'CubeUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitResponse( my6UNIT_CUBE *Unit, my6ANIM *Ani )
{
  MY6_RndWs = Ani->W;
  MY6_RndHs = Ani->H;
  MY6_RndWp = MY6_RndHp * Ani->W / Ani->H;
} /* End of 'CubeUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitRender( my6UNIT_CUBE *Unit, my6ANIM *Ani )
{
  INT i, s = 12;
  VEC p;
  POINT pt;
  //                  0          1            2           3           4            5             6            7
  VEC CubeP[8] = {{1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {-1, 1, 1}, {1, -1, -1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}};
  typedef struct
  {
    INT P0, P1;
  } EDGE;
  EDGE CubeE[12] = {{0, 1}, {0, 3}, {0, 2}, {2, 4}, {2, 6}, {6, 3}, {3, 5}, {5, 1}, {1, 4}, {6, 7}, {7, 4}, {7, 5}};
  VEC ppp = {0, 0, 0};

  MY6_RndMatrView = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  MY6_RndMatrWorld = MatrRotateY(Ani->Time * 30);;

  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));

  for (i = 0; i < 8; i++)
  {
    p = CubeP[i];

    pt = MY6_RndWorldToScreen(p);
    Ellipse(Ani->hDC, pt.x - s, pt.y - s, pt.x + s, pt.y + s);
  }
  for (i = 0; i < 12; i++)
  {
    p = CubeP[i];
    pt = MY6_RndWorldToScreen(p);
    if (i == 0)
      MoveToEx(Ani->hDC, pt.x, pt.y, NULL);
    else
      LineTo(Ani->hDC, pt.x, pt.y);
  }
} /* End of 'CubeUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT *) указатель на созданный объект анимации.
 */
my6UNIT * MY6_CubeUnitCreate( VOID )
{
  my6UNIT_CUBE *Unit;

  if ((Unit = (my6UNIT_CUBE *)MY6_AnimUnitCreate(sizeof(my6UNIT_CUBE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;
  Unit->Render = (VOID *)CubeUnitRender;
  return (my6UNIT *)Unit;
} /* End of 'MY6_CubeUnitCreate' function */

/* END OF 'CUBE.C' FILE */
