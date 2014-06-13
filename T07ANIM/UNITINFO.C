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
  MATR WVP;
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);

  Ani->MatrView = MatrViewLookAt(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));

  Unit->cy += Ani->JX * 5;
  Unit->cx += Ani->JY * 5;
  //sp += Ani->JR / 60.0;
  if (Ani->JButsClick[6])
    Unit->sp += 0.5;
  if (Ani->JButsClick[4])
    Unit->sp -= 0.5;
  Ani->MatrProjection = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                           -Ani->Hp / 2, Ani->Hp / 2,
                                            Ani->ProjDist, Ani->FarClip);

  Ani->MatrWorld = MatrScale( 1 + Unit->sp, 1 + Unit->sp, 1 + Unit->sp);
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateY(-Unit->cy));
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateX(-Unit->cx));

  WVP = MatrMulMatr(Ani->MatrWorld, MatrMulMatr(Ani->MatrView, Ani->MatrProjection));

  glLoadMatrixd( WVP.A[0] );

  glColor3d(0.32 + sin(Ani->Time ) / 2, 0.1 + Ani->JButs [1] / 1.5, 0.85);
  glRectd(0, 0, 0.5, 0.5);
  
  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();
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
