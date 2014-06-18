/* FILENAME: GEOM.H
 * PROGRAMMER: MY6
 * PURPOSE: Geom declaration module.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __GEOM_H_
#define __GEOM_H_

#include "anim.h"

/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagmy6UV
{
  FLT U, V; /* компоненты текстурных координат */
} my6UV;

/* Функиця инициализации текстурных координат */
__inline my6UV MY6_UVSet( FLT U, FLT V )
{
  my6UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'MY6_UVSet' function */

/* Теп представления вершины */
typedef struct tagmy6VERTEX
{
  VEC P;    /* координаты */
  my6UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} my6VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagmy6PRIM_TYPE
{
  MY6_PRIM_TRIMESH, MY6_PRIM_GRID
} my6PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagmy6PRIM
{
  my6PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  my6VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} my6PRIM;

/* Цвет по-умолчанию */
extern COLOR MY6_DefaultColor;

extern my6PRIM prim;

/* Функция создания примитива */
BOOL MY6_PrimCreate( my6PRIM *P, my6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL MY6_PrimCreateSphere( my6PRIM *P,
       VEC C, FLT R, INT M, INT N );

BOOL MY6_PrimCreateCylinder( my6PRIM *P, FLT H, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID MY6_PrimFree( my6PRIM *P );

/* Функция отрисовки примитива */
VOID MY6_PrimDraw( my6PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagmy6MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} my6MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagmy6GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  my6MATERIAL *Mtls;         /* таблица материалов */
  my6PRIM *Prims;            /* массив примитивов */
  FLT Data[10];
} my6GEOM;

//extern my6PRIM prim = {0};
//extern my6MATERIAL mtl = {0};


/* Функция добавления материала к объекту */
INT MY6_GeomAddMaterial( my6GEOM *G,
                         my6MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT MY6_GeomAddPrim( my6GEOM *G, my6PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID MY6_GeomFree( my6GEOM *G );

/* Функция рисования */
VOID MY6_GeomDraw( my6GEOM *G );

VOID MY6_GeomTransform( my6GEOM *G, MATR M );

/* Функция загрузки геометрического объекта */
BOOL MY6_GeomLoad( my6GEOM *G, CHAR *FileName );

#endif /* __GEOM_H_ */

/* END OF GEOM.H FILE */
