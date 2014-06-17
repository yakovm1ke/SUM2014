/* FILENAME: ANIM.H
 * PROGRAMMER: MY6
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"
#include "render.h"
#include "shader.h"

/* Максимальное количество элементов анимации */
#define MY6_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagmy6UNIT my6UNIT;

/* Структура хранения контекста анимации */
typedef struct tagmy6ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HDC hDC;            /* Контекст screen вывода */
  HGLRC hRC;

  /* Массив элементов анимации и их количество */
  my6UNIT *Units[MY6_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE Keys[256], KeysOld[256], KeysClick[256];     /* Сотояние клавиш клавиатуры и мыши */
  INT JButs[32], JButsClick[32], JButsOld[32];
  DBL JX, JY, JZ, JU, JR, JPOV;

  /* Параметры проецирования */
  DBL
    Wp, Hp, /* размеры обрасти проецирования */
    ProjDist,      /* расстояние до плоскости проекции */
    FarClip;

  /* Матрицы */
  MATR
    MatrWorld, /* матрица преобразования мировой СК */
    MatrView,  /* матрица преобразования видовой СК */
    MatrProjection;

} my6ANIM;
extern my6ANIM MY6_Anim;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( my6UNIT *Unit, my6ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( my6UNIT *Unit, my6ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( my6UNIT *Unit, my6ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( my6UNIT *Unit, my6ANIM *Ani );
 */
#define MY6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( my6UNIT *Unit, my6ANIM *Ani );       \
  VOID (*Close)( my6UNIT *Unit, my6ANIM *Ani );      \
  VOID (*Response)( my6UNIT *Unit, my6ANIM *Ani );   \
  VOID (*Render)( my6UNIT *Unit, my6ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagmy6UNIT
{
  MY6_UNIT_BASE_FIELDS; /* Базовые поля */
};

extern UINT MY6_ShaderProg;

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
BOOL MY6_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID MY6_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID MY6_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID MY6_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID MY6_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID MY6_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID MY6_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID MY6_AnimAddUnit( my6UNIT *Unit );

/* Функция создания объекта анимации */
my6UNIT * MY6_AnimUnitCreate( INT Size );

VOID MY6_AnimResponse( VOID );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
