/* FILENAME: SHADER.H
 * PROGRAMMER: MY6
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Функция инициализации шейдерной программы */
UINT MY6_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Функция удаления шейдерной программы */
VOID MY6_ShadProgClose( UINT ProgId );

extern UINT MY6_ShaderProg;

#endif /* __SHADER_H_ */

/* END OF 'SHADER.H' FILE */