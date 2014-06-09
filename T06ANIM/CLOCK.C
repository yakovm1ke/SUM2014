/* FILENAME: SAMPLE.C
 * PROGRAMMER: MY6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagmy6UNIT_CLOCK
{
  MY6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
} my6UNIT_CLOCK;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitInit( my6UNIT_CLOCK *Unit, my6ANIM *Ani )
{
} /* End of 'ClockUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitClose( my6UNIT_CLOCK *Unit, my6ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitResponse( my6UNIT_CLOCK *Unit, my6ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */
/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitRender( my6UNIT_CLOCK *Unit, my6ANIM *Ani )
{
  SYSTEMTIME st;
  FLOAT PI = 3.14159265358979323846;
  
  GetLocalTime(&st);

  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(0, 0, 0));
  SetDCBrushColor(Ani->hDC, RGB(0, 0, 255));
  DrawArrow(Ani->hDC, Ani->W / 2, Ani->H / 2 , 100, 50, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * PI);
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(Ani->hDC, RGB(0, 255, 0));
  DrawArrow(Ani->hDC, Ani->W / 2, Ani->H / 2 , 140, 45, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * PI);
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(Ani->hDC, RGB(255, 0, 0));
  DrawArrow(Ani->hDC, Ani->W / 2, Ani->H / 2 , 200, 30, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * PI);
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(Ani->hDC, RGB(0, 0, 0));
} /* End of 'MY6_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT *) указатель на созданный объект анимации.
 */
my6UNIT * MY6_ClockUnitCreate( VOID )
{
  my6UNIT_CLOCK *Unit;

  if ((Unit = (my6UNIT_CLOCK *)MY6_AnimUnitCreate(sizeof(my6UNIT_CLOCK))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)ClockUnitInit;
  Unit->Close = (VOID *)ClockUnitClose;
  Unit->Response = (VOID *)ClockUnitResponse;
  Unit->Render = (VOID *)ClockUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (my6UNIT *)Unit;
} /* End of 'MY6_CowUnitCreate' function */
/*********************************************************/
typedef struct tagmy6UNIT_INFO
{
  MY6_UNIT_BASE_FIELDS;
  IMAGE And, Xor;
}my6UNIT_INFO;

static VOID InfoUnitInit( my6UNIT_INFO *Unit, my6ANIM *Ani )
{
  ImageLoad(&Unit->And, "logo_and.bmp");
  ImageLoad(&Unit->Xor, "logo_xor.bmp");
}/*End of 'InfoUnitInit' function "*/

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( my6UNIT_INFO *Unit, my6ANIM *Ani )
{
  static CHAR Buf[1000];
  static INT cx = 500, cy = 500;

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 30, 30, Buf, sprintf(Buf, "FPS: %.3f, JR: %f, JX: %f, JY: %f, JZ: %f", Ani->FPS, Ani->JR, Ani->JX, Ani->JY, Ani->JZ));

  cx += Ani->JR * 20;
  cy += Ani->JZ * 20;
  if (cx > 1500)
    cx = - 300;
  if (cx < - 300)
    cx = 1500;
  if (cy > 1200)
    cy = - 300;
  if (cy < - 300)
    cy = 1200;
  ImageDraw(&Unit->And, Ani->hDC,cx, cy, SRCAND );
  ImageDraw(&Unit->Xor, Ani->hDC,cx, cy, SRCINVERT );
} /* End of 'MY6_AnimUnitRender' function */
/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT *) указатель на созданный объект анимации.
 */
my6UNIT * MY6_InfoUnitCreate( VOID )
{
  my6UNIT *Unit;

  if ((Unit = (my6UNIT_INFO *)MY6_AnimUnitCreate(sizeof(my6UNIT_INFO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'MY6_InfoUnitCreate' function */

/**************************************************************************/
/* END OF 'SAMPLE.C' FILE */
