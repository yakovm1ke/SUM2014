/* FILENAME: SHADER.C
 * PROGRAMMER: MY6
 * PURPOSE: Shader handle functions.
 * LAST UPDATE: 17.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

/* ������� �������� ���������� ����� � ������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (CHAR *) - ��������� �� ���������� �
 *              ����������� ������ (��� NULL).
 */
static CHAR *LoadFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT len;

  if ((F = fopen(FileName, "r")) == NULL)
    return NULL;

  /* ���������� ����� ����� */
  fseek(F, 0, SEEK_END);
  len = ftell(F);

  /* �������� ������ */
  if ((txt = malloc(len + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  /* ��������� ���� */
  rewind(F);
  len = fread(txt, 1, len, F);
  txt[len] = 0;
  fclose(F);
  return txt;
} /* End of 'LoadFile' function */

/* ������� ������������� ��������� ���������.
 * ���������:
 *   - ����� ������ ���������� � ������������ ��������:
 *       CHAR *VSFileName, *FSFileName;
 * ������������ ��������:
 *   (UINT) - ������������� ����������� � ����������������
 *            ��������� ��� 0 ��� ������.
 */
UINT MY6_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName )
{
  CHAR *txt;
  UINT shv, shf, prg;
  INT res, len;
  static CHAR Buf[1000];

  /* �������������� ��������� ������ */
  if ((txt = LoadFile(VSFileName)) == NULL)
    return 0;
  if ((shv = glCreateShader(GL_VERTEX_SHADER)) == 0)
  {
    free(txt);
    return 0;
  }
  /* ���������� ����� */
  glShaderSource(shv, 1, &txt, NULL);
  free(txt);
  /* ����������� ������ */
  glCompileShader(shv);
  glGetShaderiv(shv, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shv, sizeof(Buf), &len, Buf);
    glDeleteShader(shv);
    return 0;
  }

  /* �������������� ����������� ������ */
  if ((txt = LoadFile(FSFileName)) == NULL)
  {
    glDeleteShader(shv);
    return 0;
  }
  if ((shf = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
  {
    glDeleteShader(shv);
    free(txt);
    return 0;
  }
  /* ���������� ����� */
  glShaderSource(shf, 1, &txt, NULL);
  free(txt);
  /* ����������� ������ */
  glCompileShader(shf);
  glGetShaderiv(shf, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shf, sizeof(Buf), &len, Buf);
    glDeleteShader(shf);
    glDeleteShader(shv);
    return 0;
  }

  /* �������������� ��������� - ����� �������� */
  if ((prg = glCreateProgram()) == 0)
  {
    glDeleteShader(shf);
    glDeleteShader(shv);
    return 0;
  }
  /* ������������ � ��������� ������� */
  glAttachShader(prg, shv);
  glAttachShader(prg, shf);

  /* ��������� ��������� */
  glLinkProgram(prg);
  glGetProgramiv(prg, GL_LINK_STATUS, &res);
  if (res != 1)
  {
    glGetProgramInfoLog(prg, sizeof(Buf), &len, Buf);
    glDetachShader(prg, shv);
    glDetachShader(prg, shf);
    glDeleteShader(shv);
    glDeleteShader(shf);
    glDeleteProgram(prg);
    return 0;
  }
  return prg;
} /* End of 'MY6_ShadProgInit' function */

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ������������� ����������� ���������:
 *       UINT ProgId;
 * ������������ ��������: ���.
 */
VOID MY6_ShadProgClose( UINT ProgId )
{
  UINT i, n, shdrs[4];

  if (ProgId == 0)
    return;
  /* �������� �������������� ������� */
  glGetAttachedShaders(ProgId, 4, &n, shdrs);

  /* ������� */
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'MY6_ShadProgClose' function */

/* END OF 'SHADER.C' FILE */
