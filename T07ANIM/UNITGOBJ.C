/* FILENAME: UNITGOBJ.C
 * PROGRAMMER: MY6q
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 14.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"

/* Структура описания объекта анимации */
typedef struct tagmy6UNIT_GOBJ
{
  MY6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  my6GOBJ GObj;
} my6UNIT_GOBJ;
/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitInit( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  MY6_RndGObjLoad( &Unit->GObj, "Porsche_911_GT2.obj");
} /* End of 'GObjUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitClose( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  MY6_RndGObjFree(&Unit->GObj);
} /* End of 'GObjUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitResponse( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    MY6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    MY6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
} /* End of 'GObjUnitResponse' function */

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       my6UNIT_GOBJ *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitRender( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  MY6_RndGObjDraw(&Unit->GObj, Ani->hDC);
} /* End of 'GObjUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT_GOBJ *) указатель на созданный объект анимации.
 */
my6UNIT * MY6_GObjUnitCreate( VOID )
{
  my6UNIT_GOBJ *Unit;

  if ((Unit = (my6UNIT_GOBJ *)MY6_AnimUnitCreate(sizeof(my6UNIT_GOBJ))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)GObjUnitInit;
  Unit->Close = (VOID *)GObjUnitClose;
  Unit->Render = (VOID *)GObjUnitRender;
  Unit->Response = (VOID *)GObjUnitResponse;
  return (my6UNIT *)Unit;
} /* End of 'MY6_GObjUnitCreate' function */

/* END OF 'UNITGOBJ.C' FILE */
