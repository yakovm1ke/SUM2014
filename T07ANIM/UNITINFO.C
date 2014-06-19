/* FILENAME: UNITINFO.C
 * PROGRAMMER: MY6
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"

/* Структура описания объекта анимации */
typedef struct tagmy6UNIT_INFO
{
  MY6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL cx, cy, sp;
} my6UNIT_INFO;
/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitInit( my6UNIT_INFO *Unit, my6ANIM *Ani )
{
  wglUseFontBitmaps(Ani->hDC, 0, 256, 111);
} /* End of 'InfoUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitClose( my6UNIT_INFO *Unit, my6ANIM *Ani )
{
} /* End of 'InfoUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitResponse( my6UNIT_INFO *Unit, my6ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    MY6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    MY6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
} /* End of 'InfoUnitResponse' function */

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       my6UNIT_INFO *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( my6UNIT_INFO *Unit, my6ANIM *Ani )
{
  static CHAR Buf[30];

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);

  Ani->MatrWorld = MatrIdentity();
  glLoadMatrixf(Ani->MatrWorld.A[0]);


  glColor3d(0, 0, 0);
  glRasterPos3d(-0.988, 0.948, 0); 
  glListBase(111);

  glCallLists(sprintf(Buf, "FPS: %.3f", Ani->FPS), GL_UNSIGNED_BYTE, Buf);

  glColor3d(1, 0.5, 1);
  glRasterPos3d(-0.99, 0.95, 0); 
  glListBase(111);

  glCallLists(sprintf(Buf, "FPS: %.3f", Ani->FPS), GL_UNSIGNED_BYTE, Buf);

  glPopAttrib();
} /* End of 'InfoUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT_INFO *) указатель на созданный объект анимации.
 */
my6UNIT * MY6_InfoUnitCreate( VOID )
{
  my6UNIT_INFO *Unit;

  if ((Unit = (my6UNIT_INFO *)MY6_AnimUnitCreate(sizeof(my6UNIT_INFO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  Unit->cx = Unit->cy = 0;
  Unit->sp = 1;
  return (my6UNIT *)Unit;
} /* End of 'MY6_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
