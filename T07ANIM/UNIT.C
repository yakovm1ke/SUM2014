/* FILENAME: UNIT.C
 * PROGRAMMER: MY6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MY6_AnimUnitInit( my6UNIT *Unit, my6ANIM *Ani )
{
} /* End of 'MY6_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MY6_AnimUnitClose( my6UNIT *Unit, my6ANIM *Ani )
{
} /* End of 'MY6_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MY6_AnimUnitResponse( my6UNIT *Unit, my6ANIM *Ani )
{
} /* End of 'MY6_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       my6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       my6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MY6_AnimUnitRender( my6UNIT *Unit, my6ANIM *Ani )
{
} /* End of 'MY6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (my6UNIT *) ��������� �� ��������� ������ ��������.
 */
my6UNIT * MY6_AnimUnitCreate( INT Size )
{
  my6UNIT *Unit;

  if (Size < sizeof(my6UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* ��������� ���� ��-��������� */
  Unit->Size = Size;
  Unit->Init = MY6_AnimUnitInit;
  Unit->Close = MY6_AnimUnitClose;
  Unit->Response = MY6_AnimUnitResponse;
  Unit->Render = MY6_AnimUnitRender;
  return Unit;
} /* End of 'MY6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

