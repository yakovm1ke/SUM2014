/* FILENAME: SAMPLE.C
 * PROGRAMMER: MY6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagmy6UNIT_COW
{
  MY6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
} my6UNIT_COW;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitInit( my6UNIT_COW *Unit, my6ANIM *Ani )
{
} /* End of 'CowUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitClose( my6UNIT_COW *Unit, my6ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitResponse( my6UNIT_COW *Unit, my6ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitRender( my6UNIT_COW *Unit, my6ANIM *Ani )
{
  DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * 30,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * 30;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
} /* End of 'MY6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (my6UNIT *) ��������� �� ��������� ������ ��������.
 */
my6UNIT * MY6_CowUnitCreate( VOID )
{
  my6UNIT_COW *Unit;

  if ((Unit = (my6UNIT_COW *)MY6_AnimUnitCreate(sizeof(my6UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (my6UNIT *)Unit;
} /* End of 'MY6_CowUnitCreate' function */
/***********************************************************/
//Files in Clock.c beetween /***/
/* END OF 'SAMPLE.C' FILE */
